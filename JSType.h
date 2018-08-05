#ifndef JSTYPES_H
#define JSTYPES_H

#include <map>
#include <list>
#include <memory>
#include <unicode/unistr.h>

typedef icu::UnicodeString ECMAString;
typedef double ECMANumber;
typedef bool ECMABool;
class ECMAObject;

enum class ECMAValueType {String=0,Number,Bool,Object, Null, Undefined};

class ECMAValue {

	union {
		ECMAString* stringVal;
		ECMANumber numberVal;
		ECMABool boolValue;
		ECMAObject* objectValue;
	}value;
public:
	ECMAValue(ECMABool);
	ECMAValue(ECMANumber);
	ECMAValue(ECMAString*);
	ECMAValue(ECMAObject*);


	auto getValue();
	ECMAValueType getType();
};

class ECMAObject {
	std::map<const char*, ECMAValue*> internalSlots;

public:
	ECMAObject(){}


	friend ECMAObject* ObjectCreate(ECMAObject*);
	friend ECMAObject* ObjectCreate(ECMAObject* , std::list<const char*>&);
};

ECMAString* convertASCII(const char*);
ECMAObject* ObjectCreate(ECMAObject* , std::list<const char*>&);
ECMAObject* ObjectCreate(ECMAObject*);


#endif