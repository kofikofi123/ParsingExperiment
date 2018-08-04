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

struct ECMAValue {
	ECMAValueType type;
	union {
		ECMAString* stringVal;
		ECMANumber numberVal;
		ECMABool boolValue;
		ECMAObject* objectValue;
	}value;
};

class ECMAObject {
	std::map<std::shared_ptr<ECMAString>, ECMAValue*> internalSlots;

public:
	ECMAObject(){}
	friend ECMAObject* ObjectCreate(ECMAObject*, std::list<ECMAString*>*);
};

ECMAObject* ObjectCreate(ECMAObject*, std::list<ECMAString*>*);
ECMAString* convertASCII(const char*);



#endif