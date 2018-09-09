#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "Engine.h"
#include "Internals.h"
#include "GC.h"

int main(int argc, char* argv[]){
	if (argc > 2){
		return -1;
	}
	JSEngine engine;

	{
		GC myGC;
		JSFactory factory(&myGC);


		GCHandle<ECMAValue>* val = factory.createNumber(90);

		if (val->isAlive()){

			ECMANumber* num = ECMANumber::Cast(val->get());

			std::cout << num->Value()  << std::endl;
		}else{
			std::cout << "why though" << std::endl;
		}

		myGC.markFull();
		myGC.debug();
	}
	//engine.doFile(argv[1]);
	return 0;
}
