#pragma once

#include <QObject>

namespace bci {
	class BCI_Interface;
}

class Controller :public QObject
{
	Q_OBJECT

public:
	Controller();
	~Controller();

public slots:
	void mainwindowIsReady();

protected:
	bool m_mainwindow_ready{ true };

signals:
	void requestViewUpdate();

public slots:
	void run();
	void stop();

private:
	bci::BCI_Interface* m_bci_hardware;
	bool m_running{ false };

	friend class Core;
};

