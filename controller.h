#pragma once

#include <QObject>
#include <memory>

namespace bci {
	class Interface;
}

class Controller :public QObject
{
	Q_OBJECT

public:
	Controller();

signals:
	void requestViewUpdate();

public slots:
	void mainwindowIsReady();
	void slotRun();
	void stop();

protected:
	bool m_mainwindow_ready{ true };

private:
	std::unique_ptr<bci::Interface> m_bci;
	bool m_running{ false };

	friend class Core;
};

