#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "resources.h"

class QLabel;
class QPlainTextEdit;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_btn_connect_toggled(bool checked);

public slots:
	void processDebugMessage(QString msg, QString file, int line, int type);
																			  

private:
	Ui::MainWindow *ui;

	// this will be used for displaying debug messages
	QPlainTextEdit* m_debug_window;
	// these are displayed in the status bar
	QLabel* m_lbl_ioratename;
	QLabel* m_lbl_iorate;
	QLabel* m_lbl_refreshratename;
	QLabel* m_lbl_refreshrate;
	QThread* m_coreThread;
	//core* m_core;
};

#endif // MAINWINDOW_H
