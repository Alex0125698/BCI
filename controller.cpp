#include "controller.h"
#include "bciinterface.h"
#include "cytoninterface.h"
#include "offline.h"
#include "sigprocessing.h"
#include "state.h"
#include <QTimer>

Controller::Controller()
{
}

void Controller::slotStart(int source, QString file, uint32_t freq)
{
	assert(m_bci == nullptr);

	// reallocate memory + thread for each connect sig
	switch (source)
	{
	case BCISource::Emotiv:
		DEBUG_PRINTLN("Emotiv is unavaliable. Please fix emotiv.cpp");
		return;
	case BCISource::Offline:
		m_bci = new bci::Offline(file.toStdString(), freq);
		break;
	case BCISource::OpenBCI:
		m_bci = new bci::CytonInterface;
		break;
	}

	// redo connections
	connect(m_bci, &bci::Interface::sigDataReady, this, &Controller::slotDataReady, Qt::QueuedConnection);
	connect(m_bci, &bci::Interface::sigCallStopHelper, this, &Controller::slotStop, Qt::QueuedConnection);
	connect(m_bci, &bci::Interface::sigError, this, [this](QString msg, QString file, int line) {
		slotStop(); DebugOutput::println(msg, file, line, MSG_TYPE::DEXCEP);
	}, Qt::QueuedConnection);

	// start data aquisition
	m_run_timer.restart();
	DEBUG_PRINTLN("clear");
	emit sigRunStateChanged(true);
	m_bci->start();
	DEBUG_PRINTLN("Data aquisition started");

	int counter = 0;
	while (true)
	{
		if (m_bci->ready()) break;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		++counter;

		if (counter >= 100)
		{
			DEBUG_PRINTLNY("ERROR: BCI took too long to respond", MSG_TYPE::DEXCEP);
			slotStop();
			return;
		}
	}

	{
		std::lock_guard<std::mutex> lock(state->mtx_data);
		state->freq = m_bci->freq();
	}

	resetData();
	if (m_bci->numChannels() == 0)
	{
		DEBUG_PRINTLNY("ERROR: BCI has 0 channels", MSG_TYPE::DEXCEP);
		slotStop();
	}
}

void Controller::slotStop()
{
	if (m_bci != nullptr)
	{
		m_bci->stop();
		m_bci->disconnect();
		DEBUG_PRINTLN("Data aquisition stopped");
		emit sigRunStateChanged(false);
		delete m_bci;
		m_bci = nullptr;
	}
}

// TODO clear shared data
// READY TO TEST
void Controller::resetData()
{
	m_timeIndex = -1;
	m_dataTD.clear();
	m_dataTD.resize(m_bci->numChannels());
	m_dataFD.clear();
	m_dataFD.resize(m_bci->numChannels());
	m_laplaceData.clear();
	m_finalData.clear();
}

// TODO: use a for loop to get data more quickly?
// READY TO TEST
void Controller::extractData()
{
	// poll the bci for the next channel data
	// we need a temporary vector to fill in
	static std::vector<double> tmp;
	if (tmp.size() != m_bci->numChannels()) tmp.resize(m_bci->numChannels());
	m_bci->getData(tmp);

	// the first buffer to fill in is m_dataTD
	// Warning: this is inefficient - we are filling in distant locations
	const size_t numChannels = m_dataTD.size();
	for (size_t ch = 0; ch < numChannels; ++ch)
	{
		m_dataTD[ch].push_back(tmp[ch]);
	}
}

// READY TO TEST
void Controller::check_dtft_properties()
{
	// the dtft properties selected by the user
	DTFTproperties* dtft = &MainWindowState::state.dtft;

	// we need to check for changes
	{
		std::lock_guard<std::mutex> lock(MainWindowState::state.mtx_data);
		if (dtft->wndSize != m_dtft_properties.wndSize ||
			dtft->wndOverlap != m_dtft_properties.wndOverlap ||
			dtft->wndType != m_dtft_properties.wndType)
		{
			// in this case we give up trying to update all the data
			// just delete the old data and start again with the new windowing
			m_wnd_changed = true;
		}
		else if (dtft->sharpenKernelSize != m_dtft_properties.sharpenKernelSize ||
			dtft->sharpenAmount != m_dtft_properties.sharpenAmount ||
			dtft->blurKernelSize != m_dtft_properties.blurKernelSize ||
			dtft->blurAmount != m_dtft_properties.blurAmount)
		{
			// in this case we re-calc all the data with the new
			// image processing parameters
			m_recalc_img_filter = true;
		}

		// finally update our copy of dtft properties
		m_dtft_properties = *dtft;

		// note we dont worry about DTFTproperties::enabled - only the plotting
		// can be disabled, not the signal processing
	}

	// new windowing means we clear the old data
	if (m_wnd_changed)
	{
		m_wnd_changed = false;
		for (auto& d : m_dataTD) d.clear();
		// clear all timepoints
		for (auto& d : m_dataFD) d.clear();

		m_laplaceData.clear();
		m_finalData.clear();

		m_timeIndex = -1; // -1 means no data yet
	}
}

// READY TO TEST
void Controller::freqTransform()
{
	// Warning! this function requires N time points in m_dataTD

	// note: correct number of channels set in slotStart()
	const size_t numChannels = m_dataTD.size();
	const size_t N = m_dtft_properties.wndSize;

	// need to store the imaginary output of the FFT
	static std::vector<double> imag;
	if (imag.size() != N) imag.resize(N);

	// run the DFT over all channels
	for (size_t ch = 0; ch < numChannels; ++ch)
	{
		// emplace_back a new time point in the FD data
		// each time point is a vector containing the freq data
		// we put the time data in the freq data position
		// as the FFT will apply the alg in place
		m_dataFD[ch].push_back(m_dataTD[ch]);

		// apply the window function
		// note: m_timeIndex = m_dataFD[ch].size()-1
		if (m_dtft_properties.wndType == DFTwindow::GAUSSIAN)
			applyGaussianWindow(m_dataFD[ch][m_timeIndex]);

		// reset imaginary data to 0
		std::fill(imag.begin(),imag.end(), 0.0);

		// now run the FFT alg
		Fft::transform(m_dataFD[ch][m_timeIndex], imag);

		// get the magnitudes ; we don't care about phase!
		for (size_t freq = 0; freq < imag.size(); ++freq)
		{
			const double real = m_dataFD[ch][m_timeIndex][freq];
			const double img = imag[freq];
			m_dataFD[ch][m_timeIndex][freq] = std::sqrt(real*real + img*img);
		}
	}
}

// READY TO TEST
void Controller::clearOldData()
{
	// we need to get rid of the old timpoints
	// but keep enough for the overlap
	const size_t numChannels = m_dataTD.size();
	const size_t numKeep = m_dtft_properties.wndOverlap;
	const size_t offset = m_dtft_properties.wndSize - numKeep;

	for (size_t ch = 0; ch < numChannels; ++ch)
	{
		assert(m_dataTD[ch].size() == m_dtft_properties.wndSize);

		// we cannot guarantee no alaising so use normal loop
		for (size_t freq = 0; freq < numKeep; ++freq)
		{
			m_dataTD[ch][freq] = m_dataTD[ch][freq + offset];
		}
		// todo: check for alaising and use std::copy

		m_dataTD[ch].resize(numKeep);
	}

	// there is a maximum number of timepoints for the freq data
	// we may need to delete from the front
	// the timpoints use std::deque so this is fast
	while (m_timeIndex >= m_dtft_properties.timeSpan)
	{
		--m_timeIndex;
		for (size_t ch = 0; ch < numChannels; ++ch)
		{
			m_dataFD[ch].pop_front();
		}
		m_laplaceData.pop_front();
		// we do not touch final data - it will be exported
	}
}

// ADD LAPLACIAN FILTER
// READY TO TEST
void Controller::laplaceFilter()
{
	// apply laplacian/CAR filter
	// for now only use CAR
	const size_t numFreqPoints = m_dataFD[0][0].size();
	const size_t numChannels = m_dataTD.size();

	m_laplaceData.push_back(std::vector<double>(numFreqPoints));

	// add up all channel data 
	for (int ch = 0; ch < numChannels; ++ch)
	{
		for (int freq = 0; freq < numFreqPoints; ++freq)
		{
			m_laplaceData[m_timeIndex][freq] += m_dataFD[ch][m_timeIndex][freq];
		}
	}

	// normalise
	for (int freq = 0; freq < numFreqPoints; ++freq)
	{
		m_laplaceData[m_timeIndex][freq] /= (double)numChannels;
	}
}

// NOT DONE YET
// READY TO TEST
void Controller::imageProcessing()
{
	// apply blur + sharpen filters
	// for now just skip these
	m_finalData.emplace_back(m_laplaceData[m_timeIndex]);

	if (m_recalc_img_filter)
	{
		m_recalc_img_filter = false;
	}
}

// NEED TO ADD TIME AND FREQS
// READY TO TEST
void Controller::exportData()
{
	// check if buffer is getting too large

	size_t totalBufferSize = m_finalData.size();

	{
		std::lock_guard<std::mutex> lock(state->mtx_data);
		totalBufferSize += state->finalData.size();
	}

	if (totalBufferSize > 16)
	{
		DEBUG_PRINTLNY("buffer overflow - DTFT was unable to keep up with controller", MSG_TYPE::DEXCEP);
		slotStop();
	}

	std::vector<std::vector<double>> tmp;

	// temporarily grab the old data
	{
		std::lock_guard<std::mutex> lock(state->mtx_data);
		tmp = std::move(state->finalData);
		state->finalData.clear();
	}

	// append the new data	
	auto oldSize = tmp.size();
	tmp.resize(oldSize + m_finalData.size());
	for (size_t i = 0; i < m_finalData.size(); ++i)
	{
		// need to use std::move to copy just the iterators and not the whole array
		tmp[i + oldSize] = std::move(m_finalData[i]);
	}
	m_finalData.clear();

	// copy all data back into the shared state
	{
		std::lock_guard<std::mutex> lock(state->mtx_data);
		state->finalData = std::move(tmp);
	}
	tmp.clear();
}

// TIMERS + STATE NOT DONE
// READY TO TEST
void Controller::slotDataReady()
{
	if (m_bci == nullptr)
		return;
	
	if (!m_bci->isConnected())
	{
		slotStop();
		return;
	}

	static Timer viewRefreshTimer;
	static Timer calcTime;
	static double totalCalcTime{ 0 };

	m_calc_time.restart();

	// need to check if we changed the window size etc.
	// must be called before extract data since we may clear m_dataTD
	check_dtft_properties();

	// get next set of data from the bci
	extractData();

	// check if we have enough points to do the transform
	const size_t reqTimePoints = m_dtft_properties.wndSize;
	const size_t numTimePoints = m_dataTD[0].size();
	if (numTimePoints >= reqTimePoints)
	{
		++m_timeIndex;
		// calculate dft 
		freqTransform();
		// calc laplace / CAR filter
		laplaceFilter();
		// calc blur + sharpen filters
		imageProcessing();
		// remove old data
		clearOldData();
		// create new shared data for MainWindow + STFTplot
		// this data is asynchronous and is removed whenever it is polled
		exportData();
	}

	if (viewRefreshTimer.getDuration() > 0.1) emit sigViewRefresh();
}
