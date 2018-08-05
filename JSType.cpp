#include <iostream>
#include <unicode/ustream.h>
#include "JSType.h"


ECMAObject* ObjectCreate(ECMAObject* proto, std::list<const char*>& list){
	ECMAObject* obj = new ECMAObject();
	obj->internalSlots["Yo"] = nullptr;

	delete obj;
	return nullptr;
}

ECMAObject* ObjectCreate(ECMAObject* proto){
	ECMAObject* obj = new ECMAObject();
	obj->internalSlots["Yo"] = nullptr;


	delete obj;
	return nullptr;
}

ECMAString* convertASCII(const char* ascii){
	//ECMAString* str = new icu::UnicodeString(ascii, "ASCII");
	//delete str;
	return nullptr;
}