#ifndef GC_H
#define GC_H


#include <vector>
#include <list>
#include "JSType.h"



#define GC_DEFAULT_SIZE 100

class ECMAValue;
class ECMAString;
class ECMANumber;

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

template<class T>
class GCHandle {
	friend class GC;
	T* value;
	bool weak;
	//GC* gc;
	//bool marked;
public:

	GCHandle(): GCHandle(nullptr, false){}
	GCHandle(T* v): GCHandle(v, false){}
	GCHandle(T* v, bool w): value(v), weak(w){}

	GCHandle(GCHandle<T>& other): GCHandle(other.value, false){}
	GCHandle(GCHandle<T>& other, bool w): GCHandle(other.value, w){}

	GCHandle& operator=(const GCHandle& other){
		value = other.value;

		return *this;
	}

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
};

class GC {
	template <class T>
	friend class GCHandle;
	std::size_t allocSize;


	std::vector<GCHandle<ECMAValue>*> roots;
	//std::vector<ECMAValue*> objects;

	std::list<ECMAValue*> white;
	std::list<ECMAValue*> grey;
	std::list<ECMAValue*> black;

	void markGreyRoots();
	void markGreyRoots(std::vector<GCHandle<ECMAValue>*>&);
	void markObjectRoots(ECMAValue*);
	void scanGreyRoots();
public:
	GC(): GC(GC_DEFAULT_SIZE){}
	GC(std::size_t);
	~GC();

	GCHandle<ECMAValue>& registerECMAValue(ECMAValue*);

	GCHandle<ECMAValue>& registerHandle(GCHandle<ECMAValue>&, bool = false);
	void cleanupColors();


	void markFull();
	void sweep();
	void reset();
	void debug();

};

class JSFactory {
	GC* gc;
public:
	JSFactory(GC* g){gc=g;}

	GCHandle<ECMANumber>* createNumber(double);

	GCHandle<ECMAString>* createString();

	GCHandle<ECMAString>* createStringFromAscii(const char*);


	//GCHandle<ECMAObject>* createObject(ECMAObject* = nullptr, std::list<const char*> = {});
	
	//template<class T>
	//GCHandle<T> createCloneHandle(GCHandle<T>& h, ECMABool = false);
};

#endif