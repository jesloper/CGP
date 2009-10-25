#ifndef COMMON_H
#define COMMON_H

#ifdef WIN32
#include <windows.h>
#include <winbase.h>

inline void sleep(unsigned int seconds)
{
    Sleep(seconds*1000);
}
#endif

#endif // COMMON_H
