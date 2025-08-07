#ifndef TIME_H
#define TIME_H

#include <Windows.h>

struct Time
{
  double SecondsPerCount;
  double DeltaTime;
  
  __int64 BaseTime;
  __int64 PausedTime;
  __int64 StopTime;
  __int64 PrevTime;
  __int64 CurrTime;

  bool Stopped; 
};

void InitializeTime();

float GameTime(Time&) const;
float DeltaTime(Time&) const;

void Reset(Time&);
void Start();
void Stop();
void Tick();

#endif
