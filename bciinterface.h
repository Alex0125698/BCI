/**
 * bci::Interface class - this abstract class provdes a generic interface for all BCIs or  
 * simulators. It gives access to channel data, channel positions, gyro data, accelerometer
 * data and channel impedance (if avaliable).
 *
 * Error Handling - must connect to sigError and print out error messages. the bci::interface
 * will stop if there is an error, so the controller should update its state appropriately. 
 * Then you can call start() to recieve BCI data again.
 * 
 * Author:  A.S. Woodcock
 * Project: BCI Controller for Biomedical Applications
 */

#pragma once

#include "resources.h"
#include "error.h"
#include <ostream>

/**
 * All public functions are thread-safe (but indended to be used
 * in a single consumer thread).
 * 
 * All private functions need to be called in the BCI thread.
 * The public functions will add an event to a queue which will
 * call a similarly named helper function in the BCI thread.
 */

class QThread;

namespace bci {

class Interface : public QObject
{
	Q_OBJECT

public:
	// sets up new thread for the BCI's event loop
	Interface();
	// all base classes must have a virtual destructor to avoid leaks
	virtual ~Interface() = 0;
	// use this to connect to BCI & start data aquisition
	virtual void start() { m_connected = true; m_time = 0; emit sigCallStartHelper(); }
	// use this to disconnect the BCI
	virtual void stop() { m_connected = false;  emit sigCallStopHelper(); };

public:
	// are we currentlly connected and getting data?
	const bool isConnected() const { return m_connected; }
	// are we currentlly connected and getting data?
	const bool ready() const { return m_stats_avaliable; }
	// number of data channels (doesn't include references)
	virtual const size_t numChannels() = 0;
	// the frequency at which packets arrive
	virtual const double freq() = 0;
	// use these to get the least-recent data (it is released from the queue)
	void getData(std::vector<double>& rx, double& time);
	void getGyroXYZ(double& gX, double& gY, double& gZ);
	void getElecImpedance(std::vector<double>& rx);

signals:
	// data emitted whenever avaliable - need to keep up
	void sigDataReady();
	// must connect to this - the BCI will stop when there is an error
	void sigError(QString msg, QString file, int line);
	// boilerplate code to call functions in the BCI Thread
	void sigCallInit();
	void sigCallStartHelper();
	void sigCallStopHelper();

protected slots:
	// this is a "constructor" but done in BCI Thread
	virtual void init() {};
	// connect to BCI in BCI Thread
	virtual void start_helper() {};
	// disconnect BCI in BCI thread
	virtual void stop_helper() {};

protected:
	// all data is owned by BCI thread
	// raw EEG data in volts
	std::queue<std::vector<double>> m_channel;
	// gyro XYZ data - size of 0 means not supported
	std::queue<std::vector<double>> m_gyro;
	// electrode impedance - size of 0 means not supported
	std::queue<std::vector<double>> m_elec_imp;
	// protect access to these queues
	// a mutex offers much better performance than atomics
	std::mutex m_ch_mtx;
	std::mutex m_gyro_mtx;
	std::mutex m_imp_mtx;

protected:
	// we need to ensure events go to new thread
	// to avoid other stuff slowing it down
	QThread* m_bci_thread{ nullptr };
	// are we corrently connected to the BCI
	std::atomic<bool> m_connected{ false };
	// is the number of channels avaliable to read yet
	std::atomic<bool> m_stats_avaliable{ false };
	// the total time since start of data aquisition
	double m_time{ 0 };


};

}