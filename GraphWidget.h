#pragma once

#include "resources.h"
#include <QWidget>
#include <QFont>
#include "../shared/qcustomplot.h"

// forward declarations
class QToolButton;
class QCheckBox;
class QDialog;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QDialogButtonBox;

typedef const QString str;
typedef const std::pair<double, double> range;

class GraphWidget : public QWidget
{
	Q_OBJECT

public:
	struct Graph
	{
		QString name;
		QString extension;
		bool active{ false };
	};
	struct GUI
	{
		QToolButton* optionsBtn;
		QCheckBox* scrollBox;		
		// dialog stuff
		QDialog* selectDialog;
		QVBoxLayout* mainLayout;
		QHBoxLayout* btnLayout;
		QGridLayout* checkLayout;
		QDialogButtonBox* buttons;
		std::vector<QCheckBox*> activeBoxs;
	};

public:
	GraphWidget(QWidget* parent);
	void setTitle(str graphTitle = "title", QFont font = QFont("times",14));
	void setRange(range xRange = { 0,1 }, range yRange = { 0,1 });
	void setAxisTitles(str leftAxisText = "l", str rightAxisText = "r", str bottmpAxisText = "b", str topAxisText = "t");
	void setRealTime();

public:
	void addGraph(Graph&& properties);
	void addData(size_t index, std::vector<double>& x, std::vector<double>& y);
	void clearData();
	void replot();

private slots:
	void setVisiblePlots();
	void showSelectDialog();

private:
	QCustomPlot* m_plot{ nullptr };
	std::vector<Graph> m_graphs;
	QSharedPointer<QCPAxisTickerTime> m_timeticker;
	QGridLayout* m_dialogLayout{ 0 };
	double m_time{ 0 };
	GUI gui;
};
