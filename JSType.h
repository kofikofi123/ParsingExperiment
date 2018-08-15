#ifndef JSTYPES_H
#define JSTYPES_H

#include <map>
#include <list>
#include <memory>
#include <unicode/unistr.h>
#include "GC.h"

typedef icu::UnicodeString ECMAString;
typedef double ECMANumber;
typedef bool ECMABool;
class ECMAObject;

class GC;
class GCHandle;

enum class ECMAValueType {String,Number,Bool,Object, Null, Undefined, Symbol};

class ECMAValue {
	ECMAValueType type;
	union {
		ECMAString* stringVal;
		ECMANumber numberVal;
		ECMABool boolVal;
		ECMAObject* objectVal;
	}value;
public:
	ECMAValue(ECMABool v): type(ECMAValueType::Bool){value.boolVal=v;}
	ECMAValue(ECMANumber v): type(ECMAValueType::Number){value.numberVal = v;}
	ECMAValue(ECMAString*);
	ECMAValue(ECMAObject* v): type(ECMAValueType::Object){value.objectVal=v;}


	ECMANumber getNumber() const;
	ECMABool getBool() const;
	ECMAObject* getObject() const;
	ECMAValueType getType(){return type;}

	friend std::ostream& operator<<(std::ostream&, const ECMAValue&);
};

class ECMAObject {
	friend class GC;
	std::map<GCHandle*, GCHandle*> internalSlots;

public:
	ECMAObject(GCHandle*);
	ECMAObject(GCHandle*, std::list<const char*>&);

	void setInternalSlot(GCHandle*, GCHandle*);
	GCHandle* getInternalSlot(GCHandle*);
};	

GCHandle& convertASCII(const char*);

#endif