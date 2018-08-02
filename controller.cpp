#include "controller.h"
#include "csvio.h"
#include "bciinterface.h"
#include "cytoninterface.h"
#include "offline.h"
#include "spatial_filter.h"
#include "state.h"


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
	connect(m_bci.get(), &bci::CytonInterface::sigDataReady, this, &Controller::slotDataReady, Qt::QueuedConnection);
	connect(m_bci.get(), &bci::CytonInterface::sigCallStopHelper, this, &Controller::slotStop, Qt::QueuedConnection);
	//connect()
}

void Controller::slotStart()
{
	try
	{
		(bci::State::program.getVars())[(size_t)Vars::TIME]->data() = 0;
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

std::vector<double> store(16);

void Controller::slotDataReady()
{
	m_bci->getData(store);

	

	auto start = (size_t)Vars::CH1;
	auto stop = (size_t)Vars::CH16;

	bci::State::program.loadVars();

	bci::State::program[Vars::TIME] += 1.0 / 125.0; // Assume 125 Hz

	for (auto i = start; i <= stop; ++i)
	{
		bci::State::program[i] = store[i];
	}

	bci::State::program.updateVars();
}
