#ifndef ENGINE_H
#define ENGINE_H

#include "Tokenizer.h"

class JSEngine {
	Tokenizer* tokenizer;

	const char* readFile(const char*);

public:
	JSEngine();
	~JSEngine();

	void doFile(const char*); /*not final type*/
};

#endif