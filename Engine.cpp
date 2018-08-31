#include <cstdint>
#include <iostream>
#include <fstream>
#include "Engine.h"
#include "Internals.h"
#include "unicode/unistr.h"

JSEngine::JSEngine(){
	//pGC = new GC();	
	//factory = new JSFactory(pGC);
	tokenizer = new Tokenizer();
}

const char* JSEngine::readFile(const char* filename){
	std::ifstream stream(filename, std::ios::in);

	if (!stream.is_open()) return nullptr;

	stream.seekg(0, stream.end);
	std::size_t size = stream.tellg();
	stream.seekg(0, stream.beg);

	char* buffer = new char[size + 1];

	buffer[size] = 0;

	stream.read(buffer, size);

	stream.close();

	return buffer;
}

void JSEngine::doFile(const char* filename, JSType type){
	const char* content = readFile(filename);
	
	icu::UnicodeString str(content);

	std::list<TokenPtr>* tokens = tokenizer->tokenize(&str);

	/*if (tokenizer->errors.size() > 0){
		std::cout << "Errors" << std::endl;
	}*/
	for (auto i : *tokens){
		std::cout << *i << std::endl;
	}

	//ObjectCreate(nullptr);
	delete tokens;
	//td::cout << std::hex << std::showbase << str.char32At(4) << std::endl;
	delete[] content;
}	
/*
void JSEngine::mark(){
	pGC->markFull();
}

void JSEngine::debug(){
	pGC->debug();
}

void JSEngine::sweep(){
	pGC->sweep();
}*/


JSEngine::~JSEngine(){
	//delete factory;
	//delete pGC;
	delete tokenizer;
}
