/**
* Controller class - This is the only class that owns the BCI objects.
* So far there are 3 BCI sources:
* (1) offline
* (2) OpenBCI
* (3) Emotiv
* 
* This class connects to the bci, applys the filters and updates 
* the state so that the data can be displayed. In sends runState and saveState
* signals back to the mainwindow so that it can be updated properly.
* 
* Error handling - will send stop signal to mainwindow and disconnect the BCI.
*
* Author:  A.S. Woodcock
* Project: BCI Controller for Biomedical Applications
*/

#pragma once

#include "resources.h"
#include "brainmapstate.h"
#include "mainwindowstate.h"
#include "controllerstate.h"

// forward declarations
namespace bci { class Interface; }

enum BCISource
{
	Offline,
	OpenBCI,
	Emotiv
};

class Controller : public QObject
{
	Q_OBJECT

signals:
	// feedBack for mainwindow
	void sigRunStateChanged(bool running);
	// feedBack for mainwindow
	void sigSaveStateChanged(bool saving);
	// tell mainwindow to redraw graphs + labels
	void sigViewRefresh();

public slots:
	// creates the BCI object, data aquisition starts auomatically
	void slotStart(int source, QString serialPort, QString file, uint32_t freq);
	// deletes the BCI object, data will be kept until BCI started again
	void slotStop();
	// called whenever new data is avaliable. it will be added to the buffer
	// and the signal processing is done when enough data is avaliable
	void slotDataReady();
	void slotViewReady()
	{
		m_view_ready = true;
	}

private: 
	// === helper functions ===
	// clears all data & resets state (called upon new BCI connection)
	void initData();
	// a function to call the other filtering functions in order
	void runFilters();
	// gets data from the BCI
	void extractData();
	// updates properties for STFT, Tr, BrainMap (based on user input)
	void updateProperties();
	// applies custom & channel spatial filters
	void spatialFilers();
	// applies custom, channel & raw Fourier transform
	void freqTransforms();
	// applies blur/sharpen filters (to make STFT clearer)
	void imageProcessing();
	// calculates the control signals from filtered data 
	void translation();
	void exportTimePlot();
	void exportFreqPlot();
	void exportSTFTPlot();
	void exportBallTest();
	void exportTrPlot();
	void exportBrainMap();
	// exports all shared data produced by controller
	void exportAll();
	// clears data that is no longer needed
	void removeOldData();

protected:
	// shown in status bar
	Timer m_runTimer;
	// used to requesting view update
	Timer m_viewTimer;
	// used for calculating CPU usage
	Timer m_calcTimer;
	// m_bci set to the correct version in slotStart
	bci::Interface* m_bci{ nullptr };

private:
	// === parameters used for the algs ===
	// stores the latest copy from MainWindow
	DTFTproperties m_dtft;
	// stores the latest copy from MainWindow
	TrProperties m_trans;
	// parameters for two spatial filters (for each channel)
	// >0 means add ; <0 means subtract ; =0 means ignore
	SpfilProperties m_spatial;

private:
	struct Data
	{
		// === Raw ===
		// dim1 = ch ; dim2 = time
		std::vector<std::vector<double>> rawTD;

		// === spatial Filtered (in time-domain) ===
		// dim1 = left/right ; dim2 = time
		std::vector<std::vector<double>> spfilTD;
		// dim1 = channel ; dim2 = time
		std::vector<std::vector<double>> chCAR_TD;

		// === Freq Domain ===
		// dim1 = channel ; dim2 = freq
		std::vector<std::vector<double>> rawFD;
		// dim1 = left/right ; dim2 = freq
		std::vector<std::vector<double>> spfilFD;
		// dim1 = left/right ; dim2 = num ; dim3 = freq
		std::vector<std::vector<std::vector<double>>> imgStoreFD;
		// dim1 = left/right ; dim2 = freq
		std::vector<std::vector<double>> imgOutputFD;
		// dim1 = channel ; dim2 = freq
		std::vector<std::vector<double>> chCAR_FD;

		// === Translation ===
		// dim1 = fspread ; dim2 = left/right
		std::vector<std::vector<double>> spfilTR;
		// dim = channel
		std::vector<double> chCAR_TR;
	};
	Data m_data;
	double latestTime{ 0 }; // seconds

private:
	bool m_view_ready{ true };
	ControllerState* shared{ &ControllerState::state };
};

