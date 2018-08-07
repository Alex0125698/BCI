#include "bciinterface.h"
#include <QThread>

 void bci::Interface::getData(std::vector<double>& rx)
 {
	 assert(rx.size() == maxChannels());
	 std::lock_guard<std::mutex> lock(m_ch_mtx);

	 if (m_channel.empty())
		 emit sigError("getData() attempted but queue is empty");
	 else
	 {
		 std::copy(m_channel.front().begin(), m_channel.front().end(), rx.begin());
		 m_channel.pop();
	 }
 }

 void bci::Interface::getGyroXYZ(double& gX, double& gY, double& gZ)
 {
	 //TODO: assert(rx.size() <= maxChannels());
	 std::lock_guard<std::mutex> lock(m_gyro_mtx);

	 if (m_gyro.empty())
		 emit sigError("getGyroXYZ() attempted but queue is empty");
	 else
	 {
		 assert(m_gyro.front().size() == 3);
		 gX = m_gyro.front()[0];
		 gY = m_gyro.front()[1];
		 gZ = m_gyro.front()[2];
		 m_gyro.pop();
	 }
 }

 void bci::Interface::getElecImpedance(std::vector<double>& rx)
 {
	 assert(rx.size() == maxChannels());
	 std::lock_guard<std::mutex> lock(m_imp_mtx);

	 if (m_elec_imp.empty())
		 emit sigError("getElecImpedance() attempted but queue is empty");
	 else
	 {
		 std::copy(m_elec_imp.front().begin(), m_elec_imp.front().end(), rx.begin());
		 m_elec_imp.pop();
	 }
 }

 bci::Interface::Interface()
 {
	 m_bci_thread = new QThread;
	 this->moveToThread(m_bci_thread);
	 m_bci_thread->setObjectName("BCI Thread");
	 m_bci_thread->start(QThread::HighestPriority);

	 // Boilerplate code to create resources in BCI Thread
	 connect(this, &bci::Interface::sigCallInit, this, &bci::Interface::init, Qt::QueuedConnection);
	 connect(this, &bci::Interface::sigCallStartHelper, this, &bci::Interface::start_helper, Qt::QueuedConnection);
	 connect(this, &bci::Interface::sigCallStopHelper, this, &bci::Interface::stop_helper, Qt::QueuedConnection);
	 emit sigCallInit();
 }
