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

