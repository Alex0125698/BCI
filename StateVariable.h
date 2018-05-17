#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QVBoxLayout;
class QDoubleSpinBox;
class QString;
class QSpacerItem;

class StateVariable
{
private:
	unsigned int m_raw{ 0 };
	bool m_has_units{ false };
	bool m_has_setpoint{ false };
	bool m_name_editable{ false };
	double m_scale;
	double m_offset;
	bool m_signed;

public:
	void setup(const QString& name, const QString& units, bool hasSetpoint, bool isEditable, double scale = 1, double offset = 0, bool isSigned = false);
	void reset();
	bool hasUnits() { return m_has_units; }
	bool hasSetpoint() { return m_has_setpoint; }
	bool isEditable() { return m_name_editable; }
	void update(unsigned int raw);
	void updateText();
	unsigned int getRawValue();
	double getValue();
	QString getName() { return m_name_string; }
	~StateVariable();

private:
	QString m_name_string;
	QString m_units_string;
	QWidget* m_name{ nullptr };
	QLabel* m_value{ nullptr };
	QDoubleSpinBox* m_setpoint{ nullptr };
	QLabel* m_unit{ nullptr };

	friend class StateGroup;
	friend class MainWindow;
};

