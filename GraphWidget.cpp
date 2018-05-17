#include "GraphWidget.h"

#include <qlayout.h>
#include <QToolButton>

#include "StateVariable.h"
//#include "Data_Structures.h"
#include <QDebug>
#include <QDialog>
#include <QCheckBox>

const QColor PALETTE[] =
{
	QColor(225, 50, 50),
	QColor(50, 225, 50),
	QColor(50, 50, 225),
	QColor(225, 225, 0),
	QColor(225, 0, 225),
	QColor(0, 255, 255),
	QColor(175, 175, 175),
	QColor(115, 115, 115),
	QColor(55, 55, 55),
	QColor(200, 160, 160),
	QColor(160, 200, 160),
	QColor(160, 160, 200),
	QColor(200, 200, 145),
	QColor(200, 145, 200),
	QColor(145, 145, 200)
};

GraphWidgetWrapper::GraphWidgetWrapper(QWidget* parent)
	:QWidget(parent), m_timeticker(new QCPAxisTickerTime)
{
	m_plot = new QCustomPlot(this);
}

void GraphWidgetWrapper::init(QString title, QString x_title, QString y_title_left, QString y_title_right)
{
	// ===== title ; axis titles ; names =====
	m_plot->plotLayout()->insertRow(0);
	m_plot->plotLayout()->setRowSpacing(0); // !!! TEST
											// TODO: font !!!
	m_plot->plotLayout()->addElement(0, 0, new QCPTextElement(m_plot, title, QFont("BankGothic Md BT", 15)));
	m_plot->xAxis->setLabel(x_title);
	m_plot->yAxis->setLabel(y_title_left);
	// TODO: right title

	// ===== set visible elements =====
	m_plot->legend->setVisible(true);
	m_plot->xAxis->grid()->setSubGridVisible(true);
	m_plot->yAxis->grid()->setSubGridVisible(true);

	// ===== Format Axes =====
	m_plot->rescaleAxes();
	m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	m_timeticker->setTimeFormat("%h:%m:%s");
	m_plot->xAxis->setTicker(m_timeticker);
	m_plot->axisRect()->setupFullAxesBox();
	m_plot->yAxis->setRange(0, 255);
	//m_plot->reset();
	//m_plot->setNotAntialiasedElement(QCP::aeAll);
	//m_plot->setOpenGl(true,1);

	m_timer.restart();
	
	// ===== Add GUI Elements =====

	QCheckBox* auto_scroll = new QCheckBox(this);
	auto_scroll->setText(tr("Auto-Scroll"));
	auto_scroll->move(QPoint(30, 0));
	auto_scroll->setChecked(true);
	connect(auto_scroll, &QCheckBox::stateChanged, [this](int state) {}); // TODO

	QToolButton* btn_options = new QToolButton(this);
	QIcon icon(":/icons/icons/propertysheets.ico");
	btn_options->setIcon(icon);
	btn_options->setIconSize(QSize(24, 24));
	btn_options->setFixedSize(24, 24);
	btn_options->move(QPoint(2, 2));

	QVBoxLayout* layout = new QVBoxLayout(this);
	this->setLayout(layout);
	layout->addWidget(m_plot);
	layout->setMargin(0);
	layout->setSpacing(0);

	// ===== Connections =====
	m_dialog_select = new QDialog;
	connect(m_dialog_select, &QDialog::accepted, this, &GraphWidgetWrapper::setVisiblePlots);

	QPushButton* btn_ok = new QPushButton("OK");
	QPushButton* btn_cancel = new QPushButton("Cancel");
	QHBoxLayout* layout_buttons = new QHBoxLayout;
	connect(btn_ok, &QPushButton::clicked, m_dialog_select, &QDialog::accept);
	connect(btn_cancel, &QPushButton::clicked, m_dialog_select, &QDialog::reject);
	layout_buttons->addStretch();
	layout_buttons->addWidget(btn_ok);
	layout_buttons->addWidget(btn_cancel);

	layout_vars = new QGridLayout;
	
	QVBoxLayout* layout_main = new QVBoxLayout;

	layout_main->addLayout(layout_vars);
	layout_main->addStretch();
	layout_main->addLayout(layout_buttons);

	m_dialog_select->setLayout(layout_main);
	m_dialog_select->setWindowTitle(title + ": Select Graphs");
	connect(btn_options, &QToolButton::clicked, [this]() {m_dialog_select->show(); m_dialog_select->raise(); });

	// TODO: Mouse Drag should uncheck box
	//connect(this->graph, &GraphWidgetWrapper::mousePressEvent, [this]() {graph->m_autoscroll = false; auto_scroll->setChecked(false); });
	//layout->addWidget(auto_scroll);
}

void GraphWidgetWrapper::addVariable(StateVariable* var, bool show)
{
	m_vars.push_back(var);
	m_display.push_back(show);
	m_checkboxes.push_back(new QCheckBox(var->getName()));

	if (show)
	{
		// Add Graph
		m_plot->addGraph();
		m_plot->graph(m_plot->graphCount() - 1)->setName(var->getName());
		m_plot->graph(m_plot->graphCount() - 1)->setPen(QPen(PALETTE[(m_plot->graphCount() - 1) % 15]));
	}

	m_checkboxes[m_checkboxes.size() - 1]->setChecked(show);
	layout_vars->addWidget(m_checkboxes[m_checkboxes.size() - 1], m_checkboxes.size() - 1, 0);
}

void GraphWidgetWrapper::refresh()
{
	double key = m_timer.getDuration();

	int index = 0;
	for (int i=0; i<m_vars.size(); ++i)
	{
		if (m_display[i])
		{
			m_plot->graph(index)->addData(key, m_vars[i]->getValue());
			++index;
		}
	}
}

void GraphWidgetWrapper::replot()
{
	m_plot->replot();

	// move graph along ?
	if (m_autoscroll)
	{
		//double width = m_plot->xAxis->range().upper - m_plot->xAxis->range().lower;
		//if (width > 25)
		double key = m_timer.getDuration(); // time elapsed since start of test
		m_plot->xAxis->setRange(key - 10, key);
		//else
		//	this->xAxis->setRange(0, key);
	}
}

void GraphWidgetWrapper::clear()
{
	m_timer.restart();

	for (int i = 0; i< m_plot->graphCount(); ++i)
		m_plot->graph(i)->setData(QVector<double>(), QVector<double>());
}

GraphWidgetWrapper::~GraphWidgetWrapper()
{
	// TODO: delete stuff
}

void GraphWidgetWrapper::setVisiblePlots()
{
	// TODO
	while (m_plot->graphCount() > 0)
	{
		m_plot->removeGraph(0);
	}

	for (int i = 0; i < m_checkboxes.size(); ++i)
	{
		if (m_checkboxes[i]->isChecked())
		{
			m_display[i] = true;
			// Add Graph
			m_plot->addGraph();
			m_plot->graph(m_plot->graphCount() - 1)->setName(m_vars[i]->getName());
			m_plot->graph(m_plot->graphCount() - 1)->setPen(QPen(PALETTE[(m_plot->graphCount() - 1) % 15]));
		}
		else
		{
			m_display[i] = false;
		}
	}
	replot();
}

void GraphWidgetWrapper::m_select_points()
{
}
