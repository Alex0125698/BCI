#include "resources.h"
#include "sigprocessing.h"

namespace algs
{
	const std::pair<vec, vec>& generateSinusoid(double fd, size_t length)
	{
		static std::pair<vec, vec> ret;
		ret.first.resize(length);
		ret.second.resize(length);

		for (size_t t = 0; t < length; ++t)
		{
			ret.first[t] = std::sin(TWO_PI*fd*t);
			ret.second[t] = std::cos(TWO_PI*fd*t);
		}
		return ret;
	}

	size_t reverseBits(size_t x, int n)
	{
		size_t result = 0;
		for (int i = 0; i < n; i++, x >>= 1)
			result = (result << 1) | (x & 1U);
		return result;
	}

	//Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
	//The vector's length must be a power of 2. Uses the Cooley-Tukey decimation-in-time radix-2 algorithm.
	void transformRadix2(vec& real, vec& imag)
	{
		sassert(real.size() == imag.size());
		const auto N = real.size();

		size_t levels = 0;  // Compute levels = floor(log2(n))
		for (size_t temp = N; temp > 1U; temp >>= 1) levels++;
		sassert(((size_t)1 << levels) == N);

		// Trignometric tables
		//const auto& wavelet = generateSinusoid(1.0/N,N/2);
		//const vec& cosTable = wavelet.first;
		//const vec& sinTable = wavelet.second;

		vec cosTable(N / 2);
		vec sinTable(N / 2);
		for (size_t i = 0; i < N / 2; i++)
		{
			cosTable[i] = std::cos(TWO_PI*i / N);
			sinTable[i] = std::sin(TWO_PI*i / N);
		}

		// Bit-reversed addressing permutation
		for (size_t i = 0; i < N; i++)
		{
			size_t j = reverseBits(i, levels);
			if (j > i)
			{
				std::swap(real[i], real[j]);
				std::swap(imag[i], imag[j]);
			}
		}

		// Cooley-Tukey decimation-in-time radix-2 FFT
		for (size_t size = 2; size <= N; size *= 2)
		{
			size_t halfsize = size / 2;
			size_t tablestep = N / size;

			for (size_t i = 0; i < N; i += size)
			{
				for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
				{
					size_t l = j + halfsize;
					double tpre = real[l] * cosTable[k] + imag[l] * sinTable[k];
					double tpim = -real[l] * sinTable[k] + imag[l] * cosTable[k];
					real[l] = real[j] - tpre;
					imag[l] = imag[j] - tpim;
					real[j] += tpre;
					imag[j] += tpim;
				}
			}
			// Prevent overflow in 'size *= 2'
			if (size == N) break;
		}
	}

	//Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
	//The vector can have any length. This requires the convolution function, which in turn requires the radix-2 FFT function.
	//Uses Bluestein's chirp z-transform algorithm.
	void transformBluestein(vec& real, vec& imag)
	{
		// Find a power-of-2 convolution length m such that m >= n * 2 + 1
		sassert(real.size() == imag.size());
		size_t n = real.size();

		size_t m = 1;
		while (m / 2 <= n)
		{
			m *= 2;
		}

		// Trignometric tables
		vec cosTable(n), sinTable(n);
		for (size_t i = 0; i < n; i++)
		{
			unsigned long long temp = static_cast<unsigned long long>(i) * i;
			temp %= static_cast<unsigned long long>(n) * 2;
			double angle = PI * temp / n;
			// Less accurate alternative if long long is unavailable: double angle = M_PI * i * i / n;
			cosTable[i] = std::cos(angle);
			sinTable[i] = std::sin(angle);
		}

		// Temporary vectors and preprocessing
		vec areal(m), aimag(m);
		for (size_t i = 0; i < n; i++) {
			areal[i] = real[i] * cosTable[i] + imag[i] * sinTable[i];
			aimag[i] = -real[i] * sinTable[i] + imag[i] * cosTable[i];
		}
		vec breal(m), bimag(m);
		breal[0] = cosTable[0];
		bimag[0] = sinTable[0];
		for (size_t i = 1; i < n; i++) {
			breal[i] = breal[m - i] = cosTable[i];
			bimag[i] = bimag[m - i] = sinTable[i];
		}

		// Convolution
		vec creal(m), cimag(m);
		convolve(areal, aimag, breal, bimag, creal, cimag);

		// Postprocessing
		for (size_t i = 0; i < n; i++)
		{
			real[i] = creal[i] * cosTable[i] + cimag[i] * sinTable[i];
			imag[i] = -creal[i] * sinTable[i] + cimag[i] * cosTable[i];
		}
	}

	// {dim1 = channel ; dim2 = time} , {dim = channel} , {dim = time}
	void spatialFilter(const vec2D& data_in, vec& weights, vec& data_out)
	{
		sassert(data_in.size() == weights.size());
		sassert(data_in.size() > 0);
		sassert(data_in[0].size() == data_out.size());

		double neg_sum = 0.0;
		double pos_sum = 0.0;
		for (auto& v : weights)
		{
			if (v < 0) neg_sum -= v;
			else if (v > 0) pos_sum += v;
		}

		// normalise so that the +ve cancels the -ve
		for (auto& v : weights)
		{
			if (v < 0) v /= neg_sum;
			else if (v > 0) v /= pos_sum;
		}

		std::fill(data_out.begin(), data_out.end(), 0.0);

		auto numTimePoints = data_in[0].size();
		auto numChannels = data_in.size();

		for (size_t t = 0; t < numTimePoints; ++t)
		{
			for (size_t ch = 0; ch < numChannels; ++ch)
			{
				data_out[t] += data_in[ch][t] * weights[ch];
			}
		}
	}

	// dim = time
	void removeDC(vec& data_io)
	{
		double dc = 0.0;
		for (const auto& d : data_io)
			dc += d;

		dc /= data_io.size();

		for (auto& d : data_io)
			d -= dc;
	}

	// dim = time
	void applyGaussianWindow(vec& data_io)
	{
		const size_t N = data_io.size();
		const double HN = (double)(N - 1) / 2.0;

		for (size_t i = 0; i < N; ++i)
		{
			double t = (double)i / HN - 1.0;
			data_io[i] *= std::exp(-2.3*t*t);
		}
	}

	// dim = time/freq, dim = time/freq
	void FFTransfrom(vec& real_io, vec& imag_io)
	{
		sassert(real_io.size() == imag_io.size());

		const auto n = real_io.size();
		if ((n & (n - 1)) == 0)  // Is power of 2
			transformRadix2(real_io, imag_io);
		else  // More complicated algorithm for arbitrary sizes
			transformBluestein(real_io, imag_io);
	}

	// Computes the DFT of a complex vector (in place) using the FFT algorithm
	// dim = time/freq, dim = time/freq
	void DFTransfrom(vec& real_io, vec& imag_io)
	{
		sassert(real_io.size() == imag_io.size());

		// store input vectors
		static vec real_in_s, imag_in_s;
		real_in_s = real_io;
		imag_in_s = imag_io;
		const auto& real_in = real_in_s;
		const auto& imag_in = imag_in_s;

		// the dft size, N, is the same as the number of time points & freq bins
		size_t N = real_io.size();

		for (size_t freq = 0; freq < N; ++freq)
		{
			// DFT for single freq slice
			double fd = freq / double(N);
			const auto& wavelet = generateSinusoid(fd, N);

			real_io[freq] = 0;
			imag_io[freq] = 0;

			for (size_t t = 0; t < N; ++t)
			{
				real_io[freq] += wavelet.first[t] * real_in[t] - wavelet.second[t] * imag_in[t];
				imag_io[freq] += wavelet.second[t] * real_in[t] + wavelet.first[t] * imag_in[t];
			}
		}
	}

	// Computes the circular convolution of the given real vectors.
	void convolve(const vec& v1_in, const vec& v2_in, vec& out)
	{
		sassert(v1_in.size() == v2_in.size());
		sassert(v1_in.size() == out.size());
		auto N = v1_in.size();

		vec outimag(N);
		convolve(v1_in, vec(N), v2_in, vec(N), out, outimag);
	}

	// Computes the circular convolution of the given complex vectors
	void convolve(const vec& xreal, const vec& ximag, const vec& yreal, const vec& yimag, vec& outreal, vec& outimag)
	{
		const auto N = xreal.size();
		sassert(N == ximag.size());
		sassert(N == yreal.size());
		sassert(N == yimag.size());
		sassert(N == outreal.size());
		sassert(N == outimag.size());

		vec xr = xreal;
		vec xi = ximag;
		vec yr = yreal;
		vec yi = yimag;
		FFTransfrom(xr, xi);
		FFTransfrom(yr, yi);

		for (size_t i = 0; i < N; i++)
		{
			double temp = xr[i] * yr[i] - xi[i] * yi[i];
			xi[i] = xi[i] * yr[i] + xr[i] * yi[i];
			xr[i] = temp;
		}
		FFTinverse(xr, xi);

		// Scaling (because this FFT implementation omits it)
		for (size_t i = 0; i < N; i++)
		{
			outreal[i] = xr[i] / N;
			outimag[i] = xi[i] / N;
		}
	}
}