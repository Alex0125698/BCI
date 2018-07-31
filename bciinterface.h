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

namespace bci {

class Interface : public QObject
{
	Q_OBJECT

public:
	const bool connected() const { return m_connected; }
	const size_t numChannels();
	// acquire resources + connect to device + start stream
	virtual void start() = 0;
	// free resources + disconnect
	virtual void stop() = 0;
	// copy data from channels vector
	void getData(std::vector<double>& rx);
	// copy gyro data
	void getGyroXYZ(double& gX, double& gY, double& gZ);
	// copy electrode impedances
	void getElecImpedance(std::vector<double>& rx);
	// need to re-implement this
	virtual ~Interface() = default;

signals:
	// emitted when data avaliable - need to keep up
	void dataReady();

protected:
	// list of channels - can be resized as necessary
	std::vector<double> m_channel;
	// gyro XYZ data - size of 0 means not supported
	std::vector<double> m_gyro;
	// electrode impedance - size of 0 means not supported
	std::vector<double> m_elec_imp;

protected:
	std::atomic<bool> m_connected{ false };
	std::atomic<bool> m_data_copied{ true };
	std::mutex m_ch_mtx;
	std::mutex m_gyro_mtx;
	std::mutex m_imp_mtx;
};

}