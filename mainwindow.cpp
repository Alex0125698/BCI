#include <QWinTaskbarButton>
#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "graphwidget.h"
#include "state.h"
#include "variable.h"
#include "view.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	bci::State::init();

	// ===== SETUP MAINWINDOW =====
	ui->setupUi(this);
	m_core = new Core(this);
	//QWinTaskbarButton *button = new QWinTaskbarButton(this);
	//button->setWindow(this->windowHandle());
	//button->setOverlayIcon(QIcon(":/icons/icons/cool arrow effect.jpg"));
	
	// ===== SETUP DEBUG WINDOW =====
		// it will be shown as a separate window
	m_debug_window = new QPlainTextEdit;
	m_debug_window->setReadOnly(true);
	m_debug_window->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
	m_debug_window->setWindowTitle("Debug Window");
	m_debug_window->show();
	//m_debug_window->setWindowIcon(QIcon(":/icons/icons/debug.png"));
	this->setWindowTitle("Brain-Computer Interface Controller");

	// ===== CONNECTIONS =====
	connect(&DebugOutput::getDebug(), &DebugOutput::sigDebugMessage, this, &MainWindow::slotDebugMessage, Qt::QueuedConnection);

	DEBUG_PRINTLN("Program Started");

	// ===== ADD STATEVARIABLES TO STATEGROUPS =====

	auto chVars = bci::State::program.searchVars({ Tag::DATA_CHANNEL });
	auto fbandVars = bci::State::program.searchVars({ Tag::FREQ_BAND });
	fbandVars.push_back(fbandVars[0]);
	auto freqViews = bci::State::program.generateViews(fbandVars, View::Type::TABLE);

	ui->group_freqs->setup("", {}, freqViews);

	// ===== SET UP GRAPHS =====

	ui->plot_freq->init("Frequency Bands", "Time (s)", "");
	ui->plot_time->init("Time-Domain", "Time (s)", "");

	for (auto& var : chVars)
	{
		ui->plot_time->addVariable(var, true);
	}
	for (auto& var : fbandVars)
	{
		ui->plot_freq->addVariable(var, true);
	}

	// ===== set up status bar =====

	auto statusVars = bci::State::program.searchVars({ Tag::STATUS_BAR });
	auto statusViews = bci::State::program.generateViews(statusVars, View::Type::TABLE);

	for (auto& view : statusViews)
	{
		auto tmp = new QWidget();
		tmp->setLayout(view->getLayout());
		ui->statusbar->addPermanentWidget(tmp);
	}

	m_core = new Core(this);
}

MainWindow::~MainWindow()
{
	delete ui;
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
	ui->plot_time->replot();
	ui->plot_freq->replot();

	emit mainwindowReady();
}

void MainWindow::slotGraphUpdate()
{
	ui->plot_time->addData();
	ui->plot_freq->addData();
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

void MainWindow::on_btn_connect_clicked(bool checked)
{
	emit sigChangeControllerState(checked);
}

void MainWindow::on_btn_uoa_clicked()
{

}

void MainWindow::on_btn_out_file_clicked()
{

}

void MainWindow::on_btn_save_options_clicked()
{

}

void MainWindow::on_btn_save_start_clicked()
{
	
}

void MainWindow::on_btn_save_stop_clicked()
{
	
}

void MainWindow::on_box_source_currentIndexChanged(const QString& arg1)
{

}
