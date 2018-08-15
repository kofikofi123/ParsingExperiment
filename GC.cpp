#include <algorithm>
#include <iostream>
#include "GC.h"

/*Not done*/
/*Test what I have*/


GC::GC(std::size_t s){
	allocSize = s;
}

GC::~GC(){
	reset();
	for (auto i : white){
		delete i; //change this can't assume, or maybe it can through only allocation through factories
	}

	for (auto i : roots){
		delete i;
	}
}

GCHandle& GC::registerECMAValue(ECMAValue* val){
	GCHandle* handler = new GCHandle(val);

	roots.push_back(handler);
	white.push_back(val);

	return *handler;
}

GCHandle& GC::registerHandle(GCHandle& other){
	GCHandle* handler = new GCHandle(other);

	roots.push_back(handler);

	return *handler;
}

void GC::cleanupColors(){
	grey.erase(grey.begin(), grey.end());
	black.erase(black.begin(), black.end());
}

static const char* getType(ECMAValue* v){
	switch(v->getType()){
		case ECMAValueType::Number:
			return "<NUMBER>";
			break;
		case ECMAValueType::String:
			return "<String>";
			break;
		case ECMAValueType::Bool:
			return "<Bool>";
			break;
		case ECMAValueType::Object:
			return "<Object>";
			break;
		case ECMAValueType::Symbol:
			return "<Symbol>";
			break;
		default:
			return "<?Unknown?>";
			break;
	}
}

static void printObjs(const std::list<ECMAValue*>& l){
	for (auto i : l){
		std::cout << getType(i) << "(" << *(i) << ") ";
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
	cleanupColors();

	//std::vector<GCHandle*> tempRoots(roots);

	//std::copy(objects.begin(), objects.end(), std::back_inserter(white));

	markGreyRoots();
	scanGreyRoots();
}

void GC::markGreyRoots(){
	markGreyRoots(roots);
}

void GC::markGreyRoots(std::vector<GCHandle*>& pRoots){
	ECMAValue* temp = nullptr;

	for (GCHandle* i : pRoots){
		temp = i->get();

		auto p = std::find(white.begin(), white.end(), temp);

		if (p != white.end()){
			grey.push_back(temp);
			white.erase(p);
		}
	}
}

void GC::scanGreyRoots(){
	//ECMAValueType type;

	/*Idk about symbols because I haven't worked with symbols, 
	I have not used this feature at all, and have not looked at the spec for it yet*/
	ECMAValue* i = nullptr;

	//Change loop from this to basic iterator based
	for (auto it = grey.begin(); it != grey.end();){
		i = *it;
		if (i->getType() == ECMAValueType::Object){ //can do better
			markObjectRoots(i); 
		}
		black.push_back(i);
		it = grey.erase(it);
	}

}

void GC::markObjectRoots(ECMAValue* val){
	std::vector<GCHandle*> v;
	ECMAObject* obj = val->getObject();

	std::map<GCHandle*, GCHandle*>& tMap = obj->internalSlots;

	for (auto i = tMap.begin(); i != tMap.end(); i++){
		v.push_back(i->first);
		v.push_back(i->second);
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
		delete *i;
	}

	white.erase(white.begin(), white.end());

	white.swap(black);
}