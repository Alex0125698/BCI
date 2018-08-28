/**
 * bci::Interface calss - this class provdes a generic interface for all BCIs or  
 * simulator. It gives access to channel data, channel positions, gyro data, accelerometer
 * data and channel impedance (if avaliable).
 *
 * Error Handling - must connect to sigError and print out error messages. the bci::interface
 * will stop if there is an error, so the controller should update its state appropriately
 *
 * Author:  A.S. Woodcock
 * Project: BCI Controller for Biomedical Applications
 */

#pragma once

#include "resources.h"
#include <ostream>

#define SIGERROR(x) sigError(x,__FILE__, __LINE__)

class QThread;

namespace bci {

class Interface : public QObject
{
	Q_OBJECT

public:
	Interface();
	virtual ~Interface() = default;
	const bool connected() const { return m_connected; }
	virtual const size_t maxChannels()=0;
	// use this to connect to BCI
	virtual void start() { emit sigCallStartHelper(); }
	// use this to disconnect BCI
	virtual void stop() { emit sigCallStopHelper(); };

public:
	// use these to get the data
	void getData(std::vector<double>& rx);
	void getGyroXYZ(double& gX, double& gY, double& gZ);
	void getElecImpedance(std::vector<double>& rx);

signals:
	// emitted when data avaliable - need to keep up
	void sigDataReady();
	// must connect to this and stop when there is an error
	void sigError(QString msg, QString file, int line);
	// boilerplate code to call functions in the BCI Thread
	void sigCallInit();
	void sigCallStartHelper();
	void sigCallStopHelper();

protected slots:
	// this is a "constructor" but done in BCI Thread
	virtual void init() {};
	// TODO: destructor
	// connect to BCI in BCI Thread
	virtual void start_helper() {};
	// disconnect BCI in BCI thread
	virtual void stop_helper() {};

protected:
	// raw EEG data in volts
	std::queue<std::vector<double>> m_channel;
	// gyro XYZ data - size of 0 means not supported
	std::queue<std::vector<double>> m_gyro;
	// electrode impedance - size of 0 means not supported
	std::queue<std::vector<double>> m_elec_imp;
	// protect access to these queues
	std::mutex m_ch_mtx;
	std::mutex m_gyro_mtx;
	std::mutex m_imp_mtx;

protected:
	// we need to ensure events go to new thread
	// to avoid other stuff slowing it down
	QThread* m_bci_thread{ nullptr };
	// are we corrently connected to the BCI
	std::atomic<bool> m_connected{ false };
};

}