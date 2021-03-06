/**
* DebugWindow class - Used to display error messages in 
* a separate window. Will be managed by MainWindow
*
*
*/

#pragma once

#include "resources.h"
#include "error.h"
#include <QPlainTextEdit>

class DebugWindow : public QPlainTextEdit
{
	Q_OBJECT

public:
	DebugWindow(QWidget* parent = nullptr);
	void closeEvent(QCloseEvent* event) override;
	void closeWindow();

signals:
	void sigHideWindow();

private:
	bool m_closable{ false };
};

