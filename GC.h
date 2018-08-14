#ifndef GC_H
#define GC_H


#include <vector>
#include <list>
#include "JSType.h"

#define GC_DEFAULT_SIZE 100

/*
	A small, somewhat, garbage collector

	Of course, no where finished

	Will probably need to add padding
*/
//class GC;

class GCHandle {
	friend class GC;
	ECMAValue* value;
	//GC* gc;
	//bool marked;
public:

	GCHandle(): value(nullptr){}
	GCHandle(ECMAValue* v): value(v){}

	GCHandle& operator=(const GCHandle& other){
		value = other.value;

		return *this;
	}

	ECMAValue& operator*(){
		return *value;
	}

	ECMAValue* operator->(){
		return value;
	}

	ECMAValue* get(){
		return value;
	}
};

class GC {
	friend class GCHandle;
	std::size_t allocSize;

	std::vector<GCHandle*> roots;
	std::vector<ECMAValue*> objects;

	std::list<ECMAValue*> white;
	std::list<ECMAValue*> grey;
	std::list<ECMAValue*> black;

	void markGreyRoots(std::vector<GCHandle*>&);
public:
	GC(): GC(GC_DEFAULT_SIZE){}
	GC(std::size_t);
	~GC();

	GCHandle& registerECMAValue(ECMAValue*);
	void cleanupColors();


	void markFull();
	void debug();
};

#endif