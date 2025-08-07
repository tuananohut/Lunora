#include "Time.h"

void InitializeTime(Time& time)
{
  time.SecondsPerCount = 0.0;
  time.DeltaTime = -1.0;
  time.BaseTime = 0;
  time.PausedTime = 0;
  time.PrevTime = 0;
  time.CurrTime = 0;
  time.Stopped = false;

  __int64 countsPerSecond;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  time.SecondsPerCount = 1.0 / (double)countsPerSec; 
}

