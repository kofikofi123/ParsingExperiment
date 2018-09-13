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
    GCHandle<ECMAValue>* val2 = val->clone();


		myGC.markFull();
		myGC.debug();
    
    std::cout << "--------\n--------" << std::endl;
    
    val->empty();
    
    myGC.markFull();
    myGC.debug();
    
    std::cout << "--------\n--------" << std::endl;
    
    val2->empty();
    
    myGC.markFull();
    myGC.debug();
    
	}
	//engine.doFile(argv[1]);
	return 0;
}
