#include <thread>
#include "controller.h"
#include "csvio.h"
#include "bciinterface.h"
#include "emotiv.h"
#include "offline.h"
#include "spatial_filter.h"
#include "timer.h"
#include "error.h"



Controller::Controller()
	//: m_bci{ new bci::EmotivInterface }
	: m_bci{ new bci::Offline("right_smr_1.csv") }
{
}

void Controller::mainwindowIsReady()
{
	m_mainwindow_ready = true;
}

void Controller::slotRun()
{
	try
	{
		m_running = true;
		m_bci->connect();

		Timer view_refresh;

		DEBUG_PRINTLN("Data aquisition started");
		while (m_running)
		{
			if (!m_bci->connected()) break;

			m_bci->update();
			//auto&& tmp =  bci::spatial_filter::CAR(m_bci_hardware->getData());

			// update mainwindow every 200 ms if it is ready
			if ((view_refresh.getDuration() > 0.3))
			{
				m_mainwindow_ready = false;
				// store data statically for mainwindow

				// tell mainwindow to update itself
				emit requestViewUpdate();
				view_refresh.restart();
			}

			// prevent maxing out CPU
			std::this_thread::sleep_for(std::chrono::milliseconds(0));
		}
	}
	catch (DetailedException e)
	{
		DebugOutput::println("ERROR: " + e.getMessage(), e.getFile(), e.getLine(), MSG_TYPE::DEXCEP);
	}
	catch (std::exception e)
	{
		DebugOutput::println(QString("STD Exception: ") + e.what(), "", 0, MSG_TYPE::STDERROR);
	}

	m_running = false;
	DEBUG_PRINTLN("Data aquisition stopped");
}


void Controller::stop()
{
	m_running = false;
}
