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

void report_assertion_failed(const char *expression, const char *message, const char *file, i32 line);

#define LASSERT(expr)
{
  if (expr) {
  } else {
    report_assertion_failure(#expr, "", __FILE__, __LINE__);
    debugBreak(); 
  }
}

#define LASSERT_MSG(expr, message)
{
  if (expr) { 
  } else {
    report_assertion_failure(#expr, "", __FILE__, __LINE__);
    debugBreak();
  }
}

#endif
