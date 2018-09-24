#include "resources.h"
#include "core.h"
#include "controller.h"
#include "mainwindow.h"
#include "bciinterface.h"

#include <QThread>

Core::Core(MainWindow* wnd)
{
	// ====== Setup Threads ======

	m_io_thread = new QThread;

	m_io_thread->setObjectName("I/O Thread");
	m_controller = new Controller;
	m_controller->moveToThread(m_io_thread);
	m_io_thread->start(QThread::HighestPriority);

	//DEBUG_PRINTLN(m_io_thread->currentThreadId());

	// ====== Connections ======
	connect(wnd, &MainWindow::sigRunController, m_controller, &Controller::slotStart, Qt::QueuedConnection);
	connect(wnd, &MainWindow::sigStopController, m_controller, &Controller::slotStop, Qt::QueuedConnection);
	connect(m_controller, &Controller::sigRunStateChanged, wnd, &MainWindow::slotRunStateChanged, Qt::QueuedConnection);
	connect(m_controller, &Controller::sigSaveStateChanged, wnd, &MainWindow::slotSaveStateChanged, Qt::QueuedConnection);
}

Core::~Core()
{
	if (m_io_thread)
	{
		m_io_thread->quit();
		m_io_thread->wait();
		m_io_thread->deleteLater();
		m_io_thread = nullptr;
	}
	if (m_controller)
	{
		delete m_controller;
		m_controller = nullptr;
	}
	
}
