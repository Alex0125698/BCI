/*****************************************************************************************
*    File: BCI_Interface.h                                                               *
*                                                                                        *
*    DetailedException class - Used for handling errors and recording where the error    *
*    occurred. Should only catch problems which happen infrequently, otherwise it will   *
*    cause large performance issues                                                      *
*                                                                                        *
*    Author: A.S. Woodcock                                                               *
*****************************************************************************************/

#pragma once

#include "resources.h"

namespace bci {
/*
enum BCI_Electrode_Names
{
	CMS, DRL, FP1, AF3,
	F7, F3, FC5, T7,
	P7, O1, O2, P8,
	T8, FC6, F4, F8,
	AF4, FP2,
	END_OF_DATA
};*/

const std::string BCI_Channel_names[] =
{
	"AF3", "F7", "F3", "FC5",
	"T7", "P7", "O1", "O2",
	"P8", "T8", "FC6", "F4",
	"F8", "AF4"
};

enum BCI_Channels
{
	AF3, F7, F3, FC5,
	T7,  P7, O1, O2, 
	P8,  T8, FC6, F4,
	F8,  AF4,
	END_OF_DATA
};


struct BCI_Packet
{
	std::array<double, BCI_Channels::END_OF_DATA> data;
	inline double& operator[](int index) { return data[index]; };

	friend std::ostream& operator<< (std::ostream& out, const BCI_Packet& data)
	{
		out << "BCI channel data\n{\n";
		for (int i = 0; i < BCI_Channels::END_OF_DATA; ++i)
			out << "  " << BCI_Channel_names[i] << " = " << data.data[i] << '\n';
		out << '}';

		return out;
	}
};

class BCI_Interface : public Uncopyable
{
public:
	virtual ~BCI_Interface() = default;
	virtual void connect() = 0;
	virtual BCI_Packet& getData() = 0;

protected:
	BCI_Packet m_data;
};

}