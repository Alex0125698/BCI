#pragma once

#include "resources.h"

namespace bci {
	class Interface;
}

class Controller :public QObject
{
	Q_OBJECT

public:
	Controller();

signals:
	void sigViewUpdate();
	void sigGraphUpdate();
	void sigRunStateChanged(bool running);
	void sigSaveStateChanged(bool saving);

public slots:
	void slotRun();
	void stop();

protected:
	bool m_view_ready{ true };

private:
	std::unique_ptr<bci::Interface> m_bci;
	bool m_running{ false };

	friend class Core;
};

