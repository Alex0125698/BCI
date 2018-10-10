#include "resources.h"
#include "sigprocessing.h"
#include "bciinterface.h"

const static double TWO_PI = 2 * 3.1415926535897932;

std::pair<std::vector<double>,std::vector<double>> generateSinusoid(double fd, size_t length)
{
	std::vector<double> wav_odd(length);
	std::vector<double> wav_even(length);

	for (size_t i = 0; i<length; ++i)
	{
		double s = i/double(length-1) - 0.5; // param. mapped -0.5 to +0.5
		double wnd = exp(-(s*s)*14.0);       // gaussian window

		wav_odd[i] = std::sin(TWO_PI*fd*i) * wnd;
		wav_even[i] = std::cos(TWO_PI*fd*i) * wnd;
	}

	// odd,even
	return std::pair<std::vector<double>,std::vector<double>>(std::move(wav_odd), std::move(wav_even));
}

std::vector<double> y_odd;
std::vector<double> y_even;

void DFT(std::vector<double>& data_in, std::vector<double>& data_out)
{
	if (data_out.size() != data_in.size()) data_out.resize(data_in.size());
	// the dft size, N, is the same as the number of time points
	// we also use N freq points
	size_t N = data_in.size();

	y_odd.resize(N);
	y_even.resize(N);

	for (size_t i = 0; i < N; ++i)
	{
		// DFT for single freq slice
		double fd = i / double(N);
		auto wavelet = generateSinusoid(fd, N);

		y_odd[i] = 0;
		y_even[i] = 0;

		for (size_t j = 0; j < N; ++j)
		{
			y_odd[i] += wavelet.first[j] * data_in[j];
			y_even[i] += wavelet.second[j] * data_in[j];
		}

		y_odd[i] /= double(N);
		y_even[i] /= double(N);

		data_out[i] = std::sqrt(y_odd[i]*y_odd[i] + y_even[i]*y_even[i]);
	}
}

using std::size_t;
using std::vector;

// Private function prototypes
static size_t reverseBits(size_t x, int n);

void Fft::transform(vector<double> &real, vector<double> &imag) {
	size_t n = real.size();
	if (n != imag.size())
		throw std::invalid_argument("Mismatched lengths");
	if (n == 0)
		return;
	else if ((n & (n - 1)) == 0)  // Is power of 2
		transformRadix2(real, imag);
	else  // More complicated algorithm for arbitrary sizes
		transformBluestein(real, imag);
}


void Fft::inverseTransform(vector<double> &real, vector<double> &imag) {
	transform(imag, real);
}


void Fft::transformRadix2(vector<double> &real, vector<double> &imag) {
	// Length variables
	size_t n = real.size();
	if (n != imag.size())
		throw std::invalid_argument("Mismatched lengths");
	int levels = 0;  // Compute levels = floor(log2(n))
	for (size_t temp = n; temp > 1U; temp >>= 1)
		levels++;
	if (static_cast<size_t>(1U) << levels != n)
		throw std::domain_error("Length is not a power of 2");

	// Trignometric tables
	vector<double> cosTable(n / 2);
	vector<double> sinTable(n / 2);
	for (size_t i = 0; i < n / 2; i++) {
		cosTable[i] = std::cos(2 * M_PI * i / n);
		sinTable[i] = std::sin(2 * M_PI * i / n);
	}

	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++) {
		size_t j = reverseBits(i, levels);
		if (j > i) {
			std::swap(real[i], real[j]);
			std::swap(imag[i], imag[j]);
		}
	}

	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2) {
		size_t halfsize = size / 2;
		size_t tablestep = n / size;
		for (size_t i = 0; i < n; i += size) {
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
				size_t l = j + halfsize;
				double tpre = real[l] * cosTable[k] + imag[l] * sinTable[k];
				double tpim = -real[l] * sinTable[k] + imag[l] * cosTable[k];
				real[l] = real[j] - tpre;
				imag[l] = imag[j] - tpim;
				real[j] += tpre;
				imag[j] += tpim;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}


void Fft::transformBluestein(vector<double> &real, vector<double> &imag) {
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	size_t n = real.size();
	if (n != imag.size())
		throw std::invalid_argument("Mismatched lengths");
	size_t m = 1;
	while (m / 2 <= n) {
		if (m > SIZE_MAX / 2)
			throw std::length_error("Vector too large");
		m *= 2;
	}

	// Trignometric tables
	vector<double> cosTable(n), sinTable(n);
	for (size_t i = 0; i < n; i++) {
		unsigned long long temp = static_cast<unsigned long long>(i) * i;
		temp %= static_cast<unsigned long long>(n) * 2;
		double angle = M_PI * temp / n;
		// Less accurate alternative if long long is unavailable: double angle = M_PI * i * i / n;
		cosTable[i] = std::cos(angle);
		sinTable[i] = std::sin(angle);
	}

	// Temporary vectors and preprocessing
	vector<double> areal(m), aimag(m);
	for (size_t i = 0; i < n; i++) {
		areal[i] = real[i] * cosTable[i] + imag[i] * sinTable[i];
		aimag[i] = -real[i] * sinTable[i] + imag[i] * cosTable[i];
	}
	vector<double> breal(m), bimag(m);
	breal[0] = cosTable[0];
	bimag[0] = sinTable[0];
	for (size_t i = 1; i < n; i++) {
		breal[i] = breal[m - i] = cosTable[i];
		bimag[i] = bimag[m - i] = sinTable[i];
	}

	// Convolution
	vector<double> creal(m), cimag(m);
	convolve(areal, aimag, breal, bimag, creal, cimag);

	// Postprocessing
	for (size_t i = 0; i < n; i++) {
		real[i] = creal[i] * cosTable[i] + cimag[i] * sinTable[i];
		imag[i] = -creal[i] * sinTable[i] + cimag[i] * cosTable[i];
	}
}


void Fft::convolve(const vector<double> &x, const vector<double> &y, vector<double> &out) {
	size_t n = x.size();
	if (n != y.size() || n != out.size())
		throw std::invalid_argument("Mismatched lengths");
	vector<double> outimag(n);
	convolve(x, vector<double>(n), y, vector<double>(n), out, outimag);
}


void Fft::convolve(
	const vector<double> &xreal, const vector<double> &ximag,
	const vector<double> &yreal, const vector<double> &yimag,
	vector<double> &outreal, vector<double> &outimag) {

	size_t n = xreal.size();
	if (n != ximag.size() || n != yreal.size() || n != yimag.size()
		|| n != outreal.size() || n != outimag.size())
		throw std::invalid_argument("Mismatched lengths");

	vector<double> xr = xreal;
	vector<double> xi = ximag;
	vector<double> yr = yreal;
	vector<double> yi = yimag;
	transform(xr, xi);
	transform(yr, yi);

	for (size_t i = 0; i < n; i++) {
		double temp = xr[i] * yr[i] - xi[i] * yi[i];
		xi[i] = xi[i] * yr[i] + xr[i] * yi[i];
		xr[i] = temp;
	}
	inverseTransform(xr, xi);

	for (size_t i = 0; i < n; i++) {  // Scaling (because this FFT implementation omits it)
		outreal[i] = xr[i] / n;
		outimag[i] = xi[i] / n;
	}
}


static size_t reverseBits(size_t x, int n) {
	size_t result = 0;
	for (int i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1U);
	return result;
}


// dim1 = ch ;  dim2 = time , dim = ch , dim = time
void spatialFilter(std::vector<std::vector<double>>& data_in, std::vector<double>& weights, std::vector<double>& data_out)
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

// apply gaussianwindow on data and store result back in data
// at either end the data is reduced to 10%, in the muddle it is the same
void applyGaussianWindow(std::vector<double>& data)
{
	const size_t N = data.size();
	const double HN = (double)(N-1) / 2.0;

	for (size_t i = 0; i < N; ++i)
	{
		double t = (double)i/HN - 1.0;
		data[i] *= std::exp(-2.3*t*t);
	}
}