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


		GCHandle<ECMAValue>& v0 = myGC.registerECMAValue(new ECMANumber(32.0));
		GCHandle<ECMAValue>& v1 = myGC.registerECMAValue(new ECMAString("hello wrodl"));

		myGC.markFull();
		myGC.debug();
		myGC.sweep();

		std::cout << "-----------------------" << std::endl;

		v1 = v0;

		myGC.markFull();
		myGC.debug();

	}
	//engine.doFile(argv[1]);
	return 0;
}
