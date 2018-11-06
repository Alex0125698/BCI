/**
* This structure stores shared static data produced by Controller
*
* Consumers:
* > MainWindow
* > BallTest
* > BrainMap
* > PixPlotter
*
*/

#pragma once

#include "resources.h"
#include "error.h"

struct ControllerState
{
public:
	// lock whenever any data needs to be accessed
	std::mutex mtx_data;
	static ControllerState state;

public:
	bool resetMainwindow{ false };
	bool resetSTFT{ false };
	double freq{ 1 }; // constant

	double timeLerp{ 1 }; // seconds
	double time_plot_time{ 0 }; // seconds
	double freq_plot_time{ 0 }; // seconds
	double tr_plot_time{ 0 }; // seconds
	double stft_plot_time{ 0 }; // seconds

public:
	// dim1 = ch ; dim2 = time
	// consumer: Time Plot
	std::vector<std::vector<double>> rawTD;
	// dim1 = channel ; dim2 = freq
	// consumer: Freq Plot
	std::vector<std::vector<double>> rawFD; // smoothed
	// dim1 = buff ; dim2 = left/right ; dim3 = freq
	// consumer: STFT Plot
	std::vector<std::vector<std::vector<double>>> imgOutputFD;
	// dim1 = buff ; dim2 = left/right
	// consumer: Ball Test
	std::vector<std::vector<double>> ball_spfillTR;
	// dim1 = buff ; dim2 = n (freq spread) ; dim3 = left/right
	// consumer: Tr Plot
	std::vector<std::vector<std::vector<double>>> graph_spfillTR;
	// dim = channel
	// consumer: BrainMap
	std::vector<double> chCAR_TR; // smoothed

private:
	ControllerState() = default;
	~ControllerState() = default;
};