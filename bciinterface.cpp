#include "bciinterface.h"
#include <QThread>

 const size_t bci::Interface::numChannels()
 { 
	 std::lock_guard<std::mutex> lock(m_ch_mtx); return m_channel.size(); 
 }

 void bci::Interface::getData(std::vector<double>& rx)
 {
	 assert(rx.size() == m_channel.size());

	 std::lock_guard<std::mutex> lock(m_ch_mtx);
	 for (size_t i = 0; i < m_channel.size(); ++i)
		 rx[i] = m_channel[i];
	 m_data_copied = true;
 }

 void bci::Interface::getGyroXYZ(double & gX, double & gY, double & gZ)
 {
	 assert(m_gyro.size() == 3);

	 std::lock_guard<std::mutex> lock(m_gyro_mtx);
	 gX = m_gyro[0];
	 gY = m_gyro[1];
	 gZ = m_gyro[2];
 }

 void bci::Interface::getElecImpedance(std::vector<double>& rx)
 {
	 assert(rx.size() == m_elec_imp.size());

	 std::lock_guard<std::mutex> lock(m_imp_mtx);
	 for (size_t i = 0; i < m_elec_imp.size(); ++i)
		 rx[i] = m_elec_imp[i];
 }

 bci::Interface::Interface()
 {
	 m_bci_thread = new QThread;
	 this->moveToThread(m_bci_thread);
	 m_bci_thread->setObjectName("BCI Thread");
	 m_bci_thread->start(QThread::HighestPriority);

	 connect(this, &bci::Interface::sigCallInit, this, &bci::Interface::init, Qt::QueuedConnection);
	 connect(this, &bci::Interface::sigCallStartHelper, this, &bci::Interface::start_helper, Qt::QueuedConnection);
	 connect(this, &bci::Interface::sigCallStopHelper, this, &bci::Interface::stop_helper, Qt::QueuedConnection);
	 emit sigCallInit();
 }
