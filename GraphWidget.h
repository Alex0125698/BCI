#pragma once

#include <vector>
#include <QWidget>
#include "qcustomplot.h"
#include "Timer.h"

class QDialog;
class StateVariable;
class QGridLayout;
class QCheckBox;

class GraphWidgetWrapper : public QWidget
{
	Q_OBJECT

	friend class MainWindow;
public:
	GraphWidgetWrapper(QWidget* parent);
	void init(QString title, QString x_title, QString y_title_left, QString y_title_right = "");
	void addVariable(StateVariable* var, bool show);
	void refresh();
	void replot();
	void clear();
	~GraphWidgetWrapper();

public slots:
	void setVisiblePlots();

private:
	// graph library
	QCustomPlot* m_plot{ nullptr };
	// store variables ; graph will automatically plot
	std::vector<StateVariable*> m_vars;
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
	// will refresh the graphs to display
	void m_select_points();
	// !!! HACK !!!
	QGridLayout* layout_vars{ 0 };

	friend class Ui_MainWindow;
};