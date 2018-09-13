#ifndef GC_H
#define GC_H


#include <vector>
#include <list>
#include <memory>
#include <cstdint>
#include "JSType.h"



#define GC_DEFAULT_SIZE 100

class ECMAValue;
class ECMAString;
class ECMANumber;

class GC;

//TODO: make GC allocate emcavalue's itself

/*
typedef icu::UnicodeString ECMAString;
typedef double ECMANumber;
typedef bool ECMABool;
class ECMAValue;
class ECMAObject;
*/

/*
	A small, somewhat, garbage collector

	Of course, no where finished

	Will probably need to add padding
*/
//class GC;
class GCAllocator {
	std::size_t max;
	char* space;
	char* end;

	struct Node {
		uint32_t spaceSize;
		Node* next;
	};

	Node head;

	Node* findSpace(uint32_t);
	Node* prev(Node*);
	Node* findNearestNode(Node*);

	void cleanSweep();
	void unlink(Node*);
public:
	GCAllocator(std::size_t m): max(m){
		space = static_cast<char*>(std::malloc(m));
		if (space == nullptr) return;

		end = space + (m - 1);

		head.spaceSize = max;


		Node* node = reinterpret_cast<Node*>(space);
		node->spaceSize = max;
		node->next = nullptr;

		head.next = node;
	}

	~GCAllocator(){
		std::free(space);
	}


	template <class T>
	T* allocate(uint32_t);

	template <class T, class ... Args>
	void construct(void*, Args&&...);

	template <class T>
	void deconstruct(T*);


	template <class T>
	void deallocate(T*, uint32_t);
};


template<class T>
class GCHandle {
	friend class GC;
	GC* gc;
	T* value;
	bool weak;
	//bool marked;
public:
	GCHandle(GC* g): GCHandle(g, nullptr, false){}
	GCHandle(GC* g, T* v): GCHandle(g, v, false){}
	GCHandle(GC* g, T* v, bool w): gc(g), value(v), weak(w){}

	GCHandle(GC* g, const GCHandle<T>* other): GCHandle(g, other->value, false){}
	GCHandle(GC* g, const GCHandle<T>* other, bool w): GCHandle(g, other->value, w){}
  
	GCHandle& operator=(const GCHandle& other){
		value = other.value;

		return *this;
	}

	GCHandle& operator=(T* other){
		value = other;

		return *this;
	}
  
	GCHandle<T>* clone();

	T& operator*(){
		return *value;
	}

	ECMAValue* operator->(){
		return value;
	}

	ECMAValue* get(){
		return value;
	}

	bool isWeak(){
		return weak;
	}

	bool isAlive(){
		return value != nullptr;
	}

	void empty(){
		value = nullptr;
	}
};


class GC {
	template <class T>
	friend class GCHandle;
	std::size_t allocSize;

	GCAllocator* allocator;

	std::vector<GCHandle<ECMAValue>*> roots;
	//std::vector<ECMAValue*> objects;

	std::list<ECMAValue*> white;
	std::list<ECMAValue*> grey;
	std::list<ECMAValue*> black;

	void markGreyRoots();
	void markGreyRoots(std::vector<GCHandle<ECMAValue>*>&);
	void markObjectRoots(ECMAValue*);
	void killHandle(GCHandle<ECMAValue>*);
	void scanGreyRoots();
	inline void emergencySweep(){ //save for later
		markFull();
		sweep();
	}

	template <class T>
	T* allocate(std::size_t = 1);

	template <class T>
	void deallocate(T*, std::size_t);
public:
	GC(): GC(GC_DEFAULT_SIZE){}
	GC(std::size_t);
	~GC();

	template <class T, class ... Args>
	GCHandle<ECMAValue>* registerECMAValue(Args&&...);

	GCHandle<ECMAValue>* registerHandle(GCHandle<ECMAValue>*, bool = false);
	void cleanupColors();


	void markFull();
	void sweep();
	void reset();
	void debug();

};

class JSFactory {
	GC* gc;
public:
	JSFactory(GC* g): gc(g){}

	GCHandle<ECMAValue>* createNumber(double);

	GCHandle<ECMAValue>* createString();

	GCHandle<ECMAValue>* createStringFromAscii(const char*);

	GCHandle<ECMAValue>* createCloneHandle(GCHandle<ECMAValue>*, bool = false);

	GCHandle<ECMAValue>* createEmptyHandler();

	template <class T, class ... Args>
	GCHandle<ECMAValue>* create(Args&&...);


	//GCHandle<ECMAObject>* createObject(ECMAObject* = nullptr, std::list<const char*> = {});
	
	//template<class T>
	//GCHandle<T> createCloneHandle(GCHandle<T>& h, ECMABool = false);
};

#endif