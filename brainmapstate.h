#pragma once
#include "resources.h"
#include "error.h"

struct SpfilProperties
{
	std::vector<double> spatial1Params;
	std::vector<double> spatial2Params;
};

struct BrainMapState
{
	std::mutex mtx_data;

	static BrainMapState state;
	SpfilProperties properties;

private:
	BrainMapState() = default;
	~BrainMapState() = default;
};