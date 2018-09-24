#include "resources.h"
#include "debugwindow.h"

DebugWindow::DebugWindow(QWidget* parent)
	: QPlainTextEdit(parent)
{
	this->setReadOnly(true);
	//this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
	this->setWindowTitle("Debug Window");
	this->setMinimumSize(350, 200);
}

void DebugWindow::closeEvent(QCloseEvent* event)
{
	if (!m_closable)
	{
		event->ignore();
		emit sigHideWindow();
	}
}

void DebugWindow::closeWindow()
{
	m_closable = true;
	close();
}
