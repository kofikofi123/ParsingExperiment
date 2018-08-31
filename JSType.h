#ifndef JSTYPES_H
#define JSTYPES_H

#include <map>
#include <list>
#include <memory>
#include <unicode/unistr.h>
#include "GC.h"

//typename icu::UnicodeString* ECMAString;

//decided to model the V8 structure

//could be templated

template<class T>
class GCHandle;

enum class ECMAValueType {Number, Undefined, String, Boolean, Object, Value, Symbol, Reference, CompletionRecord};

class ECMAValue {
public:
	virtual ~ECMAValue(){}
	virtual ECMAValueType Type(){return ECMAValueType::Value;}
};

class ECMANumber : public ECMAValue {
	double val;
public:
	ECMANumber(): ECMANumber(0.0){}
	ECMANumber(double v): val(v){}

	ECMANumber(ECMANumber& v): ECMANumber(v.val){}

	double Value() const {return val;}
	ECMAValueType Type() override {return ECMAValueType::Number;}

	static ECMANumber* Cast(ECMAValue* v){
		return dynamic_cast<ECMANumber*>(v);
	}
};

class ECMABool : public ECMAValue {
	bool val;
public:
	ECMABool(): ECMABool(false){}
	ECMABool(bool v): val(v){}

	ECMABool(ECMABool& v): ECMABool(v.val){}

	bool Value() const {return val;}
	ECMAValueType Type() override {return ECMAValueType::Boolean;}

	static ECMABool* Cast(ECMAValue* v){
		return dynamic_cast<ECMABool*>(v);
	}
};

class ECMAString : public ECMAValue {
	icu::UnicodeString* val;
public:
	ECMAString(): ECMAString((icu::UnicodeString*)nullptr){}
	ECMAString(const char* s): ECMAString(new icu::UnicodeString(s, nullptr)){}
	ECMAString(icu::UnicodeString* s): val(s){}



	icu::UnicodeString* Value() const {return val;}
	ECMAValueType Type() override {return ECMAValueType::String;}

	static ECMAString* Cast(ECMAValue* v){
		return dynamic_cast<ECMAString*>(v);
	}
};

class ECMAObject : public ECMAValue {

public:
	ECMAObject(){}

	ECMAObject* Value() const {return nullptr;}
	ECMAValueType Type() override {return ECMAValueType::Object;}

	std::map<GCHandle<ECMAValue>*, GCHandle<ECMAValue>*> internalSlots;

	static ECMAObject* Cast(ECMAValue* v){
		return dynamic_cast<ECMAObject*>(v);
	}
};

class ECMACompletionRecord : public ECMAValue {

public:

	ECMACompletionRecord(){}


	ECMACompletionRecord* Value() const {return nullptr;}
	ECMAValueType Type() override {return ECMAValueType::CompletionRecord;}
};


class ECMAReference : public ECMAValue {
	ECMAValue* base;
	ECMAValue* name;
	bool strict;

	friend ECMAValue* GetBase(ECMAValue*);
	friend ECMAValue* GetReferencedName(ECMAValue*);
	friend bool IsStrictReference(ECMAValue*);
	friend bool HasPrimitiveBase(ECMAValue*);
	friend bool IsPropertyReference(ECMAValue*);
	friend bool IsUnresolvableReference(ECMAValue*);
	friend bool IsSuperReference(ECMAValue*);
	friend ECMAValue* GetValue(ECMAValue*);
public:
	ECMAReference(): ECMAReference(nullptr, nullptr, false){}
	ECMAReference(ECMAValue* b, ECMAValue* r): ECMAReference(b, r, false){}
	ECMAReference(ECMAValue* b, ECMAValue* r, bool s): base(b), name(r), strict(s){}


	ECMAReference* Value() const {return nullptr;}
	ECMAValueType Type() override {return ECMAValueType::Reference;}

	static ECMAReference* Cast(ECMAValue* v){
		return dynamic_cast<ECMAReference*>(v);
	}
};

ECMAValue* GetBase(ECMAValue*);
ECMAValue* GetReferencedName(ECMAValue*);
bool IsStrictReference(ECMAValue*);
bool HasPrimitiveBase(ECMAValue*);
bool IsPropertyReference(ECMAValue*);
bool IsUnresolvableReference(ECMAValue*);
bool IsSuperReference(ECMAValue*);
ECMAValue* GetValue(ECMAValue*);

ECMAValueType typeof(ECMAValue*);
bool typeofIsPrimative(ECMAValue*);


#endif