#ifndef LOGGER_H
#define LOGGER_H

#include "../defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1 

#if LRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif 

enum log_level {
  LOG_LEVEL_FATAL = 0,  
  LOG_LEVEL_ERROR = 1,
  LOG_LEVEL_WARN = 2,
  LOG_LEVEL_INFO = 3,
  LOG_LEVEL_DEBUG = 4,
  LOG_LEVEL_TRACE = 5
};

b8 initialize_logging();
void shutdown_logging(); 

LAPI void log_output(log_level level, const char* message, ...); 

#define LFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, __VA_ARGS__); 

#ifndef LERROR
#define LERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, __VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#define LWARN(message, ...) log_output(LOG_LEVEL_WARN, message, __VA_ARGS__);
#else
#define LWARN(message, ...)
#endif 

#if LOG_INFO_ENABLED == 1
#define LINFO(message, ...) log_output(LOG_INFO_ENABLED, message, __VA_ARGS__);
#else
#define LINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define LDEBUG(message, ...) log_output(LOG_DEBUG_ENABLED, message, __VA_ARGS__);
#else
#define LDEBUG(message, ...)
#endif 

#if LOG_TRACE_ENABLED == 1
#define LTRACE(message, ...) log_output(LOG_TRACE_ENABLED, message, __VA_ARGS__);
#else
#define LTRACE(message, ...)
#endif 

#endif
