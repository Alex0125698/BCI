#pragma once
#include "resources.h"
#include "error.h"

struct ControllerState
{
	static ControllerState state;
	double freq;
	std::vector<std::vector<double>> finalData;
	std::mutex mtx_data;

private:
	ControllerState() = default;
	~ControllerState() = default;
};