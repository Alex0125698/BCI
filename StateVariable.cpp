#include "StateVariable.h"

#include "resources.h"
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QSpacerItem>

void StateVariable::setup(const QString& name, const QString& units, bool hasSetpoint, bool isEditable, double scale, double offset, bool isSigned)
{
	m_scale = scale;
	m_offset = offset;
	m_signed = isSigned;
	m_units_string = units;

	assert(name != "");

	if (units != "") m_has_units = true;
	m_has_setpoint = hasSetpoint;
	m_name_editable = isEditable;
	m_name_string = name;

	if (m_name_editable)
	{
		auto tmp = new QLineEdit;
		tmp->setText(name);
		m_name = tmp;
	}
	else
	{
		auto tmp = new QLabel;
		tmp->setText(name);
		m_name = tmp;
	}

	m_value = new QLabel;
	m_value->setText("N/A");
	m_value->setAlignment(Qt::AlignHCenter);

	if (m_has_units)
	{
		m_unit = new QLabel;
		m_unit->setText(QString("<html><head/><body><p><span style=\" font-weight:600; \">%1</span></p></body></html>").arg(units));
		m_unit->setAlignment(Qt::AlignHCenter);
	}

	if (m_has_setpoint)
	{
		m_setpoint = new QDoubleSpinBox;
		m_setpoint->setDecimals(0);
		m_setpoint->setRange(0, 65535);
		// set numb decimals
		// set range
	}

}

void StateVariable::reset()
{
	if (m_has_setpoint)
	{
		m_setpoint->setValue(m_raw);
	}
}

void StateVariable::update(unsigned int raw)
{
	m_raw = raw;
}

void StateVariable::updateText()
{
	// set precision & format
	if (m_signed)
		m_value->setText(QString::number(double(short(m_raw) * m_scale + m_offset), 'f', 2));
	else
	{
		if (m_units_string == "V")
			m_value->setText(QString::number(double(m_raw * m_scale + m_offset), 'f', 2));
		else
			m_value->setText(QString::number(double(m_raw * m_scale + m_offset), 'f', 0));
	}
}

unsigned int StateVariable::getRawValue()
{
	return m_raw;
}

double StateVariable::getValue()
{
	return m_raw * m_scale + m_offset;
}

StateVariable::~StateVariable()
{
	// delete stuff
}
