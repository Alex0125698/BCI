#include <thread>
#include "controller.h"
#include "bciinterface.h"
#include "emotivinterface.h"
#include "spatial_filter.h"
#include "timer.h"
#include "error.h"



Controller::Controller()
	: m_bci_hardware{ new bci::EmotivInterface }
{
}


Controller::~Controller()
{
}

void Controller::mainwindowIsReady()
{
	m_mainwindow_ready = true;
}

void Controller::run()
{
	try
	{
		m_running = true;
		m_bci_hardware->connect();

		Timer view_refresh;

		while (m_running)
		{
			// get the data
			// TODO: store + save data
			auto&& tmp =  bci::spatial_filter::CAR(m_bci_hardware->getData());

			// update mainwindow every 200 ms if it is ready
			if ((view_refresh.getDuration() > 0.3))
			{
				m_mainwindow_ready = false;
				// store data statically for mainwindow
			   bci::BCI_Interface::data = std::move(tmp);

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
}


void Controller::stop()
{
	m_running = false;
}
