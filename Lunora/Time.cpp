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

float Time::TotalTime(Time& time) const
{
  if(time.Stopped)
    {
      return(float)(((time.StopTime - time.PausedTime) - time.BaseTime) * time.SecondsPerCount);
    }

  else
    {
      return (float)(((time.CurrTime - time.PausedTime) - time.BaseTime) * time.SecondsPerCount);
    }
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

float DeltaTime(Time& time) const
{
  return (float)time.DeltaTime; 
}

void Stop(Time& time)
{
  if (!time.Stopped)
    {
      __int64 currTime;
      QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

      time.StopTime = currTime;
      time.Stopped = true; 
    }
}

void Start(Time& time)
{
  __int64 startTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

  if (time.Stopped)
    {
      time.PausedTime += (startTime - time.StopTime);

      time.PrevTime = startTime;

      time.StopTime = 0;
      time.Stopped = false;  
    }
}
