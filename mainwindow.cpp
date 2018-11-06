#include "resources.h"
#include <QWinTaskbarButton>
#include <QtWidgets>
#include <QDesktopServices>
#include <QSerialPortInfo>

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
	ui->plot_freq->setRange({ 0,60 }, { 0,10 });

	ui->plot_time->setTitle("Time-Domain Data");
	ui->plot_time->setAxisTitles("Magnitude (V)", "", "Time (h:m:s)", "");
	ui->plot_time->setRange({ 0,128 }, { -0.2,0.2 });
	ui->plot_time->setRealTime();

	ui->plot_translation_left->setTitle("Translation Control (left)");
	ui->plot_translation_left->setAxisTitles("Magnitude (V)", "", "Time (h:m:s)", "");
	ui->plot_translation_left->setRange({ 0,120 }, { -1,1 });
	ui->plot_translation_left->addGraph({ "Control", "", true});
	ui->plot_translation_left->setRealTime();

	ui->plot_translation_right->setTitle("Translation Control (right)");
	ui->plot_translation_right->setAxisTitles("Magnitude (V)", "", "Time (h:m:s)", "");
	ui->plot_translation_right->setRange({ 0,120 }, { -1,1 });
	ui->plot_translation_right->addGraph({ "Control", "", true });
	ui->plot_translation_right->setRealTime();

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

	on_box_source_currentIndexChanged("Offline");
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

	bool resetMainwindow;
	double freq;
	double timeLerp;
	double time_plot_time;
	double tr_plot_time;
	std::vector<std::vector<double>> rawTD;
	std::vector<std::vector<double>> rawFD;
	std::vector<std::vector<std::vector<double>>> graph_spfillTR;

	// get data
	{
		std::lock_guard<std::mutex> lock(ControllerState::state.mtx_data);
		resetMainwindow = ControllerState::state.resetMainwindow;
		ControllerState::state.resetMainwindow = false;
		freq = ControllerState::state.freq;
		timeLerp = ControllerState::state.timeLerp;
		time_plot_time = ControllerState::state.time_plot_time;
		tr_plot_time = ControllerState::state.tr_plot_time;
		rawTD = std::move(ControllerState::state.rawTD);
		ControllerState::state.rawTD.clear();
		rawFD = std::move(ControllerState::state.rawFD);
		ControllerState::state.rawFD.clear();
		graph_spfillTR = std::move(ControllerState::state.graph_spfillTR);
		ControllerState::state.graph_spfillTR.clear();
	}

	if (resetMainwindow)
	{
		ui->plot_time->clearData();
		ui->plot_translation_left->clearData();
		ui->plot_translation_right->clearData();
	}

	// update time plot

	if (rawTD.size() > 0 && rawTD[0].size() > 0)
	{
		// ensure we have the right number of graphs
		if (ui->plot_time->numGraphs() != rawTD.size())
		{
			ui->plot_time->clearGraphs();
			for (size_t i = 0; i < rawTD.size(); i++)
			{
				ui->plot_time->addGraph({ ("Channel " + QString::number(i)), " (V)", true });
			}
		}

		double deltaT = 1.0 / freq;
		double startTime = time_plot_time - rawTD[0].size()*deltaT;

		for (size_t ch = 0; ch < rawTD.size(); ++ch)
		{
			for (size_t t = 0; t < rawTD[0].size(); ++t)
			{
				ui->plot_time->addData(ch, startTime + deltaT*(t+1), rawTD[ch][t]);
			}
		}
	}

	// update freq plot
	
	if (rawFD.size() > 0 && rawFD[0].size() > 0)
	{
		// ensure we have the right number of graphs
		if (ui->plot_freq->numGraphs() != rawFD.size())
		{
			ui->plot_freq->clearGraphs();
			for (size_t i = 0; i < rawFD.size(); i++)
			{
				ui->plot_freq->addGraph({ ("Channel " + QString::number(i)), " (V)", true });
			}
		}

		ui->plot_freq->clearData();

		for (size_t ch = 0; ch < rawFD.size(); ++ch)
		{
			for (size_t t = 0; t < rawFD[0].size(); ++t)
			{
				ui->plot_freq->addData(ch, (freq*t)/rawFD[0].size(), rawFD[ch][t]);
			}
		}
	}

	// add translation data

	if (graph_spfillTR.size() > 0)
	{
		// ensure we have the right number of graphs
		if (ui->plot_translation_left->numGraphs() != graph_spfillTR[0].size())
		{
			ui->plot_translation_left->clearGraphs();
			ui->plot_translation_right->clearGraphs();

			for (size_t i = 0; i < graph_spfillTR[0].size(); i++)
			{
				ui->plot_translation_left->addGraph({ ("Channel " + QString::number(i)), " (V)", true });
				ui->plot_translation_right->addGraph({ ("Channel " + QString::number(i)), " (V)", true });
			}
		}

		double deltaT = timeLerp;
		double startTime = tr_plot_time - graph_spfillTR.size()*deltaT;

		for (size_t t = 0; t < graph_spfillTR.size(); ++t)
		{
			for (size_t n = 0; n < graph_spfillTR[0].size(); ++n)
			{
				ui->plot_translation_left->addData(n, startTime + deltaT*(t+1), graph_spfillTR[t][n][0]);
				ui->plot_translation_right->addData(n, startTime + deltaT*(t+1), graph_spfillTR[t][n][1]);
			}
		}
	}

	// replot graphs
	if (ui->tabWidget->tabText(ui->tabWidget->currentIndex()) == "Freq Analysis")
		ui->plot_freq->replot();
	else if (ui->tabWidget->tabText(ui->tabWidget->currentIndex()) == "Time Analysis")
		ui->plot_time->replot();
	else if (ui->tabWidget->tabText(ui->tabWidget->currentIndex()) == "Translation")
	{
		ui->plot_translation_left->replot();
		ui->plot_translation_right->replot();
	}

	emit sigViewReady();
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

	on_box_source_currentIndexChanged(ui->box_source->currentText());
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
		ui->btn_connect->setText("Connecting");

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
		if (ui->box_source->currentText() == "CytonDaisy")
		{
			if (ui->box_com_port->currentText() == "No ports available")
			{
				DEBUG_PRINTLNY("Please plug in the USB Dongle", MSG_TYPE::DEXCEP);
				slotRunStateChanged(false);
				return;
			}
			auto comPort = ui->box_com_port->currentText().section(' ', 0, 0);
			emit sigRunController(ui->box_source->currentIndex(), comPort, ui->line_data_in->text(),  ui->box_freq->value());
		}
		else
		{
			emit sigRunController(ui->box_source->currentIndex(), "", ui->line_data_in->text(), ui->box_freq->value());
		}

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

	if (arg1 == "CytonDaisy")
	{
		ui->box_com_port->setVisible(true);
		ui->lbl_com_port->setVisible(true);
	}
	else
	{
		ui->box_com_port->setVisible(false);
		ui->lbl_com_port->setVisible(false);
	}

	auto ports = QSerialPortInfo::availablePorts();
	ui->box_com_port->clear();
	if (ports.size() == 0)
	{
		ui->box_com_port->addItem("No ports available");
		ui->box_com_port->setEnabled(false);
	}
	else
	{
		ui->box_com_port->setEnabled(true);
	}
			

	for (auto& p : ports)
	{
		auto str = p.portName();
		if (p.manufacturer() != "")
			str += " (" + p.manufacturer() + ')';
		if (p.isBusy())
			str += " (busy)";

		ui->box_com_port->addItem(str);
	}
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
