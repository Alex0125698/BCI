#include "resources.h"
#include "bciinterface.h"

// automatically add line & file macros
#define SIGERROR(x) sigError(x,__FILE__,__LINE__)

bci::Interface::~Interface()
{
	if (m_bci_thread)
	{
		// TODO: is this done right ???
		m_bci_thread->deleteLater();
		m_bci_thread->quit();
		auto finished = m_bci_thread->wait(40);
		if (!finished)
		{
			m_bci_thread->terminate();
			DEBUG_PRINTLNY("BCI Thread terminated", MSG_TYPE::WARNING);
		}
		else
		{
			DEBUG_PRINTLN("BCI Thread finished");
		}
		
		m_bci_thread = nullptr;
	}
}

void bci::Interface::getData(std::vector<double>& rx, double& time)
 {
	 sassert(rx.size() == numChannels());
	 sassert(freq() != 0.0);

	 m_time += 1.0 / freq();
	 time = m_time;

	 // lock guard will always unlock the mutex when we are done
	 std::lock_guard<std::mutex> lock(m_ch_mtx);

	 if (m_channel.empty())
		 emit SIGERROR("getData() attempted but queue is empty");
	 else
	 {
		 // fill in rx with least-recent data packet
		 std::copy(m_channel.front().begin(), m_channel.front().end(), rx.begin());
		 m_channel.pop();
	 }
 }

 void bci::Interface::getGyroXYZ(double& gX, double& gY, double& gZ)
 {
	 //TODO: sassert(rx.size() <= numChannels());
	 std::lock_guard<std::mutex> lock(m_gyro_mtx);

	 if (m_gyro.empty())
		 emit SIGERROR("getGyroXYZ() attempted but queue is empty");
	 else
	 {
		 sassert(m_gyro.front().size() == 3);
		 gX = m_gyro.front()[0];
		 gY = m_gyro.front()[1];
		 gZ = m_gyro.front()[2];
		 m_gyro.pop();
	 }
 }

 void bci::Interface::getElecImpedance(std::vector<double>& rx)
 {
	 std::lock_guard<std::mutex> lock(m_imp_mtx);
	 sassert(rx.size() == m_elec_imp.size());
	 
	 if (m_elec_imp.empty())
		 emit SIGERROR("getElecImpedance() attempted but queue is empty");
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

	 // TODO: use QMetaObject::invokeMethod instead
	 // Boilerplate code to create resources in BCI Thread
	 connect(this, &bci::Interface::sigCallInit, this, &bci::Interface::init, Qt::QueuedConnection);
	 connect(this, &bci::Interface::sigCallStartHelper, this, &bci::Interface::start_helper, Qt::QueuedConnection);
	 connect(this, &bci::Interface::sigCallStopHelper, this, &bci::Interface::stop_helper, Qt::QueuedConnection);
	 emit sigCallInit();
 }
