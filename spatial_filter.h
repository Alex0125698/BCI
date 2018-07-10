#pragma once

#include <vector>
#include <utility>

namespace bci
{
	std::pair<std::vector<double>,std::vector<double>> DFT(std::vector<double>& data);


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


