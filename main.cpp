#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "Engine.h"
#include "Internals.h"


int main(int argc, char* argv[]){
	if (argc > 2){
		return -1;
	}
	{
		DeclEnvRecord r;
		std::shared_ptr<CompletionRecord> comp;

		std::shared_ptr<ECMAString> test = convertASCII("A");

		comp = r.CreateMutableBinding(test.get(), true);
		comp = r.InitializeBinding(test.get(), val);


		ECMAValue* va = r.GetBindingValue(test.get(), true);

		std::cout << *va << std::endl;
	}
	JSEngine engine;

	engine.doFile(argv[1]);
	return 0;
}