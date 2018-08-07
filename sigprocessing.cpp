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


std::vector<double> bci::DFT(std::vector<double>& data)
{
	// the dft size, N, is the same as the number of time points
	// we also use N freq points
	size_t N = data.size();
	std::vector<double> y_odd(N,0);
	std::vector<double> y_even(N,0);

	for (size_t i = 0; i < N; ++i)
	{
		// DFT for single freq slice
		auto wavelet = generateSinusoid(i/double(N),N);

		for (size_t j = 0; j < N; ++j)
		{
			y_odd[i] += wavelet.first[j] * data[j];
			y_even[i] += wavelet.second[j] * data[j];
		}
		y_odd[i] /= double(N);
		y_even[i] /= double(N);
		// put mag in y_odd
		y_odd[i] = std::sqrt(y_odd[i] * y_odd[i] + y_even[i] * y_even[i]);
	}

	return std::move(y_odd);
}



/*
bci::BCI_Packet& bci::spatial_filter::CAR(bci::BCI_Packet& data)
{

	double sum = 0;
	for (int i = 0; i < bci::BCI_Channels::END_OF_DATA; ++i)
	{
		sum += data[i];
	}
	sum /= bci::BCI_Channels::END_OF_DATA;

	for (int i = 0; i < bci::BCI_Channels::END_OF_DATA; ++i)
	{
		data[i] -= sum;
	}

	return data;
}

bci::BCI_Packet& bci::spatial_filter::Laplacian(bci::BCI_Packet & data)
{
	// TODO
	return data;
}

bci::BCI_Packet& bci::spatial_filter::ICA(bci::BCI_Packet & data)
{
	// no plans to complete this year
	return data;
}

bci::BCI_Packet& bci::spatial_filter::biophysical(bci::BCI_Packet & data)
{
	// no plans to complete this year
	return data;
}

bci::BCI_Packet& bci::spatial_filter::CSP(bci::BCI_Packet & data)
{
	// no plans to complete this year 
	return data;
}
*/