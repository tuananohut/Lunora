#ifndef ASSERTS_H
#define ASSERTS_H

#include "../Defines.h"

#define LASSERTIONS_ENABLED

#ifdef LASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif 

#endif
