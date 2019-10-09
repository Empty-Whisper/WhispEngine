#include "Random.h"



Random::Random()
{
	rng = pcg32(seed_source);
}


Random::~Random()
{
}

float Random::Randomf(const float & min, const float & max)
{
	std::uniform_real_distribution<float> rand(min, max);
	
	return rand(rng);
}

int Random::Randomi(const int & min, const int & max)
{
	std::uniform_real_distribution<int> rand(min, max);
	return rand(rng);
}
