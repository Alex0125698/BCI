/**
 * Use this class to read BCI data from a CSV file
 *
 *
 * Author:  A.S. Woodcock
 * Project: BCI Controller for Biomedical Applications
 *
 */

#pragma once

#include "resources.h"
#include "bciinterface.h"

// forward declarations
class QTimer;

namespace bci {

class Offline : public bci::Interface
{
	Q_OBJECT

public:
	Offline(const std::string&& fileName, uint32_t freq_Hz);
	virtual ~Offline()
	{ 
		stop(); 
	}
	// load csv file and set the sample rate in Hz
	void openFile();
	// the number of channels will depend on the file
	virtual const size_t numChannels() override
	{
		return m_num_channels;
	};
	virtual const double freq() override { return m_freq_hz; }
	virtual void start_helper() override;
	virtual void stop_helper() override;
	virtual void init() override;

private slots:
	void slotGetSample();

private:
	// created in constructor and used by BCI thread
	std::string m_fileName;
	uint32_t m_freq_hz;

private:
	// private data only for BCI thread
	// whenever we open a file; all data is put in here
	// TODO: what is data is too large, maybe only buffer some of it
	std::vector<std::vector<double>> m_data;
	QTimer* m_timer{ nullptr };
	size_t m_num_channels{ 0 };
	size_t m_data_counter{ 0 };
};

}