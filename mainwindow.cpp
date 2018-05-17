#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "GraphWidget.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	// ===== setup debug window =====
		// it will be shown as a separate window
	m_debug_window = new QPlainTextEdit;
	m_debug_window->setReadOnly(true);
	m_debug_window->appendPlainText("Program Started");
	m_debug_window->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
	m_debug_window->setWindowTitle("Debug Window");
	m_debug_window->show();
	//m_debug_window->setWindowIcon(QIcon(":/icons/icons/debug.png"));
	this->setWindowTitle("Brain-Computer Interface");

	// ===== Set up Graphs =====

	ui->plot_freq->init("Frequency Bands", "Time (s)", "Band");
	ui->plot_time->init("Tim-Domain", "Time (s)", "Signal");


	// ===== Connections =====
	connect(&DebugOutput::getDebug(), &DebugOutput::output, this, &MainWindow::processDebugMessage, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_btn_connect_toggled(bool checked)
{
	if (checked)
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

void MainWindow::processDebugMessage(QString msg, QString file, int line, int type)
{
	if (type != MSG_TYPE::MESSAGE && type != MSG_TYPE::GOOD_NEWS && type != MSG_TYPE::STD)
	{
		file = file.section('\\', -1);
		msg += QString("\n Error Thrown on line: %1  file: %2").arg(line).arg(file);
	}

	auto setcolor = [&](QString c)
	{
		return "<html><head/><body><p><span style=\" font-weight:600; color:#" + c + ";\">" + msg + "</span></p></body></html>";
	};

	switch (type)
	{
	case MSG_TYPE::FATAL:
		msg = setcolor("e01040");
		// TODO: show details
		QApplication::beep();
		break;
	case MSG_TYPE::GOOD_NEWS:
		msg = setcolor("14c440");
		break;
	case MSG_TYPE::MESSAGE:
		msg = setcolor("303030");
		break;
	case MSG_TYPE::NON_FATAL:
		msg = setcolor("d02510");
		// TODO: show details
		QApplication::beep();
		break;
	case MSG_TYPE::WARNING:
		msg = setcolor("d0c510");
		break;
	}

	m_debug_window->appendHtml(msg);
}
