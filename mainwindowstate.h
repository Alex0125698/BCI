#pragma once
#include "resources.h"

enum DFTwindow
{
	RECTANGULAR,
	GAUSSIAN,
};

struct DTFTproperties
{
	int wndSize{ 128 };
	int wndOverlap{ 64 };
	int timeSpan{ 20 };

	double maxFreq{ 0.5 };
	double brightness{ 0.5 };
	double darkBoost{ 0.0 };
	double hardLimit{ 0.0 };

	double sharpenKernelSize{ 2.0 };
	double sharpenAmount{ 0 };
	double blurKernelSize{ 0.8 };
	double blurAmount{ 0 };

	DFTwindow wndType{ DFTwindow::GAUSSIAN };
	int channelSource{ 0 };
	bool enabled{ true };
};

struct MainWindowState
{
	static MainWindowState state;
	DTFTproperties dtft;
	std::mutex mtx_data;

private:
	MainWindowState() = default;
	~MainWindowState() = default;
};