/**
* Controller class - This is the only class that owns the BCI objects.
* So far there are 3 BCI sources:
* (1) offline
* (2) OpenBCI
* (3) Emotiv
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
#include "mainwindowstate.h"
#include "controllerstate.h"
#include "state.h"

namespace bci {
	class Interface;
}

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
	void resetData();
	void extractData();
	void check_dtft_properties();
	void freqTransform();
	void clearOldData();
	void laplaceFilter();
	void imageProcessing();
	void exportData();


protected:
	Timer m_run_timer;
	Timer m_io_time;
	Timer m_calc_time;
	// the m_bci object is managed manually
	// it will be replaced whenever slotStart is called;
	bci::Interface* m_bci{ nullptr };
	DTFTproperties m_dtft_properties;
	// may need to redo all blur+sharpen filtering
	bool m_recalc_img_filter{ false };
	// need to throw away old data if window size changes
	bool m_wnd_changed{ false };

protected:
	int64_t m_timeIndex{ -1 };
	// the BCI buffered data
	// time-domain ; dim1 = channel ; dim2 = time
	std::vector<std::vector<double>> m_dataTD;
	// freq domain ; dim1 = channel  ; dim2 = time ; dim3 = freq (0-DFTsize/2)
	std::vector<std::deque<std::vector<double>>> m_dataFD;
	// DFT processing
	uint32_t DFTsize{ 128 };
	uint32_t DFToverlap{ 128 };
	DFTwindow DFTwnd{ DFTwindow::GAUSSIAN };
	// blur + sharpen filers

	// laplacian parameters

	// laplacian filtered data - no image processing applied yet
	// dim1 = time ; dim2 = freq
	std::deque<std::vector<double>> m_laplaceData;
	// final data 
	// dim1 = time ; dim2 = freq
	std::vector<std::vector<double>> m_finalData;

private:
	ControllerState* state{ &ControllerState::state };
};

