#include <QDebug>
#include <QDialog>
#include <QCheckBox>
#include <QLayout>
#include <QToolButton>
#include "graphwidget.h"
#include "statevariable.h"

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

GraphWidget::GraphWidget(QWidget* parent)
	:QWidget(parent), m_timeticker(new QCPAxisTickerTime)
{
	m_plot = new QCustomPlot(this);
}

void GraphWidget::init(QString title, QString x_title, QString y_title_left, QString y_title_right)
{
	// ===== ADD TITLES =====
	m_plot->plotLayout()->insertRow(0);
	m_plot->plotLayout()->setRowSpacing(0);
	m_plot->plotLayout()->addElement(0, 0, new QCPTextElement(m_plot, title, QFont("BankGothic Md BT", 14)));
	m_plot->xAxis->setLabel(x_title);
	m_plot->yAxis->setLabel(y_title_left);
	// TODO: secondary axis title

	// ===== SETUP AXES =====
	m_plot->rescaleAxes();
	m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	m_timeticker->setTimeFormat("%h:%m:%s");
	m_plot->xAxis->setTicker(m_timeticker);
	m_plot->axisRect()->setupFullAxesBox();
	m_plot->yAxis->setRange(0, 255);
	m_plot->xAxis->grid()->setSubGridVisible(true);
	m_plot->yAxis->grid()->setSubGridVisible(true);
	//m_plot->setOpenGl(true,1);

	// ===== ADD GUI ELEMENTS =====
	// auto_scroll check box
	QCheckBox* auto_scroll = new QCheckBox(this);
	auto_scroll->setText(tr("Auto-Scroll"));
	auto_scroll->move(QPoint(30, 0));
	auto_scroll->setChecked(true);
	//connect(auto_scroll, &QCheckBox::stateChanged, [this](int state) {}); // TODO

	// options button
	QToolButton* btn_options = new QToolButton(this);
	QIcon icon(":/icons/icons/spanner.png");
	btn_options->setIcon(icon);
	btn_options->setIconSize(QSize(22, 22));
	btn_options->setFixedSize(24, 24);
	btn_options->move(QPoint(2, 2));

	// Dialog + buttons
	m_dialog_select = new QDialog;
	QPushButton* btn_ok = new QPushButton("OK");
	QPushButton* btn_cancel = new QPushButton("Cancel");
	QHBoxLayout* layout_buttons = new QHBoxLayout;
	layout_buttons->addStretch();
	layout_buttons->addWidget(btn_ok);
	layout_buttons->addWidget(btn_cancel);

	m_dialogLayout = new QGridLayout;

	QVBoxLayout* layout_main = new QVBoxLayout;
	layout_main->addLayout(m_dialogLayout);
	layout_main->addStretch();
	layout_main->addLayout(layout_buttons);
	m_dialog_select->setLayout(layout_main);
	m_dialog_select->setWindowTitle(title + ": Select Graphs");


	// main layout
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	mainLayout->addWidget(m_plot);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);


	// ===== CONNECTIONS =====
	connect(m_dialog_select, &QDialog::accepted, this, &GraphWidget::setVisiblePlots);
	connect(btn_ok, &QPushButton::clicked, m_dialog_select, &QDialog::accept);
	connect(btn_cancel, &QPushButton::clicked, m_dialog_select, &QDialog::reject);
	connect(btn_options, &QToolButton::clicked, [this]() {m_dialog_select->show(); m_dialog_select->raise(); });

	// TODO: Mouse Drag should uncheck box
	//connect(this->graph, &GraphWidget::mousePressEvent, [this]() {graph->m_autoscroll = false; auto_scroll->setChecked(false); });
	//mainLayout->addWidget(auto_scroll);
}

void GraphWidget::addVariable(StateVariable* var, bool show)
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
	m_dialogLayout->addWidget(m_checkboxes[m_checkboxes.size() - size_t(1)], m_checkboxes.size() - size_t(1), 0);

	connect(var, &StateVariable::sigUnitsChanged, this, &GraphWidget::slotRefreshNames, Qt::QueuedConnection);
	connect(var, &StateVariable::sigNameChanged, this, &GraphWidget::slotRefreshNames, Qt::QueuedConnection);

	slotRefreshNames();
}

void GraphWidget::refresh()
{
	double key = m_timer.getDuration();

	size_t index = 0;
	for (size_t i=0; i<m_vars.size(); ++i)
	{
		if (m_display[i])
		{
			m_plot->graph(index)->addData(key, m_vars[i]->getValue());
			++index;
		}
	}
}

void GraphWidget::replot()
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

void GraphWidget::clear()
{
	m_timer.restart();

	for (int i = 0; i< m_plot->graphCount(); ++i)
		m_plot->graph(i)->setData(QVector<double>(), QVector<double>());
}

void GraphWidget::slotRefreshNames()
{
	QString var_name;
	size_t j = 0;
	for (size_t i = 0; i < m_vars.size(); ++i)
	{
		if (m_display[i])
		{
			var_name = m_vars[i]->getName() + " (" + m_vars[i]->getUnitName() + ')';
			m_plot->graph(j)->setName(var_name);
			++j;
		}
	}
	
	if (j > 1)
	{
		m_plot->legend->setVisible(true);
		m_plot->yAxis->setLabel("");
	}
	else
	{
		m_plot->legend->setVisible(false);
		m_plot->yAxis->setLabel(var_name);
	}
	replot();
}

void GraphWidget::setVisiblePlots()
{
	while (m_plot->graphCount() > 0)
		m_plot->removeGraph(0);

	for (size_t i = 0; i < m_checkboxes.size(); ++i)
	{
		if (m_checkboxes[i]->isChecked())
		{
			m_display[i] = true;

			// Add Graph
			m_plot->addGraph();
			m_plot->graph(m_plot->graphCount() - 1)->setPen(QPen(PALETTE[(m_plot->graphCount() - 1) % 15]));
		}
		else
		{
			m_display[i] = false;
		}
	}

	slotRefreshNames();
}