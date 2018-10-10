#include "resources.h"
#include "controller.h"
#include "cytoninterface.h"
#include "offline.h"
#include "sigprocessing.h"

// READY
void Controller::slotStart(int source, QString file, uint32_t freq)
{
	sassert(m_bci == nullptr);

	// reallocate memory + thread for each connect sig
	switch (source)
	{
	case BCISource::Emotiv:
		DEBUG_PRINTLNY("Emotiv is unavaliable. Please fix emotiv.cpp", MSG_TYPE::DEXCEP);
		return;
	case BCISource::Offline:
		m_bci = new bci::Offline(file.toStdString(), freq);
		break;
	case BCISource::OpenBCI:
		m_bci = new bci::CytonInterface;
		break;
	}

	// make connections
	connect(m_bci, &bci::Interface::sigDataReady, this, &Controller::slotDataReady, Qt::QueuedConnection);
	connect(m_bci, &bci::Interface::sigCallStopHelper, this, &Controller::slotStop, Qt::QueuedConnection);
	connect(m_bci, &bci::Interface::sigError, this, [this](QString msg, QString file, int line) {
		slotStop(); DebugOutput::println(msg, file, line, MSG_TYPE::DEXCEP);
	}, Qt::QueuedConnection);

	// reset timers to 0
	m_runTimer.restart();
	m_viewTimer.restart();
	m_calcTimer.restart();

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

	if (m_bci->numChannels() == 0)
	{
		DEBUG_PRINTLNY("ERROR: BCI has 0 channels", MSG_TYPE::DEXCEP);
		slotStop();
		return;
	}

	initData();
}

// READY
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

// READY
void Controller::slotDataReady()
{
	if (m_bci == nullptr) return;
	if (!m_bci->isConnected())
	{
		slotStop();
		return;
	}

	runFilters();

	if (m_viewTimer.getDuration() > 0.15)
	{
		emit sigViewRefresh();
		m_viewTimer.restart();
	}
}

// READY
void Controller::initData()
{
	sassert(m_bci != nullptr);
	sassert(m_bci->numChannels() > 0);
	sassert((m_bci->freq() >= 1.0) && (m_bci->freq() <= 1000.0));

	// clear controller data
	m_data.rawTD.clear();
	m_data.spfilTD.clear();
	m_data.chCAR_TD.clear();
	m_data.rawFD.clear();
	m_data.spfilFD.clear();
	m_data.imgStoreFD.clear();
	m_data.imgOutputFD.clear();
	m_data.chCAR_FD.clear();
	m_data.spfilTR.clear();
	m_data.chCAR_TR.clear();

	// resize controller data
	m_data.rawTD.resize(m_bci->numChannels());
	m_data.spfilTD.resize(2);
	m_data.chCAR_TD.resize(m_bci->numChannels());
	m_data.rawFD.resize(m_bci->numChannels());
	m_data.spfilFD.resize(2);
	m_data.imgStoreFD.resize(2);
	m_data.imgOutputFD.resize(2);
	m_data.chCAR_FD.resize(m_bci->numChannels());
	//m_data.spfilTR.resize(n);
	m_data.chCAR_TR.resize(m_bci->numChannels());

	// clear shared data
	{
		std::lock_guard<std::mutex> lock(shared->mtx_data);

		shared->reset = true;
		shared->freq = m_bci->freq();
		shared->latestTime = 0;

		shared->rawTD.clear();
		shared->rawD_times.clear();
		shared->rawFD.clear();
		shared->imgOutpuFD.clear();
		shared->ball_spfillTR.clear();
		shared->graph_spfillTR.clear();
		shared->chCAR_TR.clear();
	}
}

// READY
void Controller::runFilters()
{
	// need to check if we changed the window size etc.
	// must be called before extract data since we may clear m_dataTD
	updateProperties();

	// get next set of data from the bci
	extractData();

	// check if we have enough points to do the transform
	const size_t reqTimePoints = m_dtft.wndSize;
	sassert(m_data.rawTD.size() > 0);
	const size_t numTimePoints = m_data.rawTD[0].size();
	sassert(numTimePoints <= m_dtft.wndSize);

	if (numTimePoints == reqTimePoints)
	{
		spatialFilers();
		sassert(m_data.spfilTD.size() > 0);
		sassert(m_data.spfilTD[0].size() == numTimePoints);
		sassert(m_data.chCAR_TD.size() == m_bci->numChannels());
		sassert(m_data.chCAR_TD[0].size() == numTimePoints);
		freqTransforms();
		sassert(m_data.rawFD.size() == m_bci->numChannels());
		sassert(m_data.rawFD[0].size() == numTimePoints);
		sassert(m_data.spfilFD.size() == 2);
		sassert(m_data.spfilFD[0].size() == numTimePoints);
		sassert(m_data.chCAR_FD.size() == m_bci->numChannels());
		sassert(m_data.chCAR_FD[0].size() == numTimePoints);
		imageProcessing();
		sassert(m_data.imgStoreFD.size() == 2);
		sassert(m_data.imgStoreFD[0].size() >= 1);
		sassert(m_data.imgStoreFD[0][0].size() == numTimePoints);
		sassert(m_data.imgOutputFD.size() == 2);
		sassert(m_data.imgOutputFD[0].size() == numTimePoints);
		translation();
		sassert(m_data.spfilTR.size() == m_trans.freqSpreading);
		sassert(m_data.spfilTR[0].size() == 2);
		sassert(m_data.chCAR_TR.size() == m_bci->numChannels());
		exportAll();
		removeOldData();
	}
}

// READY
void Controller::extractData()
{
	// poll the bci for the next channel data
	// we need a temporary vector to fill in
	static std::vector<double> tmpData;
	tmpData.resize(m_bci->numChannels());

	double time;
	m_bci->getData(tmpData,time);

	sassert(m_data.rawTD.size() == m_bci->numChannels());

	// Warning: this is inefficient - we are filling in distant locations
	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		m_data.rawTD[ch].push_back(tmpData[ch]);
	}

	sassert(m_data.rawTD[0].size() <= m_dtft.wndSize);
}

void Controller::updateProperties()
{
	// the dtft properties selected by the user
	static DTFTproperties oldSTFT;
	oldSTFT = m_dtft;

	// we need to check for changes of the DTFT window
	{
		std::lock_guard<std::mutex> lock(MainWindowState::state.mtx_data);
		m_dtft = MainWindowState::state.dtft;
		// also grab latest copy of TrProperties
		m_trans = MainWindowState::state.translation;
	}

	if (oldSTFT.wndSize != m_dtft.wndSize ||
		oldSTFT.wndOverlap != m_dtft.wndOverlap ||
		oldSTFT.wndType != m_dtft.wndType)
	{
		// in this case we give up trying to update all the data
		// just delete the old data and start again with the new windowing
		initData();
	}
	else if (oldSTFT.sharpenKernelSize != m_dtft.sharpenKernelSize ||
		oldSTFT.sharpenAmount != m_dtft.sharpenAmount ||
		oldSTFT.blurKernelSize != m_dtft.blurKernelSize ||
		oldSTFT.blurAmount != m_dtft.blurAmount)
	{
		// in this case we re-calc all the data with the new
		// image processing parameters
		initData();
	}

	// note we dont worry about DTFTproperties::enabled - only the plotting
	// can be disabled, not the signal processing
}

void Controller::spatialFilers()
{
	// === left/right spatial filters ===

	auto numTimePoints = m_dtft.wndSize;

	sassert(m_data.spfilTD.size() == 2);
	m_data.spfilTD[0].resize(numTimePoints);
	m_data.spfilTD[1].resize(numTimePoints);

	spatialFilter(m_data.rawTD, ,m_data.spfilTD[0]);
	spatialFilter(m_data.rawTD, ,m_data.spfilTD[1]);
	
	// === channel CAR filters ===

	sassert(m_data.chCAR_TD.size() == m_bci->numChannels());
	for (auto& v : m_data.chCAR_TD)
		v.resize(numTimePoints);

	// temporary vector to store CAR references
	static std::vector<double> CARmags;
	CARmags.resize(numTimePoints);

	// calc reference/average of each timepoint
	for (size_t t = 0; t < numTimePoints; ++t)
	{
		CARmags[t] = 0;
		for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
		{
			CARmags[t] += m_data.rawTD[ch][t];
		}
		// normalise
		CARmags[t] /= m_bci->numChannels();
	}

	// now subtract the reference from each channel
	for (size_t ch = 0; ch < numTimePoints; ++ch)
	{
		for (size_t t = 0; t < numTimePoints; ++t)
		{
			m_data.chCAR_TD[ch][t] = m_data.rawTD[ch][t] - CARmags[t];
		}
	}
}

void Controller::freqTransforms()
{
	sassert(m_dataTD.size() == m_bci->numChannels());
	sassert(m_dataFD.size() == m_bci->numChannels());
	sassert(m_dataTD[0].size() == m_dtft.wndSize);
	const auto N = m_dtft.wndSize;

	// need to store the imaginary output of the FFT
	static std::vector<double> imag;
	if (imag.size() != N) imag.resize(N);

	// run the DFT over all channels
	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		// emplace_back a new time point in the FD data
		// each time point is a vector containing the freq data

		// we put the time data in the freq data position (to start with)
		// as the FFT will apply the alg in place
		m_dataFD[ch].push_back(m_dataTD[ch]);

		// apply the window function
		sassert(m_timeIndex == (m_dataFD[ch].size()-1));
		sassert(m_dtft.wndType == DFTwindow::GAUSSIAN); //remove
		if (m_dtft.wndType == DFTwindow::GAUSSIAN)
			applyGaussianWindow(m_dataFD[ch][m_timeIndex]);

		// reset imaginary data to 0
		std::fill(imag.begin(), imag.end(), 0.0);
		// now run the FFT alg
		Fft::transform(m_dataFD[ch][m_timeIndex], imag);

		// get the magnitudes ; we don't care about phase!
		for (size_t freq = 0; freq < N; ++freq)
		{
			const double real = m_dataFD[ch][m_timeIndex][freq];
			const double img = imag[freq];
			m_dataFD[ch][m_timeIndex][freq] = std::sqrt(real*real + img*img);
		}
	}
}

void Controller::imageProcessing()
{
	// apply blur + sharpen filters
	// for now just skip these
	sassert(m_laplaceData.size()-1 == m_timeIndex);
	m_finalData = m_laplaceData[m_timeIndex];

	if (m_recalc_img_filter)
	{
		m_recalc_img_filter = false;
	}
}

void Controller::translation()
{
	// this alg will create a stat. proportional to the mag. of a
	// user-selected freq. band

	sassert(m_finalData.size() == m_dtft.wndSize);

	// convert freq to index
	int centreIndex = int(m_trans.centreFreq * (m_dtft.wndSize-1) / m_bci->freq());
	sassert(centreIndex < m_dtft.wndSize);
	sassert(centreIndex >= 0);

	double offset = 2.0*(m_trans.offset * 2.0 - 1.0);
	int spanIndex = int(m_trans.spanFreq * (m_dtft.wndSize - 1) / m_bci->freq());
	if (spanIndex < 1) spanIndex = 1;
	sassert(spanIndex <= 10);

	double gain = 0.01*std::pow(10.0, m_trans.gain*8.0 - 3.0);
	double sensitivity = std::pow(10.0, m_trans.sensitivity*4.0 - 2.0);

	int offMin = centreIndex - spanIndex;
	if (offMin < 0) offMin = 0;
	
	int offMax = centreIndex + spanIndex;
	if (offMax > m_dtft.wndSize-1) offMax = m_dtft.wndSize-1;

	double sum = 0.0;
	for (size_t i = offMin; i <= offMax; ++i)
	{
		// weird bug when we don't cast separately
		// parameter 0 to 1

		double s = (double(i) - double(centreIndex)) / double(spanIndex);
		sassert(std::abs(s) <= 1.0);
		
		double weight = std::exp(-0.7*s*s);
		sum += weight * m_finalData[i];
	}

	sum = sum * gain + offset;
	sum *= sensitivity;
	sum = std::clamp(sum, -1.0, 1.0);

	if (m_trans.flip)
		sum = -sum;

	static double oldSum = 0;

	double w1 = m_trans.smoothing;
	double w2 = 1.0 - w1;

	m_CARcontrolData = w1*oldSum + w2*sum;

	oldSum = m_CARcontrolData;
}

void Controller::exportTimePlot()
{
}

void Controller::exportFreqPlot()
{
}

void Controller::exportSTFTPlot()
{
}

void Controller::exportBallTest()
{
}

void Controller::exportTrPlot()
{
}

void Controller::exportBrainMap()
{
}

void Controller::exportAll()
{
	// ===== check if buffer is getting too large =====

	size_t totalBufferSize = 0;
	{
		std::lock_guard<std::mutex> lock(state->mtx_data);
		totalBufferSize = state->finalData.size();
	}

	if (totalBufferSize > 64)
	{
		DEBUG_PRINTLNY("buffer overflow - DTFT was unable to keep up with controller", MSG_TYPE::DEXCEP);
		slotStop();
		return;
	}



	// === create filtered freq for MainWindow ===

	// dim1 = channel ; dim2 = freq
	std::vector<std::vector<double>> tmp;
	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(state->mtx_data);
		tmp = std::move(state->filteredFreqs);
		state->filteredFreqs.clear();
	}

	if (tmp.size() != m_bci->numChannels())
		tmp.resize(m_bci->numChannels());

	sassert(m_dataFD[0].size() - 1 == m_timeIndex);
	sassert(m_dataFD.size() == m_bci->numChannels());

	if (tmp[0].size() != m_dtft.wndSize)
	{
		// copy data
		for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
		{
			tmp[ch].resize(m_dtft.wndSize);
			std::copy(m_dataFD[ch][m_timeIndex].begin(), m_dataFD[ch][m_timeIndex].end(), tmp[ch].begin());
		}

	}
	else
	{
		for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
		{
			for (size_t f = 0; f < m_dtft.wndSize; ++f)
			{
				tmp[ch][f] = 0.9*tmp[ch][f] + 0.1*m_dataFD[ch][m_timeIndex][f];
			}
		}
	}

	{
		// put data back
		std::lock_guard<std::mutex> lock(state->mtx_data);
		state->filteredFreqs = std::move(tmp);
		tmp.clear();
	}



	// === move data to final data buffer ===
	std::vector<std::vector<double>> tmp2;
	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(state->mtx_data);
		tmp2 = std::move(state->finalData);
		state->finalData.clear();
	}

	// append the new data	
	auto oldSize = tmp2.size();
	tmp2.push_back(m_finalData);
	//m_finalData.clear();

	// copy all data back into the shared state
	{
		std::lock_guard<std::mutex> lock(state->mtx_data);
		state->finalData = std::move(tmp2);
	}
	tmp2.clear();



	// === export control signal data ===

	std::vector<double> tmp3;
	std::vector<double> tmp4;
	{
		std::lock_guard<std::mutex> lock(state->mtx_data);
		tmp3 = std::move(state->controlMW);
		state->controlMW.clear();
		tmp4 = std::move(state->controlBT);
		state->controlBT.clear();
	}

	tmp3.push_back(m_CARcontrolData);
	tmp4.push_back(m_CARcontrolData);

	{
		std::lock_guard<std::mutex> lock(state->mtx_data);
		state->controlMW = std::move(tmp3);
		state->controlBT = std::move(tmp4);
	}

	tmp3.clear();
	tmp4.clear();
}

void Controller::removeOldData()
{
	sassert(m_dataTD.size() == m_bci->numChannels());

	// we need to get rid of the old timpoints
	// but keep enough for the overlap
	const size_t numKeep = m_dtft.wndOverlap;
	const size_t offset = m_dtft.wndSize - numKeep;

	sassert(numKeep < m_dtft.wndSize);

	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		sassert(m_dataTD[ch].size() == m_dtft.wndSize);

		// we cannot guarantee no alaising so use normal loop
		for (size_t freq = 0; freq < numKeep; ++freq)
		{
			if (!(freq + offset < m_dataTD[ch].size()))
			{
				sassert(false);
			}
			sassert(freq + offset < m_dataTD[ch].size());
			m_dataTD[ch][freq] = m_dataTD[ch][freq + offset];
		}
		// todo: check for alaising and use std::copy

		// this will effectively delete the old data
		m_dataTD[ch].resize(numKeep);
	}

	sassert(m_dataTD[0].size() <= m_dtft.wndSize);

	// there is a maximum number of timepoints for the freq data
	// we just keep enough to do the image processing
	// we may need to delete from the front
	// the timpoints use std::deque so this is fast
	// todo: allow for more timepoins
	while (m_timeIndex > -1)
	{
		--m_timeIndex;
		for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
		{
			m_dataFD[ch].pop_front();
		}
		m_laplaceData.pop_front();
		// we do not touch final data - it will be exported
	}

	sassert(m_timeIndex == -1);
	sassert(m_dataTD[0].size() == m_dtft.wndOverlap);
}
