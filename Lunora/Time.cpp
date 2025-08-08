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

void Tick(Time& time)
{
  if (time.Stopped)
    {
      time.DeltaTime = 0.0;
    }

  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  time.CurrTime = currTime;
  
  time.DeltaTime = (time.CurrTime - time.PrevTime) * time.SecondsPerCount;
  
  time.PrevTime = time.CurrTime;

  if (time.DeltaTime < 0.0)
    {
      time.DeltaTime = 0.0; 
    }
}

float DeltaTime() const
{
  return (float)time.DeltaTime; 
}
