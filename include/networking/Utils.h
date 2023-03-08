#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <string>
#include "Socket.h"

#ifndef PASSWORD_CALLBACK_PTR
#define PASSWORD_CALLBACK_PTR
typedef std::string(*passwordCallbackPtr)(void);
#endif

namespace Utils
{
    int randomInt(int min, int max);
    int randomInt(int max);

    unsigned long randomUL(unsigned long min = 0, unsigned long max = ULONG_MAX);

    template <class _InIt, class _Ty>
    _InIt find(_InIt first, _InIt last, const _Ty& val)
    {
        for (_InIt it = first; it != last; ++it)
            if (*it == val)
                return it;
    }
}

#endif // !UTILS_H
