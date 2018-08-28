/**
* Controller class - This class connects to the bci, applys the filters and updates 
* the state so that the data can be displayed. In sends runState and saveState
* signals back to the mainwindow so that it can be updated properly.
* 
* Error handling - will send stop signal to mainwindow and disconnect the BCI.
*
* Author:  A.S. Woodcock
* Project: BCI Controller for Biomedical Applications
*/

#pragma once

#include "resources.h"

namespace bci {
	class Interface;
}

class Controller : public QObject
{
	Q_OBJECT

public:
	Controller();
	
signals:
	void sigRunStateChanged(bool running);
	void sigSaveStateChanged(bool saving);

public slots:
	void slotStart();
	void slotStop();;
	void slotDataReady();

protected:
	Timer m_run_timer;
	Timer m_io_time;
	Timer m_calc_time;
	std::unique_ptr<bci::Interface> m_bci;
	std::atomic<bool> m_running{ false };
	boost::circular_buffer<std::vector<double>> m_data_buff;
	std::vector<double> m_channel_freq_mag;
};

