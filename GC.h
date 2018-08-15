#ifndef GC_H
#define GC_H


#include <vector>
#include <list>
#include "JSType.h"



#define GC_DEFAULT_SIZE 100
class ECMAValue;

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
	GCHandle(const GCHandle& other): value(const_cast<ECMAValue*>(other.value)){}

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

	bool isAlive(){
		return value != nullptr;
	}
};

class GC {
	friend class GCHandle;
	std::size_t allocSize;

	std::vector<GCHandle*> roots;
	//std::vector<ECMAValue*> objects;

	std::list<ECMAValue*> white;
	std::list<ECMAValue*> grey;
	std::list<ECMAValue*> black;

	void markGreyRoots();
	void markGreyRoots(std::vector<GCHandle*>&);
	void markObjectRoots(ECMAValue*);
	void scanGreyRoots();
public:
	GC(): GC(GC_DEFAULT_SIZE){}
	GC(std::size_t);
	~GC();

	GCHandle& registerECMAValue(ECMAValue*);
	GCHandle& registerHandle(GCHandle&);
	void cleanupColors();


	void markFull();
	void sweep();
	void reset();
	void debug();

};

#endif