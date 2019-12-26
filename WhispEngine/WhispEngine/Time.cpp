#include "Time.h"

Timer GameTime::game_time;
float GameTime::dt;

GameTime::GameTime()
{
	game_time.Clear();
}


GameTime::~GameTime()
{
}

void GameTime::Start()
{
	game_time.Start();
}

void GameTime::Pause()
{
	game_time.Stop();
}

void GameTime::Reanude()
{
	game_time.Reanude();
}

void GameTime::Stop()
{
	game_time.Clear();
}

float GameTime::ReadSec()
{
	return game_time.ReadSec();
}

void GameTime::SetDeltaTime(float delta)
{
	dt = delta;
}

float GameTime::GetDeltaTime()
{
	return dt;
}
