#pragma once

#include "Timer.h"

class GameTime
{
	friend class Application;
public:
	GameTime();
	~GameTime();

	void Start();
	void Pause();
	void Reanude();
	void Stop();

	float ReadSec();

	void SetDeltaTime(float delta);
	float GetDeltaTime();

private:
	Timer game_time;
	float dt = 0.f;
};

extern GameTime* Time;