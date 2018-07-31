/*****************************************************************************************
*    File: BCI_Interface.h                                                               *
*                                                                                        *
*    DetailedException class - Used for handling errors and recording where the error    *
*    occurred. Should only catch problems which happen infrequently, otherwise it will   *
*    cause large performance issues                                                      *
*                                                                                        *
*    Author: A.S. Woodcock                                                               *
*****************************************************************************************/

#pragma once

#include "resources.h"
#include <ostream>

class QThread;

namespace bci {

class Interface : public QObject
{
	Q_OBJECT

public:
	const bool connected() const { return m_connected; }
	const size_t numChannels();
	// acquire resources + connect to device + start stream
	virtual void start() { emit sigCallStartHelper(); }
	// free resources + disconnect
	virtual void stop() { emit sigCallStopHelper(); };
	// copy data from channels vector
	void getData(std::vector<double>& rx);
	// copy gyro data
	void getGyroXYZ(double& gX, double& gY, double& gZ);
	// copy electrode impedances
	void getElecImpedance(std::vector<double>& rx);
	// need to re-implement this
	Interface();
	virtual ~Interface() = default;

signals:
	void sigCallInit();
	//void sigCallExit();
	// emitted when data avaliable - need to keep up
	void sigDataReady();
	void sigCallStartHelper();
	void sigCallStopHelper();

protected slots:
	// use this to set up class in BCI Thread
	virtual void init() {};
	virtual void start_helper() {};
	virtual void stop_helper() {};

protected:
	// list of channels - can be resized as necessary
	std::vector<double> m_channel;
	// gyro XYZ data - size of 0 means not supported
	std::vector<double> m_gyro;
	// electrode impedance - size of 0 means not supported
	std::vector<double> m_elec_imp;

protected:
	// we need to ensure events go to new thread
	// to avoid other stuff slowing it down
	QThread* m_bci_thread{ nullptr };
	std::atomic<bool> m_connected{ false };
	std::atomic<bool> m_data_copied{ true };
	std::mutex m_ch_mtx;
	std::mutex m_gyro_mtx;
	std::mutex m_imp_mtx;
};

}