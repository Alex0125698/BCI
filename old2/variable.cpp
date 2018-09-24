#include "variable.h"

Unit::Unit(Dimension dim, std::string& name) : m_dim{ dim }, m_name{ name } {}
Unit::Unit(Dimension dim, std::string&& name) : Unit{ dim, name } {}
Unit::Unit(Dimension dim, std::string& name, double scale, double offset) : m_dim{ dim }, m_name{ name }, m_scale{ scale }, m_offset{ offset } { assert(m_scale != 0); }
Unit::Unit(Dimension dim, std::string&& name, double scale, double offset) : Unit{ dim, name, scale, offset } {}

std::vector<Unit> Unit::SI
{
	{ Dimension::NONE, "no unit" } ,{ Dimension::LENGTH, "m" } ,
	{ Dimension::MASS, "kg" }      ,{ Dimension::TIME, "s" } ,
	{ Dimension::AMPERE, "A" }     ,{ Dimension::TEMPERATURE, "K" } ,
	{ Dimension::LUMINOSITY, "cd" },{ Dimension::AMOUNT, "mol" } ,
	{ Dimension::ANGLE, "rad" },{ Dimension::SOLID_ANGLE, "rad2" } ,
	{ Dimension::FREQUENCY, "Hz" },{ Dimension::FORCE, "N" } ,
	{ Dimension::PRESSURE, "Pa" },{ Dimension::VELOCITY, "mps2" } ,
	{ Dimension::AREA, "m2" },{ Dimension::VOLUME, "m3" } ,
	{ Dimension::ACCELERATION, "mps3" },{ Dimension::VOLTAGE, "V" },
	{ Dimension::ENERGY, "J" }
};

std::vector<Unit> Unit::Other
{
	{ Dimension::MASS, "g", 0.001, 0 },{ Dimension::LENGTH, "mm", 1000, 0 },
	{ Dimension::TEMPERATURE, "deg C", 1.0, -273 },{ Dimension::FORCE, "kG", 1.0/9.8, 0 },
	{ Dimension::FORCE, "kN", 0.001, 0 },{ Dimension::TIME, "m", 1.0/60.0, 0 }
};

Variable::Variable(const std::string&& name, const Dimension dim, const double val, const double min, const double max, const double step, const std::vector<Tag>&& tags)
	: Variable{ name, dim, val, min, max, step, tags }
{
}

Variable::Variable(const std::string& name, const Dimension dim, const double val, const double min, const double max, const double step, const std::vector<Tag>& tags)
	: m_name{ name }, m_dim{ dim }, m_min{ min }, m_max{ max }, Taggable{ tags }, m_data_si{ val }, m_step_size{ step }
{
	load_default_units();
	assert(m_units.size() > (size_t)0);
}

Variable::Variable(const std::string&& name, const Dimension dim, const std::vector<Tag>&& tags)
	: Variable{ name, dim, 0.0, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), 1.0, tags }
{
}

Variable::Variable(const std::string && name, const std::vector<Tag>&& tags)
	: Variable{ std::move(name), Dimension::NONE, std::move(tags) }
{
}

// set value using the current unit

void Variable::setValue(const double valueInCurrentUnit)
{
	auto tmp = std::clamp(valueInCurrentUnit, m_min, m_max);
	m_data_si = m_units[m_current_unit]->convertToUnit(tmp);
	emit sigValueChanged();
}

const std::vector<std::string> Variable::getUnitNames() const
{
	std::vector<std::string> tmp;
	tmp.reserve(m_units.size());

	for (auto& unit : m_units)
		tmp.push_back(unit->getName());

	return std::move(tmp);
}

// set the current unit

void Variable::slotSetActiveUnit(const int index)
{
	assert(size_t(index) < m_units.size());
	m_current_unit = size_t(index);
	emit sigUnitsChanged();
}

void Variable::setActiveUnit(const std::string& name)
{
	for (size_t i =0; i<m_units.size(); ++i)
	{
		if (m_units[i]->getName() == name)
		{
			slotSetActiveUnit((int)i);
			return;
		}
	}
	assert(false); // failed to find unit
}

// TODO: check for duplicate names
void Variable::load_default_units()
{
	assert(Unit::Other.size() > 0);
	assert(Unit::SI.size() == size_t(Dimension::END_OF_DATA));
	m_units.reserve(Unit::SI.size());
	
	for (auto& si_unit : Unit::SI)
	{
		if (si_unit.getDimension() == this->m_dim)
			m_units.push_back(&si_unit);
	}

	for (auto& other_unit : Unit::Other)
	{
		if (other_unit.getDimension() == this->m_dim)
			m_units.push_back(&other_unit);
	}
}

// check if it has a particular tag

bool Taggable::checkTag(const Tag tag) const
{
	for (const auto& check : m_tags)
		if (tag == check) return true;
	return false;
}

// check if it has any of the given tags

bool Taggable::checkTags(const std::vector<Tag> tags) const
{
	for (const auto& check : tags)
		if (checkTag(check)) return true;
	return false;
}
