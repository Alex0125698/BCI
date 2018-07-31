#include "offline.h"
#include "csvio.h"

bci::Offline::Offline(const std::string & fileName)
	: m_file_name{ fileName }, m_in{ new CSVreader }
{
}

void bci::Offline::start()
{
	disconnect();
	m_in->open(m_file_name);

	size_t num_cols = 0;
	// get channel names + number of channels
	while (true)
	{
		// read the next element
		const auto str = m_in->read();
		if (str == "\n")
			break;
		if (str == "")
			throw DETAILEDEXCEPTION("premature end of data input file");

		// TODO: check positions

		++num_cols;
	}
	m_channel.resize(num_cols);
	// TODO: fill in positions
	m_connected = true;
}

void bci::Offline::stop()
{
	m_connected = false;
	m_in->close();
}
/*
void bci::Offline::update()
{
	// get next row of data
	for (size_t i = 0; i < channels.size(); ++i)
	{
		// read the next element
		const auto str = m_in->read();
		if (str == "\n")
		{
			throw DETAILEDEXCEPTION("row found without required column count");
		}
		if (str == "")
			throw DETAILEDEXCEPTION("data input file ended before row complete");
		else
			channels[i].value = std::stod(str);
	}

	// get to end of line
	while (true)
	{
		// read the next element
		const auto str = m_in->read();
		if (str == "\n")
			break;
		else if (str == "")
			disconnect();
	}
}
*/