/*****************************************************************************************
*    File:    StateVariable.h                                                            *
*    Author:  A.S. Woodcock (alex.woodcock@outlook.com)                                  *
*    Project: Lynx/TestRig (P0011)                                                       *
*    Company: The Dynamic Engineering Solution Ptd. Ltd.                                 *
*                                                                                        *
*    StateVariable class - This class stores all information about a particular variable *
*            including its name, raw size and units. It keeps track of two pointers      *
*            for its input and output location (all raw values are stored elsewhere).    *
*                                                                                        *
*    License Info: TODO                                                                  *
*****************************************************************************************/

/* This class lists out the variables in formatted groups
 * with titles (and a group title)
 *
 * It is Graphical only - no special functions
 *
 * For not, promote to this class in ui file
 */

#pragma once

#include <cstdint>
#include <cassert>
#include <vector>
#include <QObject>
#include <QWidget>
#include <QDebug>

class QLayout;

// ===== Unit =====

class Unit
{
public:
	Unit(QString name, double scale, double offset, bool is_signed);
	double convert(uint32_t raw, uint32_t size_in_bytes);
	uint32_t getRaw(double value, uint32_t size_in_bytes);
	const QString& getName() { return m_name; }

private:
	QString m_name;
	double m_scale{ 0 };
	double m_offset{ 0 };
	bool m_signed{ false };
};

// ===== StateVariable =====

class StateVariable : public QObject
{
	Q_OBJECT

public:
	StateVariable(QString name, uint32_t size_in_bytes, const uint32_t* const in, uint32_t min_raw, uint32_t max_raw, uint32_t* const out = nullptr, bool name_editable = false);

public:
	// is a setpoint avaliable for the variable?
	bool isOutput() { return m_raw_out_ptr != nullptr; }
	// is a measured/sensor value avaliable?
	bool isInput() { return m_raw_in_ptr  != nullptr; }
	// can the user modify the variable name?
	bool isEditable() { return m_name_editable; }
	// get the input value (in the active unit)
	double getValue() { return getValue(m_active_unit); }
	// get the input value in the desired unit
	double getValue(int unit_index);
	// get the setpointValue (in the active unit)
	double getSetpoint() { return getSetpoint(m_active_unit); }
	// get the setpoint value in the desired unit
	double getSetpoint(int unit_index);
	// get the max value in the current unit
	double getMaximum()
	{
		return units[m_active_unit].convert(m_max_raw, m_size_in_bytes);
	}
	// get the minimum value in the current unit
	double getMinimum();
	// get the variable name
	const QString& getName() { return m_name; }
	// the minimum change in raw data (i.e. 1) mapped to the current unit
	double getStepSize();
	// get the active unit index
	const uint32_t getActiveUnit(){ return m_active_unit; }
	// get the name of the active unit
	const QString& getUnitName() { return units[m_active_unit].getName(); }

signals:
	void sigNameChanged();
	void sigValueChanged();
	void sigSetpointChanged();
	void sigUnitsChanged();

public:
	// find the nearest multiple of the step size
	// double clampValue(double desiredValue);
	// add units to this vector - default raw units only
	std::vector<Unit> units;

public slots:
	// set the variable name string
	void setName(QString str);
	// causes associated views to update
	void updateValue();
	// set the stepoint (will be clamped)
	void setSetpoint(double desiredValue);
	// change the active unit
	void setActiveUnit(int unit_index);

protected:
	QString m_name;
	bool m_name_editable;
	uint32_t m_active_unit{ 0 };
	uint32_t m_size_in_bytes;
	const uint32_t* const m_raw_in_ptr;
	uint32_t* const m_raw_out_ptr;
	const uint32_t m_min_raw;
	const uint32_t m_max_raw;
	bool m_reset{ true };
};

// ===== StateView =====

class StateView : public QObject
{
	Q_OBJECT

public:
	enum TYPE
	{
		TABLE,
		BUTTONS,
		SLIDER
	};
	StateView(StateVariable& var, TYPE type = TABLE, std::vector<QString> names = std::vector<QString>());
	// get sizing of each widget
	std::vector<int> getSizingInfo();
	// use this to allign StateViews of the same type
	void setSizing(std::vector<int> sizes);
	// the view type (see enum)
	StateView::TYPE getType();

public slots:
	void slotGetSetpoint();
	void slotNameChanged();
	void slotValueChanged();
	void slotSetpointChanged();
	void slotUnitsChanged();

public:
	// everything must put inside this layout
	QLayout* layout{ nullptr };

protected:
	// stored the underlying state + units
	StateVariable& m_var_ref;
	// how the view displays the state
	StateView::TYPE m_type;
	QWidget* m_name{ nullptr };
	QWidget* m_value{ nullptr };
	QWidget* m_setpoint{ nullptr };
	QWidget* m_units{ nullptr };
};

// ===== StateGroup =====

class StateGroup : public QWidget
{
	Q_OBJECT

public:
	StateGroup(QWidget* parent);
	void setup(std::vector<QString> colNames, QString groupName);
	void addView(StateView* view) { m_views.push_back(view); }

private:
	std::vector<StateView*> m_views;
};
