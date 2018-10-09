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
	// BCI freq. todo: move to BCIstate
	// consumer = 
	double freq;
	// dim1 = time ; dim2 = freq ; STFT data
	// consumer = 
	std::vector<std::vector<double>> finalData;
	// dim1 = channel ; dim2 = freq ; for FFT Plot
	// consumer = 
	std::vector<std::vector<double>> filteredFreqs;
	// dim = time
	// consumer = 
	std::vector<double> controlMW;
	// dim = time
	// consumer = 
	std::vector<double> controlBT;
	// dim1 = index
	// consumer = 
	std::vector<double> times;

private:
	ControllerState() = default;
	~ControllerState() = default;
};