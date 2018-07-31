#include "core.h"
#include "controller.h"
#include "mainwindow.h"
#include "bciinterface.h"

#include <QThread>

Core::Core(MainWindow* wnd)
{
	// ====== Setup Threads ======

	m_io_thread = new QThread;
	m_controller = new Controller;
	m_controller->moveToThread(m_io_thread);
	m_io_thread->start();


	// ====== Connections ======

	connect(wnd, &MainWindow::sigChangeControllerState, this, &Core::slotChangeControllerState, Qt::QueuedConnection);
	connect(this, &Core::sigRun, m_controller, &Controller::slotRun, Qt::QueuedConnection);
	connect(m_controller, &Controller::sigViewUpdate, wnd, &MainWindow::slotViewUpdate, Qt::QueuedConnection);
	connect(m_controller, &Controller::sigGraphUpdate, wnd, &MainWindow::slotGraphUpdate, Qt::QueuedConnection);
	connect(m_controller, &Controller::sigRunStateChanged, wnd, &MainWindow::slotRunStateChanged, Qt::QueuedConnection);
	connect(m_controller, &Controller::sigSaveStateChanged, wnd, &MainWindow::slotSaveStateChanged, Qt::QueuedConnection);
}

Core::~Core()
{
	delete m_controller;
	delete m_io_thread;
}

void Core::slotChangeControllerState(bool on)
{
	if (m_controller->m_running) return;
	m_controller->m_running = true;

	if (on)
	{
		emit sigRun();
	}
	else
		m_controller->m_running = false;
}
