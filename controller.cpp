#include "resources.h"
#include "controller.h"
#include "cytoninterface.h"
#include "offline.h"
#include "sigprocessing.h"

// READY
void Controller::slotStart(int source, QString serialPort, QString file, uint32_t freq)
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
		m_bci = new bci::CytonInterface(serialPort);
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
		m_bci->disconnect(this);	
		emit sigRunStateChanged(false);
		m_bci->deleteLater();
		//delete m_bci;
		m_bci = nullptr;
		DEBUG_PRINTLN("Data aquisition stopped");
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

	if (!m_view_ready) m_viewTimer.restart();

	if (m_viewTimer.getDuration() > 0.066666)
	{
		emit sigViewRefresh();
		m_viewTimer.restart();
		m_view_ready = false;
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
	m_data.spfilTR.resize(1);
	m_data.spfilTR[0].resize(2);
	m_data.chCAR_TR.resize(m_bci->numChannels());

	// clear shared data
	{
		std::lock_guard<std::mutex> lock(shared->mtx_data);

		shared->resetMainwindow = true;
		shared->resetSTFT = true;
		shared->freq = m_bci->freq();
		shared->time_plot_time = 0;
		shared->freq_plot_time = 0;
		shared->tr_plot_time = 0;
		shared->stft_plot_time = 0;

		shared->rawTD.clear();
		shared->rawFD.clear();
		shared->imgOutputFD.clear();
		shared->ball_spfillTR.clear();
		shared->graph_spfillTR.clear();
		shared->chCAR_TR.clear();

		// resize shared data
		shared->rawTD.resize(m_bci->numChannels());
		shared->rawFD.resize(m_bci->numChannels());
		shared->chCAR_TR.resize(m_bci->numChannels(),0.0);
	}
}

// READY
void Controller::runFilters()
{
	// need to check if we changed the window size etc.
	// must be called before extract data since we may clear m_dataTD
	updateProperties();

	// get next set of data from the bci
	sassert(m_data.rawTD[0].size() <= m_dtft.wndSize-1);
	extractData();

	// check if we have enough points to do the transform
	const size_t reqTimePoints = m_dtft.wndSize;
	sassert(m_data.rawTD.size() > 0);
	const size_t numTimePoints = m_data.rawTD[0].size();
	sassert(numTimePoints <= m_dtft.wndSize);
	sassert(numTimePoints > 0);

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
		//sassert(m_data.imgStoreFD.size() == 2);
		//sassert(m_data.imgStoreFD[0].size() >= 1);
		//sassert(m_data.imgStoreFD[0][0].size() == numTimePoints);
		sassert(m_data.imgOutputFD.size() == 2);
		sassert(m_data.imgOutputFD[0].size() == numTimePoints);
		translation();
		//sassert(m_data.spfilTR.size() == m_trans.freqSpreading);
		sassert(m_data.spfilTR.size() == 1); // todo: remove
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

	m_bci->getData(tmpData,latestTime);

	sassert(m_data.rawTD.size() == m_bci->numChannels());

	// Warning: this is inefficient - we are filling in distant locations
	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		m_data.rawTD[ch].push_back(tmpData[ch]);
	}

	sassert(m_data.rawTD[0].size() <= m_dtft.wndSize);
}

// TODO m_data.spfilTR resize
// READY
void Controller::updateProperties()
{
	// the dtft properties selected by the user
	static DTFTproperties oldSTFT;
	oldSTFT = m_dtft;

	// we need to check for changes of the DTFT window
	{
		std::lock_guard<std::mutex> lock(MainWindowState::state.mtx_data);
		m_dtft = MainWindowState::state.dtft;
		m_trans = MainWindowState::state.translation;
	}

	{
		std::lock_guard<std::mutex> lock(BrainMapState::state.mtx_data);

		// brain map may not have initialise the spatial filter params yet
		if (BrainMapState::state.properties.spatial1Params.size() != m_bci->numChannels())
		{
			BrainMapState::state.properties.spatial1Params.resize(m_bci->numChannels(), 1.0);
			BrainMapState::state.properties.spatial2Params.resize(m_bci->numChannels(), 1.0);
		}
		m_spatial = BrainMapState::state.properties;
	}

	if (oldSTFT.wndSize    != m_dtft.wndSize    ||
		oldSTFT.wndOverlap != m_dtft.wndOverlap ||
		oldSTFT.wndType    != m_dtft.wndType    ||
	    oldSTFT.sharpenKernelSize != m_dtft.sharpenKernelSize ||
		oldSTFT.sharpenAmount     != m_dtft.sharpenAmount     ||
		oldSTFT.blurKernelSize    != m_dtft.blurKernelSize    ||
		oldSTFT.blurAmount        != m_dtft.blurAmount)
	{
		initData();
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		shared->resetMainwindow = true;
		shared->resetSTFT = true;
	}

}

// READY
void Controller::spatialFilers()
{
	// === left/right spatial filters ===

	auto numTimePoints = m_dtft.wndSize;

	sassert(m_data.spfilTD.size() == 2);
	m_data.spfilTD[0].resize(numTimePoints);
	m_data.spfilTD[1].resize(numTimePoints);

	algs::spatialFilter(m_data.rawTD, m_spatial.spatial1Params, m_data.spfilTD[0]);
	algs::spatialFilter(m_data.rawTD, m_spatial.spatial2Params, m_data.spfilTD[1]);
	
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
	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		for (size_t t = 0; t < numTimePoints; ++t)
		{
			m_data.chCAR_TD[ch][t] = m_data.rawTD[ch][t] - CARmags[t];
		}
	}
}

// READY
void Controller::freqTransforms()
{
	auto N = m_dtft.wndSize;

	// buffer for imaginary output
	static std::vector<double> imag;
	imag.resize(N);

	// === FFT of raw data ===

	sassert(m_data.rawTD.size() == m_bci->numChannels());
	sassert(m_data.rawFD.size() == m_bci->numChannels());
	sassert(m_data.rawTD[0].size() == N);

	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		// we put the time data in the freq data position (to start with)
		// as the FFT will apply the alg in place
		m_data.rawFD[ch] = m_data.rawTD[ch];
		// lets first get rid of the DC component
		algs::removeDC(m_data.rawFD[ch]);

		if (m_dtft.wndType == DFTwindow::GAUSSIAN)
			algs::applyGaussianWindow(m_data.rawFD[ch]);

		std::fill(imag.begin(), imag.end(), 0.0);
		algs::FFTransfrom(m_data.rawFD[ch], imag);

		// get the magnitudes ; we don't care about phase!
		for (size_t freq = 0; freq < N; ++freq)
		{
			const double real = m_data.rawFD[ch][freq];
			const double img = imag[freq];
			m_data.rawFD[ch][freq] = std::sqrt(real*real + img*img);
		}
	}

	// === FFT of channel CAR data ===

	sassert(m_data.chCAR_FD.size() == m_bci->numChannels());
	sassert(m_data.chCAR_TD.size() == m_bci->numChannels());
	sassert(m_data.chCAR_TD[0].size() == N);

	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		m_data.chCAR_FD[ch] = m_data.chCAR_TD[ch];
		algs::removeDC(m_data.chCAR_FD[ch]);

		if (m_dtft.wndType == DFTwindow::GAUSSIAN)
			algs::applyGaussianWindow(m_data.chCAR_FD[ch]);

		std::fill(imag.begin(), imag.end(), 0.0);
		algs::FFTransfrom(m_data.chCAR_FD[ch], imag);

		for (size_t freq = 0; freq < N; ++freq)
		{
			const double real = m_data.chCAR_FD[ch][freq];
			const double img = imag[freq];
			m_data.chCAR_FD[ch][freq] = std::sqrt(real*real + img * img);
		}
	}

	// === FFT of spfill data ===

	sassert(m_data.spfilTD.size() == m_data.spfilFD.size());
	sassert(m_data.spfilTD.size() == 2);
	sassert(m_data.spfilTD[0].size() == N);

	for (size_t i = 0; i < m_data.spfilTD.size(); ++i)
	{
		m_data.spfilFD[i] = m_data.spfilTD[i];
		algs::removeDC(m_data.spfilFD[i]);

		if (m_dtft.wndType == DFTwindow::GAUSSIAN)
			algs::applyGaussianWindow(m_data.spfilFD[i]);

		std::fill(imag.begin(), imag.end(), 0.0);
		algs::FFTransfrom(m_data.spfilFD[i], imag);

		for (size_t freq = 0; freq < N; ++freq)
		{
			const double real = m_data.spfilFD[i][freq];
			const double img = imag[freq];
			m_data.spfilFD[i][freq] = std::sqrt(real*real + img*img);
		}
	}
}

// TODO IMAGE PROCESSING
// READY
void Controller::imageProcessing()
{
	// === create image buffer ===


	// === apply blur + sharpen filters ===

	// skip for now
	m_data.imgOutputFD = m_data.spfilFD;
}

// TODO FREQ SPREAD
// READY
void Controller::translation()
{
	// convert freq to index
	size_t centreIndex = size_t(m_trans.centreFreq * (m_dtft.wndSize - 1) / m_bci->freq());
	sassert(centreIndex < m_dtft.wndSize);
	sassert(centreIndex >= 0);

	// the index band to add up
	size_t spanIndex = size_t(m_trans.spanFreq * (m_dtft.wndSize - 1) / m_bci->freq());
	if (spanIndex < 1) spanIndex = 1;
	sassert(spanIndex <= 10);

	double offset = 2.0*(m_trans.offset * 2.0 - 1.0);
	double gain = 0.01*std::pow(10.0, m_trans.gain*8.0 - 3.0);
	double sensitivity = std::pow(10.0, m_trans.sensitivity*4.0 - 2.0);

	size_t offMin = centreIndex - spanIndex;
	if (offMin < 0) offMin = 0;
	size_t offMax = centreIndex + spanIndex;
	if (offMax > m_dtft.wndSize - 1) offMax = m_dtft.wndSize - 1;

	// === translate channel CAR data ===

	sassert(m_data.chCAR_FD.size() == m_bci->numChannels());
	sassert(m_data.chCAR_TR.size() == m_bci->numChannels());

	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		double sum = 0.0;
		double totalW = 0.0;
		for (size_t freq = offMin; freq <= offMax; ++freq)
		{
			// parameter 0 to 1
			double s = (double(freq) - double(centreIndex)) / double(spanIndex);
			sassert(std::abs(s) <= 1.0);

			double weight = std::exp(-0.7*s*s);
			sassert(ch < m_data.chCAR_TD.size());
			sassert(freq < m_data.chCAR_TD[0].size());
			sum += weight * m_data.chCAR_TD[ch][freq];
			totalW += weight;
		}

		sum /= totalW;
		sum = sum * gain + offset;
		sum *= sensitivity;
		sum = std::clamp(sum, -1.0, 1.0);

		if (m_trans.flip) sum = -sum;

		double w1 = m_trans.smoothing;
		double w2 = 1.0 - w1;
		sassert(ch < m_data.chCAR_TR.size());
		m_data.chCAR_TR[ch] = w1 * m_data.chCAR_TR[ch] + w2 * sum;
	}

	// === translate spfill data ===

	sassert(m_data.spfilFD.size() == 2);
	sassert(m_data.spfilTR.size() == 1); // no freq spreading
	sassert(m_data.spfilTR[0].size() == 2);

	for (size_t n = 0; n < m_data.spfilFD.size(); ++n)
	{
		double sum = 0.0;
		double totalW = 0.0;
		for (size_t freq = offMin; freq <= offMax; ++freq)
		{
			// parameter 0 to 1
			double s = (double(freq) - double(centreIndex)) / double(spanIndex);
			sassert(std::abs(s) <= 1.0);

			double weight = std::exp(-0.7*s*s);
			sum += weight * m_data.spfilFD[n][freq];
			totalW += weight;
		}

		sum /= totalW;
		sum = sum * gain + offset;
		sum *= sensitivity;
		sum = std::clamp(sum, -1.0, 1.0);

		if (m_trans.flip)
			sum = -sum;

		double w1 = m_trans.smoothing;
		double w2 = 1.0 - w1;

		m_data.spfilTR[0][n] = w1 * m_data.spfilTR[0][n] + w2 * sum;
	}

}

// READY
void Controller::exportTimePlot()
{
	// === create filtered freq for MainWindow ===

	// dim1 = channel ; dim2 = freq
	std::vector<std::vector<double>> tmp;

	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		tmp = std::move(shared->rawTD);
		shared->rawTD.clear();
	}

	// append new array
	if (tmp.size() != m_bci->numChannels())
		tmp.resize(m_bci->numChannels());

	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		tmp[ch].reserve(tmp.size() + m_data.rawTD.size());

		for (size_t t=m_dtft.wndOverlap; t<m_dtft.wndSize; ++t)
			tmp[ch].push_back(m_data.rawTD[ch][t]);
	}

	{
		// put array back
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		shared->rawTD = std::move(tmp);
		shared->time_plot_time = latestTime;
	}
	tmp.clear();
}

// READY
void Controller::exportFreqPlot()
{
	// === create filtered freq for MainWindow ===

	// dim1 = channel ; dim2 = freq
	std::vector<std::vector<double>> tmp;

	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		tmp = std::move(shared->rawFD);
		shared->rawFD.clear();
	}
	if (tmp.size() != m_bci->numChannels())
		tmp.resize(m_bci->numChannels());
	//sassert(tmp.size() == m_bci->numChannels());
	sassert(m_data.rawFD.size() == m_bci->numChannels());

	if (tmp[0].size() != m_dtft.wndSize)
	{
		// no data yet, so we copy the data
		tmp = m_data.rawFD;
	}
	else
	{
		// filter data using old data
		for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
		{
			for (size_t f = 0; f < m_dtft.wndSize; ++f)
			{
				tmp[ch][f] = 0.9*tmp[ch][f] + 0.1*m_data.rawFD[ch][f];
			}
		}
	}

	{
		// put data back
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		shared->rawFD = std::move(tmp);
		shared->freq_plot_time = latestTime;
	}
	tmp.clear();
}

// TODO TIME
// READY
void Controller::exportSTFTPlot()
{
	std::vector<std::vector<std::vector<double>>> tmp;
	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		tmp = std::move(shared->imgOutputFD);
		shared->imgOutputFD.clear();
	}

	if (tmp.size() > 0)
	{
		sassert(tmp[0].size() == 2);
		sassert(tmp[0][0].size() == m_dtft.wndSize);
	}

	// append the new data	
	tmp.push_back(m_data.imgOutputFD);

	// copy all data back into the shared state
	{
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		shared->imgOutputFD = std::move(tmp);
		shared->stft_plot_time = latestTime;
	}
	tmp.clear();
}

// READY
void Controller::exportBallTest()
{
	std::vector<std::vector<double>> tmp;
	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(shared->mtx_data);		
		tmp = std::move(shared->ball_spfillTR);
		shared->ball_spfillTR.clear();
	}

	sassert((m_data.spfilTR.size() & 0x01) == 1);
	auto centre = (m_data.spfilTR.size()-1) / 2;
	tmp.push_back(m_data.spfilTR[centre]);

	{
		// put data back
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		shared->ball_spfillTR = std::move(tmp);
	}
	tmp.clear();
}

// READY
void Controller::exportTrPlot()
{
	std::vector<std::vector<std::vector<double>>> tmp;
	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		tmp = std::move(shared->graph_spfillTR);
		shared->graph_spfillTR.clear();
	}

	sassert((m_data.spfilTR.size() & 0x01) == 1);
	tmp.push_back(m_data.spfilTR);

	{
		// put data back
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		shared->graph_spfillTR = std::move(tmp);
		shared->tr_plot_time = latestTime;
	}
	tmp.clear();
}

// READY
void Controller::exportBrainMap()
{
	std::vector<double> tmp;
	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		tmp = std::move(shared->chCAR_TR);
		shared->chCAR_TR.clear();
	}

	if (tmp.size() != m_bci->numChannels())
	{
		// no data yet, so we copy the data
		tmp = m_data.chCAR_TR;
	}
	else
	{
		// filter data using old data
		for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
		{
			tmp[ch] = 0.6*tmp[ch] + 0.4*m_data.chCAR_TR[ch];
		}
	}

	{
		// put data back
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		shared->chCAR_TR = std::move(tmp);
	}
	tmp.clear();
}

// READY
void Controller::exportAll()
{
	std::vector<std::vector<double>> tmp;
	{
		// temporarily grab old data
		std::lock_guard<std::mutex> lock(shared->mtx_data);
		shared->timeLerp = (m_dtft.wndSize - m_dtft.wndOverlap) / m_bci->freq();
	}

	exportTimePlot();

	exportFreqPlot();

	exportBallTest();

	exportTrPlot();

	exportBrainMap();

	exportSTFTPlot();
}

// TODO CLEAR IMAGE PROCESSING DATA
// READY
void Controller::removeOldData()
{
	// === time data ===

	// we need to get rid of the old timpoints
	// but keep enough for the overlap
	const size_t numKeep = m_dtft.wndOverlap;
	const size_t offset = m_dtft.wndSize - numKeep;

	sassert(numKeep < m_dtft.wndSize);
	sassert(m_data.rawTD.size() == m_bci->numChannels());
	sassert(m_data.rawTD[0].size() == m_dtft.wndSize);

	for (size_t ch = 0; ch < m_bci->numChannels(); ++ch)
	{
		sassert(m_data.rawTD[ch].size() == m_dtft.wndSize);

		// we cannot guarantee no alaising so use normal loop
		for (size_t freq = 0; freq < numKeep; ++freq)
		{
			sassert(freq + offset < m_data.rawTD[ch].size());
			m_data.rawTD[ch][freq] = m_data.rawTD[ch][freq + offset];
		}
		// todo: check for alaising and use std::copy

		// this will effectively delete the old data
		m_data.rawTD[ch].resize(numKeep);
	}

	sassert(m_data.rawTD[0].size() == m_dtft.wndOverlap);

	// === image processing data ===
	
}
