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


std::weak_ptr<ECMAValue> ECMAValueType::newValue(ECMAValue* val){
	std::shared_ptr<ECMAValue> ptr(val);
	pool.push_back(val);
	return std::weak_ptr<ECMAValue>(ptr);	
}