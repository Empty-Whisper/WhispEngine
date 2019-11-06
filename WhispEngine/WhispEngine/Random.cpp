#include "Random.h"

Random::Random()
{
	rng = pcg32(seed_source);
	guid = std::uniform_real_distribution<double>(0, UINT64_MAX);
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
	std::uniform_real_distribution<double> rand(min, max);
	return rand(rng);
}

uint64_t Random::RandomGUID()
{
	return guid(rng);
}
