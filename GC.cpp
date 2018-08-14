#include <algorithm>
#include <iostream>
#include "GC.h"

/*Not done*/
/*Test what I have*/


GC::GC(std::size_t s){
	allocSize = s;
}

GC::~GC(){
	for (auto i : objects){
		delete i; //change this can't assume, or maybe it can through only allocation through factories
	}
}

GCHandle& GC::registerECMAValue(ECMAValue* val){
	GCHandle* handler = new GCHandle(val);

	roots.push_back(handler);
	objects.push_back(val);

	return *handler;
}

void GC::cleanupColors(){
	white.erase(white.begin(), white.end());
	grey.erase(grey.begin(), grey.end());
	black.erase(black.begin(), black.end());
}

static void printObjs(const std::list<ECMAValue*>& l){
	for (auto i : l){
		std::cout << *(i) << " ";
	}
	std::cout << std::endl;
}

void GC::debug(){
	std::cout << "[Subject to deletion]" << std::endl;
	printObjs(white);
	std::cout << "[Scan area]" << std::endl;
	printObjs(grey);
}


void GC::markFull(){
	cleanupColors();

	std::vector<GCHandle*> tempRoots(roots);

	std::copy(objects.begin(), objects.end(), std::back_inserter(white));

	do {
		markGreyRoots(tempRoots);
	}while(grey.size() > 1);
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
