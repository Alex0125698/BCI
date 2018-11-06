#pragma once
#include "resources.h"

namespace algs
{
	typedef std::vector<double> vec;
	typedef std::vector<std::vector<double>> vec2D;
	const double PI = 3.14159265358979323846264338327950288;
	const double TWO_PI = 2.0 * PI;

	template<typename T>
	inline T map(T val, T val_min, T val_max, T min, T max)
	{
		return (max - min)*(val - val_min) / (val_max - val_min) + min;
	}

	// {dim1 = channel ; dim2 = time} , {dim = channel} , {dim = time}
	void spatialFilter(const vec2D& data_in, vec& weights, vec& data_out);

	// removes the DC component / average
	// dim = time
	void removeDC(vec& data_io);

	// applies a gaussianwindow on some data (in place)
	// mag is 10% at either end and 100% in middle
	// dim = time
	void applyGaussianWindow(vec& data_io);

	// Computes the DFT of a complex vector (in place) using the FFT algorithm
	// dim = time/freq, dim = time/freq
	void FFTransfrom(vec& real_io, vec& imag_io);

	// computes a naive DTF (very slow)
	// dim = time/freq, dim = time/freq
	void DFTransfrom(vec& real_io, vec& imag_io);

	// Computes the inverse DFT of a complex vector (in place) using the FFT algorithm
	// Does not perform scaling !!!
	// dim = time/freq, dim = time/freq
	inline void FFTinverse(vec& real_io, vec& imag_io)
	{
		FFTransfrom(imag_io, real_io);
	}

	// Computes the circular convolution of the given real vectors.
	void convolve(const vec& v1_in, const vec& v2_in, vec& out);

	// Computes the circular convolution of the given complex vectors
	void convolve(const vec& xreal, const vec& ximag, const vec& yreal, const vec& yimag, vec& outreal, vec& outimag);
}