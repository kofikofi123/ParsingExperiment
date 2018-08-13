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
	ECMAValue(ECMAObject*);


	ECMANumber getNumber() const;
	ECMABool getBool() const;
	ECMAValueType getType() const;

	friend std::ostream& operator<<(std::ostream&, const ECMAValue&);
};

class ECMAObject {
	std::map<const char*, ECMAValue*> internalSlots;

public:
	ECMAObject(ECMAObject*);
	ECMAObject(ECMAObject*, std::list<const char*>&);

};

std::shared_ptr<ECMAString> convertASCII(const char*);
//ECMAObject* ObjectCreate(ECMAObject* , std::list<const char*>&);
//ECMAObject* ObjectCreate(ECMAObject*);


#endif