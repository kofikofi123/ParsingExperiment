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
	{
		GC myGC;

		GCHandle& handle = myGC.registerECMAValue(new ECMAValue(true));
		GCHandle& handle2 = myGC.registerECMAValue(new ECMAValue(32.0));

		handle = handle2;

		myGC.markFull();
		myGC.debug();
		myGC.sweep();
	}
	JSEngine engine;

	engine.doFile(argv[1]);
	return 0;
}
