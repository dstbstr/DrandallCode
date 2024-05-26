#pragma once
#include "Core/Macros/PreProcessorOverride.h"

//Because __VA_ARGS__ count is essentially the number of commas plus 1, the preprocessor can't distinguish between
// 0 and 1 argument.  To get around this, we pass a dummy argument, which we always ignore, so now _STRINGIFY_ARGS_2 becomes the single argument version
// and _STRINGIFY_ARGS_1 becomes the no-args version.  Kudos to anyone who can simplify this in PreProcessorOverride.h

#define _STRINGIFY_ARGS_1(ignore)
#define _STRINGIFY_ARGS_2(ignore, x, ...) #x
#define _STRINGIFY_ARGS_3(ignore, x, ...) #x, EXPAND(_STRINGIFY_ARGS_2(ignore, __VA_ARGS__))
#define _STRINGIFY_ARGS_4(ignore, x, ...) #x, EXPAND(_STRINGIFY_ARGS_3(ignore, __VA_ARGS__))
#define _STRINGIFY_ARGS_5(ignore, x, ...) #x, EXPAND(_STRINGIFY_ARGS_4(ignore, __VA_ARGS__))
#define _STRINGIFY_ARGS_6(ignore, x, ...) #x, EXPAND(_STRINGIFY_ARGS_5(ignore, __VA_ARGS__))
#define _STRINGIFY_ARGS_7(ignore, x, ...) #x, EXPAND(_STRINGIFY_ARGS_6(ignore, __VA_ARGS__))
#define _STRINGIFY_ARGS_8(ignore, x, ...) #x, EXPAND(_STRINGIFY_ARGS_7(ignore, __VA_ARGS__))
#define _STRINGIFY_ARGS_9(ignore, x, ...) #x, EXPAND(_STRINGIFY_ARGS_8(ignore, __VA_ARGS__))

#define STRINGIFY_ARGS(...) MACRO_OVERRIDE(_STRINGIFY_ARGS_, ignore, __VA_ARGS__)
