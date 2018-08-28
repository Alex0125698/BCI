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

