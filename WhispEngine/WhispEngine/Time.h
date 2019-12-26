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

	static float ReadSec();

	void SetDeltaTime(float delta);
	static float GetDeltaTime();

private:
	static Timer game_time;
	static float dt;
};

extern GameTime* Time;