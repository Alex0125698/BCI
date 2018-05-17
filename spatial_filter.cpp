#include "spatial_filter.h"

#include "BCI_Interface.h"

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
