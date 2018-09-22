#pragma once

#include "resources.h"

namespace bci
{
	void FreqTransform(boost::circular_buffer<std::vector<double>>& in_data, double fd, std::vector<double>& out_data);
	void DFT(std::vector<double>& data_in, std::vector<double>& data_out);


	namespace spatial_filter
	{
		/*
		// Common Average Reference - removes common signal
		BCI_Packet& CAR(BCI_Packet& data);
		// Subtracts neighbouring electrodes
		BCI_Packet& Laplacian(BCI_Packet& data);
		// decompose brain signals into statistically components
		BCI_Packet& ICA(BCI_Packet& data);
		// uses a realistic biophysical head model to deblur signal
		BCI_Packet& biophysical(BCI_Packet& data);
		// ??? 
		BCI_Packet& CSP(BCI_Packet& data);
		*/
	}
}

template<typename T>
T map(T val, T val_min, T val_max, T min, T max)
{
	return (max - min)*(val - val_min) / (val_max - val_min) + min;
}

#include <vector>
#define M_PI 3.14159265358979323846264338327950288


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