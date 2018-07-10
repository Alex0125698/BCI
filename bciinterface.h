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

#include <utility>
#include <array>
#include <vector>
#include <ostream>

namespace bci {

typedef std::pair<double, double> vec2;

struct Channel
{
	double value{ 0 };
	vec2 pos{ 0,0 };
	// parameter from 0 to 1 - mapped from left ear to right ear
	const double posX() { return pos.first; }
	// parameter from 0 to 1 - mapped from back of head to temple
	const double posY() { return pos.second; }

	/*
	enum Names
	{
		AF3, F7, F3, FC5,
		T7, P7, O1, O2,
		P8, T8, FC6, F4,
		F8, AF4, SIZE
	};

	static const std::array<const Channel, Names::SIZE> Channels;
	*/
};

class Interface
{
public:
	std::vector<Channel> channels;
	// acquire resources + connect to device
	virtual void connect() = 0;
	// free resources + disconnect
	virtual void disconnect() = 0;
	// fill the channels vector with the latest data
	virtual void update() = 0;
	// need to re-implement this
	virtual ~Interface() = default;
	const bool connected() const { return m_connected; }
	// output channel data as formatted columns
	friend std::ostream& operator<< (std::ostream& out, const Interface& bci)
	{
		size_t num_cols = 5;
		if (bci.channels.size() % 4 == (size_t)0) num_cols = 4;
		if (bci.channels.size() % 6 == (size_t)0) num_cols = 6;

		for (size_t i = 1; i < bci.channels.size() + 1; ++i)
		{
			out << bci.channels[i - 1].value;
			if (i % num_cols == 0) out << '\n';
			else out << ' ';
		}

		return out;
	}

protected:
	bool m_connected{ false };
};

}

/*
enum BCI_Electrode_Names
{
CMS, DRL, FP1, AF3,
F7, F3, FC5, T7,
P7, O1, O2, P8,
T8, FC6, F4, F8,
AF4, FP2,
END_OF_DATA
};

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
};*/