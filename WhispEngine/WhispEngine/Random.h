#pragma once

#include <random>
#include "PCG/pcg_random.hpp"

class Random
{
public:
	Random();
	~Random();

	static float		Randomf(const float& min, const float& max);
	static int			Randomi(const int& min, const int& max);
	uint64_t			RandomGUID();

private:
	pcg_extras::seed_seq_from<std::random_device> seed_source;
	static pcg32 rng;

	std::uniform_real_distribution<double> guid;
};

