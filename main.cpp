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
		GC myGC(500);
		JSFactory factory(&myGC);


		GCHandle<ECMAValue>* key = factory.createStringFromAscii("This_is_key");
    GCHandle<ECMAValue>* val = factory.createNumber(90.0);
    GCHandle<ECMAValue>* obj = factory.createObject();
    
    ECMAObject* rObj = ECMAObject::Cast(obj->get());
    
    rObj->setInternalSlot(key, val);
    
    for (auto i : rObj->internalRange){
      std::cout << "[\nKey type: " << i.first->get()->Type() << std::endl;
      std::cout << "Value type: " << i.second->get()->Type() << "\n]" << std::endl;
    }
    
    myGC.markFull();
    myGC.debug();

    
	}
	//engine.doFile(argv[1]);
	return 0;
}
