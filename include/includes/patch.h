#pragma once
#include "KittyMemory/MemoryPatch.h"


class patch{
	
private:
    std::vector<MemoryPatch> patches;
public:
    void addPatch(const char *libraryName, uintptr_t address,std::string hex){
        patches.push_back(MemoryPatch::createWithHex(libraryName,address,hex));
    }
	
	void addPatch(uintptr_t address,std::string hex) {
		addPatch("libcocos2dcpp.so", address, hex);
	}
	
	void reserve(size_t size) { patches.reserve(size); }

    void Modify(){
		for(auto& patch : patches)
			patch.Modify();
    }

    void Restore(){
		for(auto& patch : patches)
			patch.Restore();
    }

};