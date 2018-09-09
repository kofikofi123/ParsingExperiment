#ifndef ENGINE_H
#define ENGINE_H

#include "GC.h"
#include "Tokenizer.h"

enum class JSType {SCRIPT = 0, MODULE = 1};



class JSEngine {
	Tokenizer* tokenizer;
	GC* pGC;

	const char* readFile(const char*);
public:

	JSEngine();
	~JSEngine();

	//JSFactory* factory;

	//void mark();
	//void debug();
	//void sweep();
	void doFile(const char*, JSType = JSType::SCRIPT); /*not final type*/
};

#endif