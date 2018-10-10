#pragma once

#include "resources.h"
#include "error.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif // M_PI

template<typename T>
T map(T val, T val_min, T val_max, T min, T max)
{
	return (max - min)*(val - val_min) / (val_max - val_min) + min;
}

// dim1 = ch ;  dim2 = time , dim = ch , dim = time
void spatialFilter(std::vector<std::vector<double>>& data_in, std::vector<double>& weights, std::vector<double>& data_out);

namespace Fft {

	/*
	 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
	 * The vector can have any length. This is a wrapper function.
	 */
	void transform(std::vector<double> &real, std::vector<double> &imag);


	/*
	 * Computes the inverse discrete Fourier transform (IDFT) of the given complex vector, storing the result back into the vector.
	 * The vector can have any length. This is a wrapper function. This transform does not perform scaling, so the inverse is not a true inverse.
	 */
	void inverseTransform(std::vector<double> &real, std::vector<double> &imag);


	/*
	 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
	 * The vector's length must be a power of 2. Uses the Cooley-Tukey decimation-in-time radix-2 algorithm.
	 */
	void transformRadix2(std::vector<double> &real, std::vector<double> &imag);


	/*
	 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
	 * The vector can have any length. This requires the convolution function, which in turn requires the radix-2 FFT function.
	 * Uses Bluestein's chirp z-transform algorithm.
	 */
	void transformBluestein(std::vector<double> &real, std::vector<double> &imag);


	/*
	 * Computes the circular convolution of the given real vectors. Each vector's length must be the same.
	 */
	void convolve(const std::vector<double> &x, const std::vector<double> &y, std::vector<double> &out);


	/*
	 * Computes the circular convolution of the given complex vectors. Each vector's length must be the same.
	 */
	void convolve(
		const std::vector<double> &xreal, const std::vector<double> &ximag,
		const std::vector<double> &yreal, const std::vector<double> &yimag,
		std::vector<double> &outreal, std::vector<double> &outimag);

}

void applyGaussianWindow(std::vector<double>& data);