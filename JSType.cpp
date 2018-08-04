#include <iostream>
#include <unicode/ustream.h>
#include "JSType.h"


ECMAObject* ObjectCreate(ECMAObject* proto, std::list<ECMAString*>* list){
	ECMAObject* obj = new ECMAObject();
	//obj->internalSlots[std::make_shared<ECMAString>(convertASCII("Yo"))] = nullptr;
	std::shared_ptr<ECMAString> p(convertASCII("Hero"));

	//std::cout << *p << std::endl;

	delete obj;
	return nullptr;
}

ECMAString* convertASCII(const char* ascii){
	//ECMAString* str = new icu::UnicodeString(ascii, "ASCII");
	//delete str;
	return nullptr;
}