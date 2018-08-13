#ifndef ENGINE_H
#define ENGINE_H

#include "Tokenizer.h"

enum class JSType {SCRIPT = 0, MODULE = 1};


class JSEngine {
	Tokenizer* tokenizer;

	const char* readFile(const char*);
	
public:
	JSEngine();
	~JSEngine();

	void doFile(const char*, JSType = JSType::SCRIPT); /*not final type*/
};

#endif