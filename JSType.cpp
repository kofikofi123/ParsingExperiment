#include <iostream>
#include <unicode/ustream.h>
#include "JSType.h"

//Will rewrite to fit with GC


inline ECMAValueType typeof(ECMAValue* v){
	return v->Type();
}

bool typeofIsPrimative(ECMAValue* v){
	if (v == nullptr) return false; //  :(
	bool result = false;

	switch (typeof(v)){
		case ECMAValueType::String:
		case ECMAValueType::Number:
		case ECMAValueType::Boolean:
		case ECMAValueType::Symbol:
			result = true;
			break;
		default:
			break;
	}

	return result;
}


//could do some checkng here
ECMAValue* GetBase(ECMAValue* v){
	return ECMAReference::Cast(v)->base;
}

ECMAValue* GetReferencedName(ECMAValue* v){
	return ECMAReference::Cast(v)->name;
}

bool IsStrictReference(ECMAValue* v){
	return ECMAReference::Cast(v)->strict;
}

bool HasPrimitiveBase(ECMAValue* v){
	return typeofIsPrimative(GetBase(v));
}

bool IsPropertyReference(ECMAValue* v){
	return (HasPrimitiveBase(v) || typeof(GetBase(v)) == ECMAValueType::Object);
}

bool IsUnresolvableReference(ECMAValue* v){
	return (typeof(GetBase(v)) == ECMAValueType::Undefined);
}

ECMAValue* GetValue(ECMAValue* v){
	if (typeof(v) != ECMAValueType::Reference)
		return v;

	ECMAValue* base = GetBase(v);

	//if unresolved 
	if (IsUnresolvableReference(v)){

	}

	if (IsPropertyReference(v)){
		if (HasPrimitiveBase(v)){
		}
	}

	return nullptr;
}