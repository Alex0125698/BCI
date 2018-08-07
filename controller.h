#pragma once

#include "resources.h"

namespace bci {
	class Interface;
}

class Controller :public QObject
{
	Q_OBJECT

public:
	Controller();
	
signals:
	void sigViewUpdate();
	void sigGraphUpdate();
	void sigRunStateChanged(bool running);
	void sigSaveStateChanged(bool saving);

public slots:
	void slotStart();
	void slotStop();;
	void slotDataReady();

protected:
	Timer m_run_timer;
	Timer m_used_time;
	std::unique_ptr<bci::Interface> m_bci;
	std::atomic<bool> m_running{ false };
	boost::circular_buffer<std::vector<double>> m_data_buff;
};

