#include "resources.h"
#include "graphwidget.h"
#include <QToolButton>
#include <QLayout>
#include <QDialog>
#include <QCheckBox>

static const std::array<QColor,size_t(16)> PALETTE
{
	{
		QColor(225, 50, 50),   QColor(50, 225, 50),
		QColor(50, 50, 225),   QColor(225, 225, 0),
		QColor(225, 0, 225),   QColor(0, 255, 255), 
		QColor(175, 175, 175), QColor(115, 115, 115),
		QColor(55, 55, 55),    QColor(200, 160, 160),
		QColor(160, 200, 160), QColor(160, 160, 200),
		QColor(200, 200, 145), QColor(200, 145, 200),
		QColor(145, 145, 200), QColor(30, 30, 30)
	}
};

GraphWidget::GraphWidget(QWidget* parent)
	:QWidget(parent), m_timeticker(new QCPAxisTickerTime)
{
	m_plot = new QCustomPlot(this);

	// ===== Default Setup =====
	setTitle();
	setAxisTitles();
	setRange();

	// ===== Default Settings =====
	//m_plot->rescaleAxes();
	m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);
	m_plot->axisRect()->setupFullAxesBox();
	m_plot->xAxis->grid()->setSubGridVisible(true);
	m_plot->yAxis->grid()->setSubGridVisible(true);
	//m_plot->setOpenGl(true,1);

	// ===== Add GUI Elements =====

	// auto-scroll box
	gui.scrollBox = new QCheckBox(this);
	gui.scrollBox->setText(tr("Auto-Scroll"));
	gui.scrollBox->move({ 30,0 });
	gui.scrollBox->setChecked(true);

	connect(m_plot, &QCustomPlot::mousePress, [this]() {gui.scrollBox->setChecked(false); });
	connect(m_plot, &QCustomPlot::mouseWheel, [this]() {gui.scrollBox->setChecked(false); });

	// options button
	gui.optionsBtn = new QToolButton(this);
	gui.optionsBtn->setIcon(QIcon(":/icons/icons/spanner.png"));
	gui.optionsBtn->setIconSize({ 22,22 });
	gui.optionsBtn->setFixedSize(24, 24);
	gui.optionsBtn->move({ 2,2 });

	// Dialog + buttons
	gui.selectDialog = new QDialog;
	gui.buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	gui.mainLayout = new QVBoxLayout;
	gui.checkLayout = new QGridLayout;
	gui.btnLayout = new QHBoxLayout;

	gui.btnLayout->addStretch();
	gui.btnLayout->addWidget(gui.buttons);
	gui.mainLayout->addLayout(gui.checkLayout);
	gui.mainLayout->addStretch();
	gui.mainLayout->addLayout(gui.btnLayout);

	gui.selectDialog->setLayout(gui.mainLayout);
	gui.selectDialog->setWindowTitle("Select Visible Graphs");

	// main layout
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	mainLayout->addWidget(m_plot);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);

	// ===== CONNECTIONS =====
	connect(gui.buttons, &QDialogButtonBox::accepted, gui.selectDialog, &QDialog::accept);
	connect(gui.buttons, &QDialogButtonBox::rejected, gui.selectDialog, &QDialog::reject);
	connect(gui.selectDialog, &QDialog::accepted, this, &GraphWidget::setVisiblePlots);
	connect(gui.optionsBtn, &QToolButton::clicked, this, &GraphWidget::showSelectDialog);
}

void GraphWidget::setTitle(str graphTitle, QFont font)
{
	// todo: test
	if (m_plot->plotLayout()->elementCount() <= 1)
	{
		m_plot->plotLayout()->insertRow(0);
		m_plot->plotLayout()->setRowSpacing(0);
		m_plot->plotLayout()->addElement(0, 0, new QCPTextElement(m_plot, graphTitle, font));
	}
	else
	{
		QCPTextElement* title = reinterpret_cast<QCPTextElement*>(m_plot->plotLayout()->element(0, 0));
		title->setText(graphTitle);
		title->setFont(font);
	}
}

void GraphWidget::setRange(range xRange, range yRange)
{
	if (!(xRange.first == 0 && xRange.second == 0))
	{
		m_plot->xAxis->setRange(xRange.first, xRange.second);
		m_plot->xAxis2->setRange(xRange.first, xRange.second);
	}
	if (!(yRange.first == 0 && yRange.second == 0))
	{
		m_plot->yAxis->setRange(yRange.first, yRange.second);
		m_plot->yAxis2->setRange(yRange.first, yRange.second);
	}
}

void GraphWidget::setAxisTitles(str leftAxisText, str rightAxisText, str bottmpAxisText, str topAxisText)
{
	if (!leftAxisText.isEmpty())   m_plot->yAxis->setLabel(leftAxisText);
	if (!rightAxisText.isEmpty())  m_plot->yAxis2->setLabel(rightAxisText);
	if (!bottmpAxisText.isEmpty()) m_plot->xAxis->setLabel(bottmpAxisText);
	if (!topAxisText.isEmpty())    m_plot->xAxis2->setLabel(topAxisText);
}

void GraphWidget::setRealTime()
{
	m_timeticker->setTimeFormat("%h:%m:%s");
	m_plot->xAxis->setTicker(m_timeticker);
}

void GraphWidget::clearData()
{
	m_time = 0;

	for (int i = 0; i < m_plot->graphCount(); ++i)
		m_plot->graph(i)->setData({},{});
}

void GraphWidget::replot()
{
	m_plot->replot();

	if (gui.scrollBox->isChecked())
	{
		auto oldRange = m_plot->xAxis->range().size();
		m_plot->xAxis->setRange(std::clamp(m_time - oldRange, 0.0, m_time), m_time);
	}
}

void GraphWidget::addGraph(GraphWidget::Graph&& properties)
{
	auto index = m_plot->graphCount();
	m_graphs.push_back(properties);

	m_plot->addGraph();
	m_plot->graph(index)->setName(m_graphs[index].name + m_graphs[index].extension);
	m_plot->graph(index)->setPen(QPen(PALETTE[index % PALETTE.size()]));
	m_plot->graph(index)->setVisible(m_graphs[index].active);

	if (m_graphs.size() > 1)
		m_plot->legend->setVisible(true);
}

void GraphWidget::addData(size_t index, std::vector<double>& x, std::vector<double>& y)
{
	assert(x.size() == y.size());
	for (size_t i = 0; i < x.size(); ++i)
		m_plot->graph((int)index)->addData(x[i], y[i]);

	m_time = *(x.end()-1);
}

void GraphWidget::setVisiblePlots()
{
	for (size_t i = 0; i < gui.activeBoxs.size(); ++i)
	{
		m_graphs[i].active = gui.activeBoxs[i]->isChecked();
		m_plot->graph(i)->setVisible(m_graphs[i].active);
	}
}

void GraphWidget::showSelectDialog()
{
	if (gui.activeBoxs.size() != m_graphs.size())
	{
		for (auto& w : gui.activeBoxs)
		{
			gui.btnLayout->removeWidget(w);
			delete w;
		}

		gui.activeBoxs.resize(m_graphs.size());
		for (size_t i = 0; i < gui.activeBoxs.size(); ++i)
		{
			gui.activeBoxs[i] = new QCheckBox(m_graphs[i].name);
			gui.checkLayout->addWidget(gui.activeBoxs[i], i & 0b0111u, i >> 3u);
		}
	}

	for (size_t i = 0; i < gui.activeBoxs.size(); ++i)
	{
		gui.activeBoxs[i]->setChecked(m_graphs[i].active);
	}

	gui.selectDialog->show();
	gui.selectDialog->raise();
}
