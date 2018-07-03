#pragma once

#include <QObject>

class Controller;
class QThread;
class MainWindow;

class Core : public QObject
{
	Q_OBJECT

public:
	Core(MainWindow* wnd);
	~Core();

signals:
	void runController();

private:
	Controller* m_controller{ nullptr };
	QThread* m_io_thread{ nullptr };

public slots:
	void changeControllerState(bool on);
};

