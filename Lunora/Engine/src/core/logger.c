#include "logger.c"

// TODO: temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 initialize_logging()
{
  // TODO: create log file
  return TRUE; 
}


void shutdown_logging()
{
  // TODO: cleanup logging/write queued entries 
} 


LAPI void log_output(log_level, const char* message, ...)
{
  const char *level_strings[6] = {"[FATAL]:"};
}
