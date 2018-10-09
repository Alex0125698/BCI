#include "resources.h"
#include <QWinTaskbarButton>
#include <QtWidgets>
#include <QDesktopServices>

#include "mainwindow.h"
#include "controllerstate.h"
#include "ui_mainwindow.h"
#include "debugwindow.h"
#include "graphwidget.h"
#include "core.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	// ===== SETUP MAINWINDOW =====
	ui->setupUi(this);
	m_core = new Core(this);

	//QWinTaskbarButton *button = new QWinTaskbarButton(this);
	//button->setWindow(this->windowHandle());
	//button->setOverlayIcon(QIcon(":/icons/icons/cool arrow effect.jpg"));

	// ===== SETUP DEBUG WINDOW =====
	// it will be shown as a separate window
	m_debug_window = new DebugWindow();
	m_debug_window->show();
	connect(m_debug_window, &DebugWindow::sigHideWindow, [this]() {on_btn_debug_window_toggled(false); });
	//m_debug_window->setWindowIcon(QIcon(":/icons/icons/debug.png"));
	this->setWindowTitle("Brain-Computer Interface Controller");

	// ===== CONNECTIONS =====
	connect(&DebugOutput::getDebug(), &DebugOutput::sigDebugMessage, this, &MainWindow::slotDebugMessage, Qt::QueuedConnection);
	DEBUG_PRINTLN("Program Started");

	// ===== SET UP GRAPHS =====

	ui->plot_freq->setTitle("FFT Plot");
	ui->plot_freq->setAxisTitles("Magnitude (V)", "", "Frequency (Hz)", "");
	ui->plot_freq->setRange({ 0,128 }, { 0,1 });

	ui->plot_time->setTitle("Time-Domain Data");
	ui->plot_time->setAxisTitles("Magnitude (V)", "", "Time (h:m:s)", "");
	ui->plot_time->setRange({ 0,128 }, { 0,1 });

	ui->plot_translation->setTitle("Translation Control Signals");
	ui->plot_translation->setAxisTitles("Magnitude (V)", "", "Time (h:m:s)", "");
	ui->plot_translation->setRange({ 0,120 }, { -1,1 });
	ui->plot_translation->addGraph({ "Control", "", true});

	auto makeQLabel = [](QString text, QColor c = QColor("#701515"), QFont f = QFont("times"), int pt = 11)
	{
		text = "<html><head/><body><p><span style = \" color:" + c.name() + ";\">" + text + "</span></p></body></html>";
		f.setPointSize(pt);
		auto lbl = new QLabel(text);
		lbl->setFont(f);
		return lbl;
	};

	// ===== set up status bar =====

	m_bar.cpuUsagePercentText = makeQLabel("CPU Usage (%): ");
	m_bar.cpuUsagePercent = new QProgressBar;
	m_bar.offlineProgressText = makeQLabel("Progress: ");
	m_bar.offlineProgressText->setVisible(false);
	m_bar.offlineProgress = new QProgressBar;
	m_bar.offlineProgress->setVisible(false);
	m_bar.runningLabel = makeQLabel("Not Running");
	m_bar.savingLabel = makeQLabel("Not Saving");
	m_bar.savingLabel->setVisible(false);
	m_bar.timeLabel = makeQLabel("0", { 0,0,0 });
	m_bar.timeText = makeQLabel("Run Time (s): ");

	ui->statusbar->addPermanentWidget(m_bar.cpuUsagePercentText);
	ui->statusbar->addPermanentWidget(m_bar.cpuUsagePercent);
	ui->statusbar->addPermanentWidget(m_bar.offlineProgressText);
	ui->statusbar->addPermanentWidget(m_bar.offlineProgress);
	ui->statusbar->addPermanentWidget(m_bar.runningLabel);
	ui->statusbar->addPermanentWidget(m_bar.savingLabel);
	ui->statusbar->addPermanentWidget(m_bar.timeText);
	ui->statusbar->addPermanentWidget(m_bar.timeLabel);
}

MainWindow::~MainWindow()
{
	
	if (m_core)
	{
		delete m_core;
		m_core = nullptr;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	
	delete ui;
	//sassert(ui->openGLWidget_2 == nullptr);

	if (m_debug_window)
	{
		delete m_debug_window;
		m_debug_window = nullptr;
	}
	// TODO: wait for run thread
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

// NEARLY DONE
void MainWindow::closeEvent(QCloseEvent* event)
{
	on_btn_save_stop_clicked();
	on_btn_connect_clicked(false);

	if (m_debug_window)
		m_debug_window->closeWindow();

	QMainWindow::closeEvent(event);
}

void MainWindow::slotDebugMessage(QString msg, QString file, int line, int type)
{
	if (msg == "clear")
	{
		m_debug_window->clear();
		return;
	}

	if (type > MSG_TYPE::WARNING)
	{
		QApplication::beep();
		m_debug_window->activateWindow();
	}

	auto setcolor = [&](QString c)
	{
		return "<p><span style=\"color:#" + c + ";font-size:13px\">" + msg + "</span></p>";
	};

	switch (type)
	{
	case MSG_TYPE::SUCCESS:
		msg = setcolor("14c440");
		break;
	case MSG_TYPE::MESSAGE:
		msg = setcolor("303030");
		break;
	case MSG_TYPE::WARNING:
		msg = setcolor("d0c510");
		break;
	case MSG_TYPE::DEXCEP:
		msg = setcolor("d02510");
		break;
	case MSG_TYPE::STDERROR:
		msg = setcolor("d02510");
		break;
	case MSG_TYPE::FATAL:
		msg = setcolor("e01040");
		break;
	}

	// display file+line details in debug mode & errors only
#if defined(_DEBUG) || defined(DEBUG)
	if (type >= MSG_TYPE::WARNING)
		msg += QString("<p><span style=\"font-size:11px\">Thrown on Line: <span style=\"color:#02698e;\">%1</span>  File: \"<span style=\"color:#02698e;\">%2</span>\"</span></p>").arg(line).arg(file);
#endif

	m_debug_window->appendHtml(msg);
}

void MainWindow::slotViewUpdate()
{
	// update labels


	// add data to graphs

	std::vector<std::vector<double>> tmp;
	double freq;
	{
		std::lock_guard<std::mutex> lock(ControllerState::state.mtx_data);
		// copy NOT move the data
		tmp = ControllerState::state.filteredFreqs;
		freq = ControllerState::state.freq;
	}

	// size == 0 means controller is using it
	if (tmp.size() != 0)
	{
		// generate frequencies vector
		static std::vector<double> freqs;
		if (freqs.size() != tmp[0].size()) freqs.resize(tmp[0].size());

		for (size_t i = 0; i < freqs.size(); i++)
		{
			freqs[i] = (freq*i) / (double)freqs.size();
		}

		// ensure we have the right number of graphs
		if (ui->plot_freq->numGraphs() != tmp.size())
		{
			ui->plot_freq->clearGraphs();
			for (size_t i = 0; i < tmp.size(); i++)
			{
				ui->plot_freq->addGraph({ ("Channel " + QString::number(i)), " (V)", true });
			}
		}

		// replace the old graph data
		ui->plot_freq->clearData();
		for (size_t i = 0; i < tmp.size(); i++)
		{
			ui->plot_freq->addData(i, freqs, tmp[i]);
		}
	}


	// add translation data

	std::vector<double> control;
	{
		std::lock_guard<std::mutex> lock(ControllerState::state.mtx_data);
		control = std::move(ControllerState::state.controlMW);
		ControllerState::state.controlMW.clear();
	}

	static int counter = 0;

	for (size_t i = 0; i < control.size(); i++)
	{
		ui->plot_translation->addData(0, counter, control[i]);
		++counter;
	}

	// replot graphs
	if (ui->tabWidget->tabText(ui->tabWidget->currentIndex()) == "Freq Analysis")
		ui->plot_freq->replot();
	else if (ui->tabWidget->tabText(ui->tabWidget->currentIndex()) == "Time Analysis")
		ui->plot_time->replot();
	else if (ui->tabWidget->tabText(ui->tabWidget->currentIndex()) == "Translation")
		ui->plot_translation->replot();
}

void MainWindow::slotRunStateChanged(bool running)
{
	if (running)
	{
		ui->btn_connect->setIcon(QIcon(":/icons/icons/arrow_orange.jpg"));
		ui->btn_connect->setText("Disconnect");
	}
	else
	{
		if (ui->btn_connect->isChecked()) ui->btn_connect->setChecked(false);
		ui->btn_connect->setIcon(QIcon(":/icons/icons/arrow_green.jpg"));
		ui->btn_connect->setText("Connect");
	}
}

void MainWindow::slotSaveStateChanged(bool saving)
{
	if (saving)
	{
		ui->btn_save_start->setEnabled(false);
		ui->btn_save_stop->setEnabled(true);
	}
	else
	{
		ui->btn_save_start->setEnabled(true);
		ui->btn_save_stop->setEnabled(false);
	}
}

// =================================== //
// ========== Main Controls ========== //
// =================================== //

void MainWindow::on_btn_connect_clicked(bool checked)
{
	if (checked)
	{
		if (ui->box_source->currentText() == "Offline")
		{
			if (ui->line_data_in->text().isEmpty())
				on_btn_data_in_clicked();
			if (ui->line_data_in->text().isEmpty())
			{
				DEBUG_PRINTLNY("ERROR: cannot load empty file", MSG_TYPE::DEXCEP);
				ui->btn_connect->setChecked(false);
				return;
			}
		}
		emit sigRunController(ui->box_source->currentIndex(), ui->line_data_in->text(), ui->box_freq->value());
	}	
	else
		emit sigStopController();
}

void MainWindow::on_btn_uoa_clicked()
{
	QDesktopServices::openUrl(QUrl("https://www.eleceng.adelaide.edu.au/students/wiki/projects/index.php/Projects:2018s1-155_Brain_Computer_Interface_Control_for_Biomedical_Applications"));
}

// NEARLY DONE
void MainWindow::on_btn_out_file_clicked()
{

	QString full_file_path = QFileDialog::getSaveFileName(this, "Save", ui->line_out_file->text(), "BCI Data: (*.csv)");

	// TODO: check if we can open the file without creating it
	// check if we can open the file
	/*QFile file(full_file_path);
	if (!file.open(QFile::WriteOnly))
	{
		DEBUG_PRINTLNY("File cannot be opened", MSG_TYPE::DEXCEP);
	}
	else
	{*/
		ui->line_out_file->setText(full_file_path);
	//}
}

// NEW DIALOG NEEDED
void MainWindow::on_btn_save_options_clicked()
{

}

// IMPLEMENT SAVING IN CONTROLLER
void MainWindow::on_btn_save_start_clicked()
{

}

// IMPLEMENT SAVING IN CONTROLLER
void MainWindow::on_btn_save_stop_clicked()
{

}

void MainWindow::on_box_source_currentIndexChanged(const QString& arg1)
{
	// the current index is polled when we try to connect
	if (arg1 == "Offline")
		ui->widget_offline->setVisible(true);
	else
		ui->widget_offline->setVisible(false);
}

// REMOVE??
void MainWindow::on_tabWidget_currentChanged(int index)
{
	index;
	slotViewUpdate();
}

void MainWindow::on_btn_data_in_clicked()
{
	QString full_file_path = QFileDialog::getOpenFileName(this, "Load", "", "BCI Data: (*.csv)");

	if (full_file_path != "")
	{
		// check if we can open the file
		QFile file(full_file_path);
		if (!file.open(QFile::ReadOnly))
		{
			DEBUG_PRINTLNY("File cannot be opened", MSG_TYPE::DEXCEP);
		}
		else
		{
			ui->line_data_in->setText(full_file_path);
		}
	}
}

void MainWindow::on_btn_debug_window_toggled(bool checked)
{
	ui->btn_debug_window->setChecked(checked);
	if (checked)
	{
		ui->btn_debug_window->setText("Hide Debug Window");
		m_debug_window->setVisible(true);
	}
	else
	{
		ui->btn_debug_window->setText("Show Debug Window");
		m_debug_window->setVisible(false);
	}
}

// =================================== //
// ========== STFT Controls ========== //
// =================================== //

void MainWindow::on_btn_stft_larger_toggled(bool checked)
{
	ui->groupBox_stft_controls->setVisible(!checked);

	if (checked)
		ui->btn_stft_larger->setArrowType(Qt::ArrowType::UpArrow);
	else
		ui->btn_stft_larger->setArrowType(Qt::ArrowType::DownArrow);
}

void MainWindow::on_box_wnd_overlap_valueChanged(int arg1)
{
	if (arg1 >= ui->box_wnd_size->value())
	{
		arg1 = ui->box_wnd_size->value() - 1;
		ui->box_wnd_overlap->setValue(arg1);
	}
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.wndOverlap = arg1;
}

void MainWindow::on_slider_sharpen_size_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.sharpenKernelSize = value / 999.0;
}

void MainWindow::on_slider_sharpen_amount_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.sharpenAmount = value / 999.0;
}

void MainWindow::on_slider_blur_size_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.blurKernelSize = value / 999.0;
}

void MainWindow::on_slider_blur_amount_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.blurAmount = value/999.0;
}

void MainWindow::on_box_wnd_type_currentIndexChanged(const QString& arg1)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	if (arg1 == "Gaussian")
		state->dtft.wndType = DFTwindow::GAUSSIAN;
	else
		state->dtft.wndType = DFTwindow::RECTANGULAR;
}

void MainWindow::on_box_wnd_size_editingFinished()
{
	int arg1 = ui->box_wnd_size->value();
	static int parg = 128;

	// bit-twiddling to find next power of 2
	int t = arg1 - 1;
	t |= t >> 1;
	t |= t >> 2;
	t |= t >> 4;
	t |= t >> 8;
	t |= t >> 16;
	++t;

	if (t != arg1)
	{
		if (arg1 > parg)
			arg1 = t;
		else
			arg1 = t >> 1;
	}	
	if (arg1 == 0) arg1 = 1;

	ui->box_wnd_size->setValue(arg1);
	parg = arg1;

	// we also need to make sure the overlap size is avaliable
	// for now just set to half the window size;
	int arg2 = arg1 >> 1;
	ui->box_wnd_overlap->setValue(arg2);

	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.wndSize = arg1;
	state->dtft.wndOverlap = arg2;
}

void MainWindow::on_btn_stft_enabled_toggled(bool checked)
{
	if (checked)
	{
		ui->btn_stft_enabled->setIcon(QIcon(":/icons/icons/arrow_green.jpg"));
		ui->btn_stft_enabled->setText("STFT Enabled");
	}
	else
	{
		ui->btn_stft_enabled->setIcon(QIcon(":/icons/icons/arrow_orange.jpg"));
		ui->btn_stft_enabled->setText("STFT Disabled");
	}

	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.enabled = checked;
}

void MainWindow::on_box_spatial_type_currentIndexChanged(const QString &arg1)
{

}

void MainWindow::on_box_time_span_valueChanged(int arg1)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.timeSpan = arg1;
}

void MainWindow::on_slider_max_freq_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.maxFreq = value/999.0;
}

void MainWindow::on_slider_brightness_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.brightness = value / 999.0;
}

void MainWindow::on_slider_dark_boost_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.darkBoost = value / 999.0;
}

void MainWindow::on_slider_hard_limit_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.hardLimit = value / 999.0;
}

void MainWindow::on_box_channel_source_currentIndexChanged(int index)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->dtft.channelSource = index;
}

// ==================================== //
// ======= Translation Controls ======= //
// ==================================== //

void MainWindow::on_slider_tr_span_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.spanFreq = value / 999.0;
}

void MainWindow::on_slider_tr_gain_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.gain = value / 999.0;
}

void MainWindow::on_box_tr_centre_freq_valueChanged(int arg1)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.centreFreq = arg1;
}

void MainWindow::on_slider_tr_offset_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.offset = value / 999.0;
}

void MainWindow::on_btn_tr_larger_toggled(bool checked)
{
	ui->groupBox_translation->setVisible(!checked);

	if (checked)
		ui->btn_tr_larger->setArrowType(Qt::ArrowType::UpArrow);
	else
		ui->btn_tr_larger->setArrowType(Qt::ArrowType::DownArrow);
}

void MainWindow::on_box_tr_span_valueChanged(int arg1)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.spanFreq = arg1;
}

void MainWindow::on_box_tr_spreading_valueChanged(int arg1)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.freqSpreading = arg1;
}

void MainWindow::on_box_tr_flip_toggled(bool checked)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.flip = checked;
}

void MainWindow::on_slider_tr_sensitivity_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.sensitivity = value / 999.0;
}

void MainWindow::on_slider_tr_smoothing_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.smoothing = value / 999.0;
}

// TODO
void MainWindow::on_btn_tr_auto_gain_clicked()
{

}

// TODO
void MainWindow::on_btn_tr_auto_offset_clicked()
{


}

void MainWindow::on_slider_tr_off_speed_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.offCorrSpeed = value / 999.0;
}

void MainWindow::on_slider_tr_off_limit_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.offCorrLimit = value / 999.0;
}

void MainWindow::on_slider_tr_mag_speed_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.magCorrSpeed = value / 999.0;
}

void MainWindow::on_slider_tr_mag_limit_valueChanged(int value)
{
	std::lock_guard<std::mutex> lock(state->mtx_data);
	state->translation.magCorrLimit = value / 999.0;
}
