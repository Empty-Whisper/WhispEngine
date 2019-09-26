#ifndef __j1PERFTIMER_H__
#define __j1PERFTIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	Uint64 ReadTicks() const;
	float ReadSec() const;
private:
	Uint64	started_at;
	static Uint64 frequency;
};

#endif //__j1PERFTIMER_H__