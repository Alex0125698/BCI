#pragma once
#include "resources.h"
#include "error.h"

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
	bool enabled{ true };
};

struct TrProperties
{
	int centreFreq{ 10 };
	int spanFreq{ 3 };
	int freqSpreading{ 2 };
	bool flip{ true };

	double sensitivity{ 0.5 };
	double smoothing{ 0.5 };
	double gain{ 0.5 };
	double offset{ 0.5 };

	double offCorrSpeed{ 0.5 };
	double offCorrLimit{ 0.5 };
	double magCorrSpeed{ 0.5 };
	double magCorrLimit{ 0.5 };
};

struct MainWindowState
{
	static MainWindowState state;
	DTFTproperties dtft;
	TrProperties translation;
	std::mutex mtx_data;

private:
	MainWindowState() = default;
	~MainWindowState() = default;
};