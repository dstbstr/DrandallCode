#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef MSVC
#include "Private/WindowsPlatform.h"
#else
#include "Private/UnixPlatform.h"
#endif

#endif // __PLATFORM_H__