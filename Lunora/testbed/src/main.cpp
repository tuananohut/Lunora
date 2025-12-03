#include <core/logger.h>

int main(void)
{
  LFATAL("A test message: %f", 3.14f); 
  LERROR("A test message: %f", 3.14f); 
  LWARN("A test message: %f", 3.14f); 
  LINFO("A test message: %f", 3.14f); 
  LDEBUG("A test message: %f", 3.14f); 
  LTRACE("A test message: %f", 3.14f); 

  return 0; 
}


