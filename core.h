/**
* Core class - This class sets up the Controller and provides a 
* communication channel between the controller and mainwindow.
* 
* Error Handling - no errors
*
* Author:  A.S. Woodcock
* Project: BCI Controller for Biomedical Applications
* Last Modified: 
*/

#pragma once

#include "resources.h"

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

