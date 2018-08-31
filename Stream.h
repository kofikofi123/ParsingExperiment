#ifndef STREAM_H
#define STREAM_H

#include <list>
#include <vector>
#include <iostream>
#include <unicode/unistr.h>

class Stream {
	//const std::vector<uint32_t>& buffer;
	const icu::UnicodeString* buffer;
	std::size_t size;
	std::size_t position;
	std::size_t savedPosition;
public:
	Stream(): buffer(nullptr), size(0), position(0), savedPosition(0){}
	Stream(const icu::UnicodeString* v): buffer(v), size(buffer->length()), position(0), savedPosition(0){std::cout << "testing" << std::endl;}

	bool check(uint32_t);	
	bool isFinished(void);

	uint32_t getCurrent(void);

	void next(std::size_t = 1);
	void prev(std::size_t = 1);
	void reset(void);
	void reload(const icu::UnicodeString*);
	void save(void);
	void restore(void);

	std::size_t getPos(){return position;}
};

#endif