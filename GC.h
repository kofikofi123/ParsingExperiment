#ifndef GC_H
#define GC_H

#include "JSTypes.h"

#define GC_DEFAULT_SIZE 100

/*
	A small, somewhat, garbage collector

	Of course, no where finished

	Will probably need to add padding
*/
class GC;

class GCHandle {
	friend class GC;
	ECMAValue* value;

	//bool marked;
public:

	GCHandle(): value(nullptr){}
	GCHandle(ECMAValue* v): value(v){}

	ECMAValue& operator*(){
		return *value;
	}

	ECMAValue* operator->(){
		return value;
	}


};

class GC {
	std::size_t allocSize;

	std::vector<GCHandle*> roots;
	std::vector<ECMAValue*> objects;

	std::list<GCHandle*> white;
	std::list<GCHandle*> grey;
	std::list<GCHandle*> black;
public:
	GC(): GC(GC_DEFAULT_SIZE){}
	GC(std::size_t);

	GCHandle& registerECMAValue(ECMAValue*);
	void cleanupColors();


	void mark();
}

#endif