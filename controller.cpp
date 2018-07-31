#include "controller.h"
#include "csvio.h"
#include "bciinterface.h"
#include "emotiv.h"
#include "offline.h"
#include "spatial_filter.h"
//#include "state.h"


template<typename T>
T map(T val, T val_min, T val_max, T min, T max)
{
	return (max-min)*(val-val_min)/(val_max-val_min) + min;
}

Controller::Controller()
	//: m_bci{ new bci::EmotivInterface }
	: m_bci{ new bci::Offline("right_smr_1.csv") }
{
}

void Controller::slotRun()
{
	const size_t DFT_SIZE = 512;
	//std::vector<std::vector<double>> dft_data;
	try
	{
		m_bci->start();
		/*
		dft_data.resize(m_bci->channels.size());
		for (auto& ch : dft_data)
		{
			ch.resize(DFT_SIZE);
		}
		*/
		size_t dft_counter = 0;

		Timer view_refresh_timer;

		m_running = true;
		emit sigRunStateChanged(true);

		DEBUG_PRINTLN("Data aquisition started");
		while (m_running)
		{
			if (!m_bci->connected()) break;

			//m_bci->update();
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
			// update mainwindow every 200 ms if it is ready
			emit sigGraphUpdate();
			
			if (m_view_ready && view_refresh_timer.getDuration() > 0.2)
			{
				//m_view_ready = false;
				emit sigViewUpdate();
				view_refresh_timer.restart();
			}

			// prevent maxing out CPU
			std::this_thread::sleep_for(std::chrono::milliseconds(0));
			std::this_thread::sleep_for(std::chrono::milliseconds(0));
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
	emit sigRunStateChanged(false);
	DEBUG_PRINTLN("Data aquisition stopped");
}

void Controller::stop()
{
	m_running = false;
}
