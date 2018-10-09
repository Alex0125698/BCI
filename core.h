/**
* Core class - This class sets up the Controller and provides a 
* communication channel between the controller and mainwindow.
* This allows the controller to respond certain button presses.
* 
* Error Handling - no errors
*
* Author:  A.S. Woodcock
* Project: BCI Controller for Biomedical Applications
*/

#pragma once

#include "resources.h"
#include "error.h"

// forward declerations
class Controller;
class QThread;
class MainWindow;

class Core : public QObject
{
	Q_OBJECT

public:
	Core(MainWindow* wnd);
	~Core();

private:
	Controller* m_controller{ nullptr };
	QThread* m_io_thread{ nullptr };
};

