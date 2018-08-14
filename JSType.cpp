#include <iostream>
#include <unicode/ustream.h>
#include "JSType.h"

//Will rewrite to fit with GC


ECMAObject* ObjectCreate(ECMAObject* proto, std::list<const char*>& list){
	//ECMAObject* obj = new ECMAObject();
	//obj->internalSlots["Yo"] = nullptr;

	//delete obj;
	return nullptr;
}

ECMAObject* ObjectCreate(ECMAObject* proto){
	//ECMAObject* obj = new ECMAObject();
	//obj->internalSlots["Yo"] = nullptr; not yet


	//delete obj;
	return nullptr;
}

std::shared_ptr<ECMAString> convertASCII(const char* ascii){
	return std::shared_ptr<ECMAString>(new icu::UnicodeString(ascii, "ASCII"));
}

ECMANumber ECMAValue::getNumber() const{
	return value.numberVal;
}

ECMABool ECMAValue::getBool() const{
	return value.boolVal;
}

std::ostream& operator<<(std::ostream& stream, const ECMAValue& value){
	switch(value.type){
		case ECMAValueType::Number:
			std::cout << value.getNumber();
			break;
		case ECMAValueType::Bool:
			std::cout << value.getBool();
			break;
		default:
			std::cout << "<unknown>";
	}

	return stream;
}
