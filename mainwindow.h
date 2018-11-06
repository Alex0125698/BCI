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
#include "error.h"
#include "mainwindowstate.h"
#include <QMainWindow>

// forward declerations
class QLabel;
class Core;
class DebugWindow;
class QProgressBar;
namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	struct StatusBar
	{
		QLabel* offlineProgressText;
		QProgressBar* offlineProgress;
		QLabel* cpuUsagePercentText;
		QProgressBar* cpuUsagePercent;
		QLabel* timeText;
		QLabel* timeLabel;
		QLabel* runningLabel;
		QLabel* savingLabel;	
		void slotViewUpdate();
	};

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
	void closeEvent(QCloseEvent* event);

signals:
	void sigRunController(int mode, QString serialPort, QString file, uint32_t freq);
	void sigStopController();
	void sigSendInFileName(QString file);
	void sigSendOutFileName(QString file);
	void sigSaveStateChanged(bool saving);
	void sigSourceChanged(QString mode);
	void sigViewReady();

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
	void on_btn_stft_larger_toggled(bool checked);
	void on_box_wnd_overlap_valueChanged(int arg1);
	void on_slider_sharpen_size_valueChanged(int value);
	void on_slider_sharpen_amount_valueChanged(int value);
	void on_slider_blur_size_valueChanged(int value);
	void on_slider_blur_amount_valueChanged(int value);
	void on_box_wnd_type_currentIndexChanged(const QString & arg1);
	void on_box_wnd_size_editingFinished();
	void on_btn_stft_enabled_toggled(bool checked);
	void on_tabWidget_currentChanged(int index);
	void on_btn_data_in_clicked();
	void on_btn_debug_window_toggled(bool checked);
	void on_box_time_span_valueChanged(int arg1);
	void on_slider_max_freq_valueChanged(int value);
	void on_slider_brightness_valueChanged(int value);
	void on_slider_dark_boost_valueChanged(int value);
	void on_slider_hard_limit_valueChanged(int value);
	void on_slider_tr_span_valueChanged(int value);
	void on_slider_tr_gain_valueChanged(int value);
	void on_box_tr_centre_freq_valueChanged(int arg1);
	void on_slider_tr_offset_valueChanged(int value);
	void on_btn_tr_larger_toggled(bool checked);
	void on_box_tr_span_valueChanged(int arg1);
	void on_box_tr_spreading_valueChanged(int arg1);
	void on_box_tr_flip_toggled(bool checked);
	void on_slider_tr_sensitivity_valueChanged(int value);
	void on_slider_tr_smoothing_valueChanged(int value);
	void on_btn_tr_auto_gain_clicked();
	void on_btn_tr_auto_offset_clicked();
	void on_slider_tr_off_speed_valueChanged(int value);
	void on_slider_tr_off_limit_valueChanged(int value);
	void on_slider_tr_mag_speed_valueChanged(int value);
	void on_slider_tr_mag_limit_valueChanged(int value);

private:
	MainWindowState* state{ &MainWindowState::state };
	Ui::MainWindow* ui;
	// this will be used for displaying debug messages
	DebugWindow* m_debug_window;
	// these are displayed in the status bar
	QLabel* m_lbl_ioratename;
	QLabel* m_lbl_iorate;
	QLabel* m_lbl_refreshratename;
	QLabel* m_lbl_refreshrate;
	QThread* m_coreThread;
	Core* m_core;
	StatusBar m_bar;
};
