#include "cocos2d.h"
#ifndef __GEOMETRYDASH_H__
#define __GEOMETRYDASH_H__













template <class T>
class CCPointer {
public:
    CCPointer(long address) {
        offset = (T*)address;
    }
    T get() {
        return *offset;
    }
    CCPointer& operator=(T newthing) {
        *offset = newthing;
        return *this;
    }
    operator T() {
        return *offset;
    }
    T* offset;
};



#define CLASS_PARAM(__TYPE__, __GETTER__, __OFFSET__) \
    inline CCPointer<__TYPE__> _##__GETTER__() { \
        if (this) {\
            return CCPointer<__TYPE__>((long)this + __OFFSET__); \
        } else { \
            return NULL; \
        }; \
    }

#define _UNUSED
#define _UNIMPLEMENTED
#define _BROKEN

#define GD_DLL

#define GD_VERSION_10 0x1000000

#define GD_VERSION GD_VERSION_10

#endif //__GEOMETRYDASH_H__