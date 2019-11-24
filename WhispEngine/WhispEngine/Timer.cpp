// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

void Timer::Reanude()
{
	running = true;
	started_at += SDL_GetTicks() - stopped_at;
}

void Timer::Clear()
{
	running = false;
	started_at = 0u;
	stopped_at = 0u;
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

// ---------------------------------------------
float Timer::ReadSec()
{
	if (running == true)
	{
		return (float)((float)SDL_GetTicks() - (float)started_at) / 1000.00000F;
	}
	else
	{
		return ((float)stopped_at - (float)started_at) / 1000.0000000F;
	}
}


