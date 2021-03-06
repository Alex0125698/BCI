#pragma once

#include "resources.h"
#include <QWidget>
#include "../shared/qcustomplot.h"

class QDialog;
class Variable;
class QGridLayout;
class QCheckBox;

class GraphWidget : public QWidget
{
	Q_OBJECT

public:
	GraphWidget(QWidget* parent);
	// add titles ; setup dialog
	void init(const QString title, const QString x_title, const QString y_title_left, const QString y_title_right = "");
	// add variable to plot
	void addVariable(Variable* const var, const bool visible);
	// grab data from StateVariables (but don't plot)
	void addData();
	// replot data stored in buffers; redraw labels
	void replot();
	// clear data and reset time to 0
	void clear();

public slots:
	void setVisiblePlots();
	void slotRefreshNames();

private:
	// graph library
	QCustomPlot* m_plot{ nullptr };
	// store variables ; graph will automatically plot
	std::vector<Variable*> m_vars;
	// which variables currently visible on graph?
	std::vector<bool> m_display;
	// store the checkboxes
	std::vector<QCheckBox*> m_checkboxes;
	// time for points
	Timer m_timer;
	// time format for axis
	QSharedPointer<QCPAxisTickerTime> m_timeticker;
	// does the graph move over time
	bool m_autoscroll{ true };
	// dialog to select display graphs
	QDialog* m_dialog_select{ nullptr };
	// layout to store check boxes
	QGridLayout* m_dialogLayout{ 0 };
};
