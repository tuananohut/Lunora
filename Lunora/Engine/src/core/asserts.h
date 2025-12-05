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

LAPI void report_assertion_failed(const char *expression, const char *message, const char *file, i32 line);

#define LASSERT(expr)                                                      \
    do {                                                                   \
        if (expr) {                                                        \
        } else {                                                           \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);       \
            debugBreak();                                                  \
        }                                                                  \
    } while (0)

#define LASSERT_MSG(expr, message)				\
  do {								\
    if (expr) {							\
    } else {							\
      report_assertion_failure(#expr, "", __FILE__, __LINE__);	\
      debugBreak();						\
    }								\
  } while (0)

#ifdef _DEBUG
#define LASSERT_DEBUG(expr)                                                \
    do {                                                                   \
        if (expr) {                                                        \
        } else {                                                           \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);       \
            debugBreak();                                                  \
        }                                                                  \
    } while (0)
#else
#define LASSERT_DEBUG(expr)
#endif

#else
#define LASSERT(expr)
#define LASSERT_MSG(expr, message)
#define LASSERT_DEBUG(expr)
#endif

#endif
