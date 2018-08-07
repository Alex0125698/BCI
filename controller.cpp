#include "controller.h"
#include "csvio.h"
#include "bciinterface.h"
#include "cytoninterface.h"
#include "offline.h"
#include "sigprocessing.h"
#include "state.h"
#include <QTimer>

template<typename T>
T map(T val, T val_min, T val_max, T min, T max)
{
	return (max-min)*(val-val_min)/(val_max-val_min) + min;
}

Controller::Controller()
	// : m_bci{ new bci::EmotivInterface }
	// : m_bci{ new bci::Offline("right_smr_1.csv") }
	: m_bci{ new bci::CytonInterface }
{
	m_data_buff.resize(1024);
	for (auto& ch : m_data_buff)
		ch.resize(16,0.0);

	connect(m_bci.get(), &bci::CytonInterface::sigDataReady, this, &Controller::slotDataReady, Qt::QueuedConnection);
	connect(m_bci.get(), &bci::CytonInterface::sigCallStopHelper, this, &Controller::slotStop, Qt::QueuedConnection);
}

void Controller::slotStart()
{
	try
	{
		(bci::State::program.getVars())[(size_t)Vars::TIME]->data() = 0;
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
	m_data_buff.push_back(std::vector(16, 0.0));
	m_bci->getData(m_data_buff[m_data_buff.size()-1]);

	auto start = (size_t)Vars::CH1;
	auto stop = (size_t)Vars::CH16;

	bci::State::program.loadVars();

	bci::State::program[Vars::RUN_TIME] = m_run_timer.getDuration();
	bci::State::program[Vars::TIME] += 1.0 / 125.0; // Assume 125 Hz

	for (auto i = start; i <= stop; ++i)
	{
		bci::State::program[i] = m_data_buff[m_data_buff.size()-1][i-start];
	}

	bci::State::program.updateVars();
}
