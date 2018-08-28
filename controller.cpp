#include "controller.h"
#include "csvio.h"
#include "bciinterface.h"
#include "cytoninterface.h"
#include "offline.h"
#include "sigprocessing.h"
#include "state.h"
#include <QTimer>


Controller::Controller()
	// : m_bci{ new bci::EmotivInterface }
	// : m_bci{ new bci::Offline("right_smr_1.csv") }
	: m_bci{ new bci::CytonInterface }
{
	m_channel_freq_mag.resize(16, 0);
	m_data_buff.resize(128);
	for (auto& ch : m_data_buff)
		ch.resize(16,0.0);

	connect(m_bci.get(), &bci::CytonInterface::sigDataReady, this, &Controller::slotDataReady, Qt::QueuedConnection);
	connect(m_bci.get(), &bci::CytonInterface::sigCallStopHelper, this, &Controller::slotStop, Qt::QueuedConnection);
	connect(m_bci.get(), &bci::CytonInterface::sigError, [this](QString msg, QString file, int line) { slotStop(); DebugOutput::println(msg, file, line, MSG_TYPE::DEXCEP); });
}

void Controller::slotStart()
{
	try
	{
		//(bci::State::program.getVars())[(size_t)Vars::TIME]->si
		m_run_timer.restart();
		DEBUG_PRINTLN("clear");
		m_bci->start();
		m_running = true;
		emit sigRunStateChanged(true);
		DEBUG_PRINTLN("Data aquisition started");		
	}
	catch (DetailedException e)
	{
		DebugOutput::println(e.getMessage(), e.getFile(), e.getLine(), MSG_TYPE::DEXCEP);
	}
	catch (std::exception e)
	{
		DebugOutput::println(QString("STD Exception: ") + e.what(), "", 0, MSG_TYPE::STDERROR);
	}
}

void Controller::slotStop()
{ 
	if (m_running)
	{
		m_running = false;
		emit sigRunStateChanged(false);
		DEBUG_PRINTLN("Data aquisition stopped");
	}
	if (m_bci->connected())
		m_bci->stop();
}

void Controller::slotDataReady()
{
	m_calc_time.restart();

	m_data_buff.push_back(std::vector(16, 0.0));
	m_bci->getData(m_data_buff[m_data_buff.size()-1]);

	bci::State::program.loadVars();

	if (bci::State::program[Vars::TIME] <= 0.00001) m_run_timer.restart();

	bci::State::program[Vars::RUN_TIME] =  m_run_timer.getDuration();
	//std::this_thread::sleep_for(std::chrono::milliseconds(2));
	bci::State::program[Vars::IO_FREQ] = 0.9*bci::State::program[Vars::IO_FREQ] + 0.1 / m_io_time.getDuration();
	m_io_time.restart();
	bci::State::program[Vars::TIME] += 1.0 / 125.0; // Assume 125 Hz

	auto start = (size_t)Vars::CH1;
	auto stop = (size_t)Vars::CH16;
	for (auto i = start; i <= stop; ++i)
	{
		bci::State::program[i] = m_data_buff[m_data_buff.size()-1][i-start];
	}

	bci::FreqTransform(m_data_buff, 50.0/125.0, m_channel_freq_mag);
	
	start = (size_t)Vars::MU1;
	stop = (size_t)Vars::MU16;
	for (auto i = start; i <= stop; ++i)
	{
		bci::State::program[i] = m_channel_freq_mag[i - start];
	}

	bci::State::program[Vars::USED_TIME] = m_calc_time.getDuration() / (1.0 / 125.0);
	bci::State::program.updateVars();

	std::vector<double> chData(m_data_buff.size());
	for (int i = 0; i < chData.size(); ++i)
		chData[i] = m_data_buff[i][0];

	{
		std::lock_guard<std::mutex> guard(DTFT_Shared_Data.mtx);
		bci::DFT(chData, DTFT_Shared_Data.data);
		DTFT_Shared_Data.timePoints = 100;
		DTFT_Shared_Data.freqPoints = DTFT_Shared_Data.data.size();
	}
}
