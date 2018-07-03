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

	//connect(this, &Core::runController, m_controller, &Controller::run, Qt::QueuedConnection);
	//connect(wnd, &MainWindow::mainwindowReady, m_controller, &Controller::mainwindowIsReady, Qt::QueuedConnection);
	//connect(m_controller, &Controller::requestViewUpdate, wnd, &MainWindow::processViewUpdate, Qt::QueuedConnection);
}

Core::~Core()
{
	delete m_controller;
	delete m_io_thread;
}

void Core::changeControllerState(bool on)
{
	if (on)
	{
		//emit runController();
	}
	else
	{
		//m_controller->stop();
	}
}
