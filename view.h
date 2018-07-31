#pragma once

#include "resources.h"
#include "variable.h"
#include <QComboBox>

class QLayout;
class QWidget;
class QLabel;
class QDoubleSpinBox;

class View : public QObject
{
	Q_OBJECT

public:
	enum class Type
	{
		TABLE,
		SLIDER,
		GAUGE,
		DIAL,
		GRAPH,
		BUTTONS
	};

public:
	View(const std::vector<Variable*>& vars, const std::vector<QString>&& titles)
		: m_vars{ vars }, m_titles{ titles } {}
	virtual ~View() = default;
	// get the layout
	constexpr const auto getLayout() const { return m_layout; }

protected:
	const std::vector<Variable*> m_vars;
	const std::vector<QString> m_titles;
	QLayout* m_layout{ nullptr };
};

class TableView : public View
{
	Q_OBJECT

public:
	TableView(const std::vector<Variable*>& vars, const std::vector<QString>&& titles = {});
};

class SliderView : public View
{
	Q_OBJECT

public:
	SliderView(const std::vector<Variable*>& vars, const std::vector<QString>&& titles = {});

};

class ButtonsView : public View
{
	Q_OBJECT

public:
	ButtonsView(const std::vector<Variable*>& vars, const std::vector<QString>&& titles = {});

};

class StateGroup : public QWidget
{
	Q_OBJECT

public:
	StateGroup(QWidget* parent)
		: QWidget(parent)
	{
	}
	void setup(QString groupName, std::vector<QString> colNames, std::vector<View*> views);
};



