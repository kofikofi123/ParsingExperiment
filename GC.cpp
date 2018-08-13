#include "GC.h"

/*Not done*/
/*Haven't test*/
/*Haven't even compiled*/


GC::GC(std::size_t s){
	allocSize = s;
}


GCHandle& GC::registerECMAValue(ECMAValue* val){
	GCHandle* handler = new GCHandle(val);

	roots.push_back(handler);
	objects.push_back(val);

	return *handler;
}

void GC::cleanupColors(){
	white.erase(white.begin(), white.end());
	grey.erase(grey.begin(), grey.end());
	black.erase(black.begin(), black.end());
}


void GC::mark(){
	cleanupColors();

	std::vector<GCHandle*> tempRoots(roots);
	std::copy(objects.begin(), objects.end(), std::back_inserter(white));

	do {
		if (grey.size() > 0){
			std::copy(grey.begin(), grey.end(), std::back_inserter(black));
			grey.erase(grey.begin(), grey.end());
		}/* sleepy, will do later
		for (GCHandle* i : tempRoots){
			if (i.get() == tempRoots)
				grey.push_back(tempRoots);
		}*/
	}while(grey.size() > 1);
}