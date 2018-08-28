/**
* mainwindow class - this class provides slot for all button state changes and 
* updates the graphs + labels. This does not draw most of the GUI elements. This
* is done in a ui file.
*
* Error Handling - not done
*
*
* Author:  A.S. Woodcock
* Project: BCI Controller for Biomedical Applications
*/

#pragma once

#include "resources.h"
#include <QMainWindow>

class QLabel;
class QPlainTextEdit;
class Core;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

signals:
	void sigRunController();
	void sigStopController();
	void sigSendInFileName(QString file);
	void sigSendOutFileName(QString file);
	void sigSaveStateChanged(bool saving);
	void sigSourceChanged(QString mode);

public slots:
	// add message to debug window
	void slotDebugMessage(QString msg, QString file, int line, int type);
	// update var labels + graphs
	void slotViewUpdate();
	// called controller starts/stops
	void slotRunStateChanged(bool running);
	// called when save starts/stops
	void slotSaveStateChanged(bool saving);

private slots:
	void on_btn_connect_clicked(bool checked);
	void on_btn_uoa_clicked();
	void on_btn_out_file_clicked();
	void on_btn_save_options_clicked();
	void on_btn_save_start_clicked();
	void on_btn_save_stop_clicked();
	void on_box_source_currentIndexChanged(const QString &arg1);

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
	Core* m_core;
};
