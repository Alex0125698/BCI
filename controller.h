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

#include "mainwindowstate.h"
#include "controllerstate.h"

// forward declarations
namespace bci { class Interface; }

class Controller : public QObject
{
	Q_OBJECT

public:
	enum BCISource
	{
		Offline,
		OpenBCI,
		Emotiv
	};
	Controller();
	~Controller();
	
signals:
	// feedBack for mainwindow
	void sigRunStateChanged(bool running);
	// feedBack for mainwindow
	void sigSaveStateChanged(bool saving);
	// tell mainwindow to redraw graphs + labels
	void sigViewRefresh();

public slots:
	// creates the BCI object, data aquisition starts auomatically
	void slotStart(int source, QString file, uint32_t freq);
	// deletes the BCI object, data will be kept until BCI started again
	void slotStop();
	// called whenever new data is avaliable. it will be added to the buffer
	// and the signal processing is done when enough data is avaliable
	void slotDataReady();

	
private: 
	// === helper functions ===
	// clears all data & resets state
	void resetData();
	// updates STFT properties such as window size
	void checkDTFTproperties();
	// gets data from the BCI
	void extractData();
	// applies Fourier transform
	void freqTransform();
	// applies spatial filters
	void laplaceFilter();
	// applies blur/sharpen filters (to make STFT clearer)
	void imageProcessing();
	// calculates the control signal from filtered data 
	void translation();
	// clears data that is no longer needed
	void clearOldData();	
	// update shared data
	void exportData();
	
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
	// stores the latest copy from MainWindow
	DTFTproperties m_dtft;
	// stores the latest copy from MainWindow
	TrProperties m_trans;
	// may need to redo all blur+sharpen filtering
	bool m_recalc_img_filter{ true };
	// stores the current index inside of m_dataFD
	// note: there are less timepoints in the STFT than the raw data
	int64_t m_timeIndex{ -1 };

private:
	// the BCI buffered data
	// time-domain ; dim1 = channel ; dim2 = time
	std::vector<std::vector<double>> m_dataTD;
	// freq domain ; dim1 = channel  ; dim2 = time ; dim3 = freq (0-wndSize)
	// we need multiple timepoints to do image processing
	std::vector<std::deque<std::vector<double>>> m_dataFD;
	// laplacian filtered data - no image processing applied yet
	// we need multiple timepoints to do image processing
	// dim1 = time ; dim2 = freq
	std::deque<std::vector<double>> m_laplaceData;
	// final data 
	// dim = freq
	std::vector<double> m_finalData;
	// the lastest control data point
	double m_CARcontrolData{ 0 };

private:
	ControllerState* state{ &ControllerState::state };
};

