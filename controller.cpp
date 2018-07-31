#include "controller.h"
#include "csvio.h"
#include "bciinterface.h"
#include "cytoninterface.h"
#include "offline.h"
#include "spatial_filter.h"
//#include "state.h"


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


	/*
	dft_data.resize(m_bci->channels.size());
	for (auto& ch : dft_data)
	{
	ch.resize(DFT_SIZE);
	}
	*/
	//size_t dft_counter = 0;

	// fill in data structure
	/*
	for (size_t i=0; i<dft_data.size(); ++i)
	{
	dft_data[i][dft_counter] = m_bci->channels[i].value;
	}
	++dft_counter;

	std::vector<double> mags;
	// if we have enough data, run DTF alg for all channels
	if (dft_counter >= DFT_SIZE)
	{
	dft_counter = 0;
	mags = std::move(bci::DFT(dft_data[0]));

	// the sample rate is 250 Hz, so this is the max freq
	double freq_res = 250.0 / DFT_SIZE;

	size_t index = (size_t)map(50.0, 0.0, 250.0, 0.0, (double)mags.size());
	//Input::Vars[Input::MU] = units::V.getRaw(1000000*mags[index], VAR[Input::MU].SIZE_IN_BYTES);

	index = (size_t)map(40.0, 0.0, 250.0, 0.0, (double)mags.size());
	//Input::Vars[Input::BETA] = units::V.getRaw(1000000 * mags[index], VAR[Input::BETA].SIZE_IN_BYTES);
	}
	*/
}
