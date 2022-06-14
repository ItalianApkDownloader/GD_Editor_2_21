#pragma once
#include "AccountLayer.h"
class  AccountLayerExt : public AccountLayer {
public:

    static inline void (*accload)(AccountLayer*);
    void accload_hk(); 

};
