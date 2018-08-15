#include <iostream>
#include <unicode/ustream.h>
#include "JSType.h"

//Will rewrite to fit with GC

void ECMAObject::setInternalSlot(GCHandle* key, GCHandle* value){
	if (key == nullptr) return;
	internalSlots[key] = value;
}

ECMANumber ECMAValue::getNumber() const{
	return value.numberVal;
}

ECMABool ECMAValue::getBool() const{
	return value.boolVal;
}

ECMAObject* ECMAValue::getObject() const {
	return value.objectVal;
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


