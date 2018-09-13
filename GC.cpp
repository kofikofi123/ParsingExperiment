#include <algorithm>
#include <iostream>
#include "GC.h"

/*Not done*/
/*Tested what I have*/


GC::GC(std::size_t s){
	allocSize = s; //not in use yet

	allocator = new GCAllocator(s);
}

GC::~GC(){
	reset();
	for (auto i = white.begin(); i != white.end(); i++) {

		allocator->deconstruct(*i);
	}


	for (auto i = roots.begin(); i != roots.end(); i++){
		allocator->deconstruct(*i);
	}
  
  delete allocator;
}

template <class T, class ... Args>
GCHandle<ECMAValue>* GC::registerECMAValue(Args&&... args){
	ECMAValue* val = allocate<T>(1);
	GCHandle<ECMAValue>* handler = allocate<GCHandle<ECMAValue>>(1);

	allocator->construct<T>(val, std::forward<Args>(args)...);

	allocator->construct<GCHandle<ECMAValue>>(handler, this, val);

	roots.push_back(handler);

	if (val != nullptr && std::find(white.begin(), white.end(), val) == white.end())
		white.push_back(val);

	return handler;
}

GCHandle<ECMAValue>* GC::registerHandle(GCHandle<ECMAValue>* other, bool weak){
	GCHandle<ECMAValue>* handler = allocate<GCHandle<ECMAValue>>(1);
  
	allocator->construct<GCHandle<ECMAValue>>(handler, this, other, weak);
  
	roots.push_back(handler);

	return handler;
}

template <class T>
T* GC::allocate(std::size_t n){
	T* obj = allocator->allocate<T>(sizeof(T) * n);

	if (obj == nullptr){
		emergencySweep(); //save for later
		obj = allocator->allocate<T>(n);

		if (obj == nullptr){
			throw std::bad_alloc();
    }
	}

	return obj;
}

template <class T>
void GC::deallocate(T* ptr, std::size_t n){
	allocator->deallocate<T>(ptr, sizeof(T) * n);
}

void GC::cleanupColors(){
	grey.erase(grey.begin(), grey.end());
	black.erase(black.begin(), black.end());
}



static void printObjs(const std::list<ECMAValue*>& l){
	for (auto i : l){
		std::cout << i->Type() << " ";
	}
	std::cout << std::endl;
}

void GC::debug(){
	std::cout << "[Subject to deletion]" << std::endl;
	printObjs(white);
	std::cout << "[Scan area]" << std::endl;
	printObjs(grey);
	std::cout << "[Safe]" << std::endl;
	printObjs(black);
}

//not done
void GC::markFull(){
  reset();

	//std::vector<GCHandle*> tempRoots(roots);

	//std::copy(objects.begin(), objects.end(), std::back_inserter(white));

	markGreyRoots();
	scanGreyRoots();
}

void GC::markGreyRoots(){
	markGreyRoots(roots);
}

void GC::markGreyRoots(std::vector<GCHandle<ECMAValue>*>& pRoots){
	ECMAValue* temp = nullptr;

	for (GCHandle<ECMAValue>* i : pRoots){
		temp = i->get();

		auto p = std::find(white.begin(), white.end(), temp);

		if (p != white.end() && !(i->weak)){
			grey.push_back(temp);
			white.erase(p);
		}
	}
}



void GC::scanGreyRoots(){
	//ECMAValueType type;

	//Idk about symbols because I haven't worked with symbols, 
	//I have not used this feature at all, and have not looked at the spec for it yet
	ECMAValue* i = nullptr;

	//Change loop from this to basic iterator based
	for (auto it = grey.begin(); it != grey.end();){
		i = *it;
		if (i->Type() == ECMAValueType::Object){ //can do better, maybe
			markObjectRoots(i); 
		}
		black.push_back(i);
		it = grey.erase(it);
	}

}

void GC::markObjectRoots(ECMAValue* val){
	std::vector<GCHandle<ECMAValue>*> v;
	ECMAObject* obj = ECMAObject::Cast(val);

	/*
	std::map<GCHandle<ECMAValue>*, GCHandle<ECMAValue>*> map;

	for (auto i = tMap.begin(); i != tMap.end(); i++){
		v.push_back(i->first);
		v.push_back(i->second);
	}*/

	for (auto i : obj->internalRange){
		v.push_back(i.first);
		v.push_back(i.second);
	}

	markGreyRoots(v);

	//haven't done object's property
}

void GC::reset(){
	std::copy(black.begin(), black.end(), std::back_inserter(white));
	std::copy(grey.begin(), grey.end(), std::back_inserter(white));
	cleanupColors();
	//std::copy(objects.begin(), objects.end(), std::back_inserter(white));
}

void GC::sweep(){
	for (auto i = white.begin(); i != white.end(); i++){
		ECMAValue* val = *i;

		allocator->deconstruct(val);
		allocator->deallocate(val, 1);
	}

	white.erase(white.begin(), white.end());

	white.swap(black);
}

GCHandle<ECMAValue>* JSFactory::createNumber(double num){
	return create<ECMANumber>(num);
}

GCHandle<ECMAValue>* JSFactory::createString(){
	return createStringFromAscii(nullptr);
}

GCHandle<ECMAValue>* JSFactory::createStringFromAscii(const char* ascii){
	return create<ECMAString>(ascii);
}

GCHandle<ECMAValue>* JSFactory::createObject(){
  return create<ECMAObject>();
}

GCHandle<ECMAValue>* JSFactory::createCloneHandle(GCHandle<ECMAValue>* handle, bool isWeak){
	return gc->registerHandle(handle, isWeak);
}

GCHandle<ECMAValue>* JSFactory::createEmptyHandler(){
	return gc->registerHandle(nullptr);
}

template <class T, class ... Args>
GCHandle<ECMAValue>* JSFactory::create(Args&&... args){
	return gc->registerECMAValue<T>(std::forward<Args>(args)...);
}

template <class T>
T* GCAllocator::allocate(uint32_t n){
	Node* loc = findSpace(n);

	uint32_t max_size = 4 + n;

	if (loc == nullptr) {
		return nullptr;
	}

	Node* l_end = reinterpret_cast<Node*>(reinterpret_cast<char*>(loc) + (max_size));
	Node* next = loc->next;
	Node* past = prev(loc);

	if (next == nullptr){
		char* cEnd = (char*)l_end;

		if (cEnd >= end)
			past->next = nullptr;
		else {
			l_end->spaceSize = (end - cEnd);
			l_end->next = nullptr;

			past->next = l_end;
		}
	}

	uint32_t* sPtr = (uint32_t*)loc;
	*sPtr = max_size;

	sPtr++;

	return reinterpret_cast<T*>(sPtr);
}

template <class T, class ... Args>
void GCAllocator::construct(void* ptr, Args&&... args){
	new (ptr) T (std::forward<Args>(args)...);
}

GCAllocator::Node* GCAllocator::findSpace(uint32_t size){
	Node* h = head.next;

	size = size + 4;
	while (h != nullptr){
		if (h->spaceSize >= size)
			return h;
		h = h->next;
	}

	return nullptr;
}

void GCAllocator::unlink(Node* node){
	Node* pHead = &head;

	while (pHead->next != nullptr){
		if (pHead == node){
			pHead->next = node->next;
			return;
		}
		pHead = pHead->next;
	}
}

typename GCAllocator::Node* GCAllocator::prev(Node* node){
	Node* pHead = &head;

	while (pHead->next != nullptr){
		if (pHead->next == node)
			return pHead;
		pHead = pHead->next;
	}

	return nullptr;
}

template <class T>
void GCAllocator::deallocate(T* ptr, uint32_t siz){
	if (ptr == nullptr) return;
	char* tempA = (char*)ptr;

	if (space > tempA || tempA > end){
		return;
	}

	uint32_t* tempB = (uint32_t*)ptr;
	uint32_t rSize = 0;

	tempB--;

	rSize = *tempB;

	Node* n = (Node*)(tempB);

	n->spaceSize = rSize;

	Node* p = findNearestNode(n);

	n->next = p->next;
	p->next = n;

	cleanSweep();

}


GCAllocator::Node* GCAllocator::findNearestNode(Node* node){
	Node* prev = &head;
	Node* current = prev->next;

	while (current != nullptr) {
		if (current > node)
			return prev;

		prev = current;
		current = current->next;
	}

	return nullptr;
}

void GCAllocator::cleanSweep(){
	Node* current = head.next;
	Node* next = current->next;
	Node* l_end = nullptr;
	while (next != nullptr) {
		l_end = (Node*)((char*)current + current->spaceSize - 1);

		if (l_end >= next){
			current->next = next->next;
		}

		current = next;
		next = current->next;
	}	
}


template <class T>
void GCAllocator::deconstruct(T* ptr){
	ptr->~T();
}

template <class T>
GCHandle<T>* GCHandle<T>::clone(bool weak){
  return gc->registerHandle(this, weak);
}


template GCHandle<ECMAValue>* GCHandle<ECMAValue>::clone(bool);