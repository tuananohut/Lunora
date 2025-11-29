#include "test.h"

#include <iostream>

extern "C" LAPI void print_int(i32 i)
{
  std::cout << "The number is: " << i << std::endl;  
}
