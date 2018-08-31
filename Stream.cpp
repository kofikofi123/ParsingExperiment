#include "Stream.h"
//#include <iostream>

bool Stream::isFinished(void){
	return (position >= size);
}

bool Stream::check(uint32_t unit){
	return (getCurrent() == unit);
}

uint32_t Stream::getCurrent(void){
	return buffer->char32At(position);
}

void Stream::next(std::size_t inc){
	position = position + inc;

	if (isFinished()) position = (size);
}

void Stream::prev(std::size_t dec){
	if (position == 0) return;

	if (dec > position) position = 0;
	else position = position - dec; 
}

void Stream::reset(void){
	position = 0;
}

void Stream::save(void){
	savedPosition = position;
}

void Stream::restore(void){
	position = savedPosition;
}

void Stream::reload(const icu::UnicodeString* v){
	buffer = v;
	size = buffer->length();
	position = 0;
}

