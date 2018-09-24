/**
* enum Dimension - this lists many standard dimensions. The appropriate conversions to SI units
*     done in variable.cpp.
*
* Unit class - this stores a unit name and conversion info (to and from SI units)
*
* enum Tag - these indicate properties about variables
* 
* Taggable class - abtract class representing a class that has a list of tags
*
* Error Handling - must connect to sigError and print out error messages. must stop controller
* if there is an error
*
*
* TODO:
*** add more standars units (14/AUG/18)
*** support for custom conversions (14/AUG/18)
*** should tags be strings??? (14/AUG/18)
*
* Author:  A.S. Woodcock
* Project: BCI Controller for Biomedical Applications
* Last Modified: 14/AUG/18 by A.S. Woodcock
*/

#pragma once

#include "resources.h"

/* variables are used to store data about physical quantities
 * They are assumed to be in SI units. Variables have a dimension
 * They have a Units array which will be filled with units of the appropriate dimension
 * They also may have a raw unit which represents the raw data from the sensor device */

enum class Dimension
{
	NONE,
	// base dimensions
	LENGTH,
	MASS,
	TIME,
	AMPERE,
	TEMPERATURE,
	LUMINOSITY,
	AMOUNT,
	// derived dimensions
	ANGLE,
	SOLID_ANGLE,
	FREQUENCY,
	FORCE,
	PRESSURE,
	VELOCITY,
	AREA,
	VOLUME,
	ACCELERATION,
	VOLTAGE,
	ENERGY,
	END_OF_DATA
};

class Unit
{
public:
	// Either set your scale + offset OR default to 1,0 respectively
	Unit() = default;
	Unit(Dimension dim, std::string& name);
	Unit(Dimension dim, std::string&& name);
	Unit(Dimension dim, std::string& name, double scale, double offset);
	Unit(Dimension dim, std::string&& name, double scale, double offset);
	// we may need the unit name to fill combo boxes etc.
	const auto& getName() const { return m_name; }
	// need to check apropriate uints based on its dimension
	const auto getDimension() const { return m_dim; }
	// the most important function - convert from SI to this unit
	const double convertToUnit(const double val) const { return val * m_scale + m_offset; }
	// if we only have a converted value, use this to find the SI version
	const double convertToSI(const double val) const { return (val - m_offset) / m_scale; }

private:
	Dimension m_dim{ Dimension::NONE };
	std::string m_name;
	double m_scale{ 1.0 };
	double m_offset{ 0.0 };
	
public:
	// use these for common SI units, otherwise you can make your own
	static std::vector<Unit> SI;
	static std::vector<Unit> Other;
};

enum class Tag
{
	DATA_CHANNEL,
	FREQ_BAND,
	MCU_INPUT,
	MCU_OUTPUT,
	USER_INPUT,
	PID_CONTROL,
	CONTROL,
	PID_SIGNAL,
	PHYSICS_CALC,
	EDITABLE,
	SAVABLE,
	STATUS_BAR,
	XVA,
	LIMITS,
	END_OF_DATA
};

class Taggable : public QObject
{
	Q_OBJECT

public:
	Taggable(const std::vector<Tag>& tags) : m_tags { tags }{ }
	Taggable(const std::vector<Tag>&& tags) : m_tags{ std::move(tags) } { }
	virtual ~Taggable() = default;
	// check if it has a particular tag
	bool checkTag(const Tag tag) const;
	// check if it has any of the given tags
	bool checkTags(const std::vector<Tag> tags) const;

private:
	const std::vector<Tag> m_tags;
};

// TODO: support for changing name
class Variable : public Taggable
{
	Q_OBJECT

public:
	Variable(const std::string&& name, const Dimension dim, const double val, const double min, const double max, const double step, const std::vector<Tag>&& tags);
	Variable(const std::string& name, const Dimension dim, const double val, const double min, const double max, const double step, const std::vector<Tag>& tags);
	Variable(const std::string&& name, const Dimension dim, const std::vector<Tag>&& tags);
	Variable(const std::string&& name, const std::vector<Tag>&& tags);

public:
	void setUnit()
	{
		m_current_unit = m_tmp_current_unit.load();
	}
	auto getData(size_t index) const
	{
		index;
		//return m_data_si;
	}
	// get the plain SI base data at index location
	auto getSIData(size_t index) const 
	{
		index;
	}

public:

	// get the value in the current unit
	double getValue() const
	{ return m_units[m_current_unit]->convertToUnit(m_data_si); }
	// set value using the current unit
	void setValue(const double valueInCurrentUnit);
	// add any unit - Dimension needs to be same as unit or NONE
	void addUnit(Unit* unit)
	{
		m_units.emplace_back(unit);
	}
	// get a vector of all unit names
	const std::vector<std::string> getUnitNames() const;
	// get the active unit name
	auto getActiveUnitName() const
	{ return m_units[m_current_unit]->getName(); }
	auto getSIUnitName() const
	{
		return m_units[0]->getName();
	}
	// set the current unit
	void setActiveUnit(const std::string& name);
	void setActiveUnit(const std::string&& name)
	{ setActiveUnit(name); }
	// get max in current unit
	double getMax() const
	{
		// TODO: check for overflow
		if (m_max == std::numeric_limits<double>::max()) return m_max;
		return m_units[m_current_unit]->convertToUnit(m_max);
	}
	// get min in current unit
	double getMin() const
	{
		// TODO: check for underflow
		if (m_min == std::numeric_limits<double>::min()) return m_min;
		return m_units[m_current_unit]->convertToUnit(m_min);
	}
	double getStepSize() const
	{
		// TODO: could be more efficient by adding a function to unit
		auto tmp1 = m_units[m_current_unit]->convertToUnit(m_step_size);
		auto tmp2 = m_units[m_current_unit]->convertToUnit(m_step_size*2);
		return tmp2-tmp1;
	}
	auto& getName() const
	{ return m_name; };
	auto& getDimension() const
	{ return m_dim; };
	void refresh()
	{ emit sigValueChanged(); }

public slots:
	void slotSetActiveUnit(const int index);
	void slotSetValue(double val)
	{
		setValue(val);
	}
	void slotSetName(QString name)
	{
		m_name = name.toStdString();
		emit sigNameChanged();
	}
	
signals:
	void sigValueChanged();
	void sigNameChanged();
	void sigUnitsChanged();

private:
	// add SI units to m_units if they have the same dimension
	void load_default_units();

private:
	std::array<double, 512> m_data_queue;
	std::atomic<size_t> m_start_index;


	// this is where the actual data is stored
	std::atomic<double> m_data_si{ 0.0 };
	// the variable name - must be given
	std::string m_name;
	// index of current unit - default is SI base
	std::atomic<size_t> m_current_unit{ 0 };
	std::atomic<size_t> m_tmp_current_unit{ 0 };
	// the minimum possible value the quantity can take
	const double m_min;
	// the maximum possible value the quantity can take
	const double m_max;
	// step size -  based on raw data representation
	// TODO: implement this
	const double m_step_size = 1;
	// the dimension of the physical quantity
	const Dimension m_dim{ Dimension::NONE };
	// this is populated with units with matching dimensions
	std::vector<Unit*> m_units;
};