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
        for(int k = 0; k < patches.size(); k++){
            patches[k].Modify();
        }
    }

    void Restore(){
        for(int k = 0; k < patches.size(); k++){
            patches[k].Restore();
        }
    }

};