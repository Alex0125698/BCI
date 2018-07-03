#include <cmath>
#include <QtWidgets>
#include <QFont>
#include "statevariable.h"
#include "error.h"


// ===== Unit =====


Unit::Unit(QString name, double scale, double offset, bool is_signed)
	: m_name{ name }, m_scale { scale }, m_offset{ offset }, m_signed{ is_signed }
{
}

double Unit::convert(uint32_t raw, uint32_t size_in_bytes)
{
	if (m_signed)
	{
		switch (size_in_bytes)
		{
		case 1:
			return int8_t(raw)*m_scale + m_offset;
		case 2:
			return int16_t(raw)*m_scale + m_offset;
		case 3:
			if (raw & 0x00800000)
				return int32_t(raw | 0xFF000000)*m_scale + m_offset;
			else
				return int32_t(raw)*m_scale + m_offset;
		case 4:
			return int32_t(raw)*m_scale + m_offset;
		default:
			throw DETAILEDEXCEPTION("unsupported raw variable length");
		}
	}
	else
	{
		return raw*m_scale + m_offset;
	}
}

uint32_t Unit::getRaw(double value, uint32_t size_in_bytes)
{
	value = (value-m_offset) / m_scale;
	uint32_t result = round(value);

	if (m_signed)
	{
		switch (size_in_bytes)
		{
		case 1:
			return int32_t(int8_t(result));
		case 2:
			return int32_t(int16_t(result));
		case 3:
			if (result & 0x008000000)
				return 0xFF000000 | result;
			else
				return result;
		case 4:
			return result;
		default:
			throw DETAILEDEXCEPTION("unsupported raw variable length");
		}
	}
	return result;
}


// ===== StateVariable =====


StateVariable::StateVariable(QString name, uint32_t size_in_bytes, const uint32_t* const in, uint32_t min_raw, uint32_t max_raw, uint32_t* const out, bool name_editable)
	: m_name{ name }, m_size_in_bytes{ size_in_bytes }, m_raw_in_ptr{ in }, m_raw_out_ptr{ out }, m_name_editable{ name_editable }, m_min_raw{ min_raw }, m_max_raw{ max_raw }
{
	// default raw units
	units.emplace_back(QString("raw"), 1.0, 0.0, false);
}

double StateVariable::getValue(int unit_index)
{
	assert(isInput());
	assert(unit_index < units.size());
	return units[unit_index].convert(*m_raw_in_ptr, m_size_in_bytes);
}

double StateVariable::getSetpoint(int unit_index)
{
	assert(isOutput());
	assert(unit_index < units.size());
	return units[unit_index].convert(*m_raw_out_ptr, m_size_in_bytes);
}

double StateVariable::getMinimum()
{
	return units[m_active_unit].convert(m_min_raw, m_size_in_bytes);
}

double StateVariable::getStepSize()
{
	return units[m_active_unit].convert(2,m_size_in_bytes)-units[m_active_unit].convert(1,m_size_in_bytes);
}

void StateVariable::setName(QString str)
{
	assert(m_name_editable);
	m_name = str;
	emit sigNameChanged();
}

void StateVariable::updateValue()
{
	if (m_reset)
	{
		m_reset = false;
		if (isOutput())
			setSetpoint(getValue());
	}
	emit sigValueChanged();
}

void StateVariable::setSetpoint(double desiredValue)
{
	assert(isOutput());
	*m_raw_out_ptr = units[m_active_unit].getRaw(desiredValue, m_size_in_bytes);
	emit sigSetpointChanged();
}

void StateVariable::setActiveUnit(int unit_index)
{
	m_active_unit = unit_index;
	emit sigUnitsChanged();
}


// ===== StateView =====


StateView::StateView(StateVariable& var, TYPE type, std::vector<QString> names)
	: m_var_ref{var}, m_type{type}
{
	QFont BankGothic("BankGothic Md BT");

	// ----- var name -----
	if (var.isEditable())
	{
		QLineEdit* tmp = new QLineEdit;
		tmp->setAlignment(Qt::AlignHCenter);
		connect(tmp, SIGNAL(textChanged(QString)), &m_var_ref, SLOT(setName(QString)));
		m_name = tmp;
	}
	else
	{
		// TODO: needs to be extra large for BUTTONS or SLIDER
		QLabel* tmp = new QLabel;
		tmp->setAlignment(Qt::AlignHCenter);
		m_name = tmp;
	}
	connect(&var, &StateVariable::sigNameChanged, this, &StateView::slotNameChanged);

	// ----- value -----
	if (var.isInput() && (type != BUTTONS))
	{
		QLabel* tmp = new QLabel(" N/A ");
		tmp->setAlignment(Qt::AlignHCenter);
		m_value = tmp;
		connect(&var, &StateVariable::sigValueChanged, this, &StateView::slotValueChanged);
	}

	// ----- setpoint -----
	if (var.isOutput())
	{
		if (type == TABLE)
		{
			QDoubleSpinBox* tmp = new QDoubleSpinBox;
			//connect(tmp, &QDoubleSpinBox::editingFinished, this, &StateView::slotGetSetpoint);
			connect(tmp, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &StateView::slotGetSetpoint);
			m_setpoint = tmp;
		}
		else if (type == BUTTONS)
		{
			auto num_buttons = size_t(size_t(1)+round((var.getMaximum()-var.getMinimum())/var.getStepSize()));
			if (num_buttons > size_t(16)) num_buttons = size_t(16);
			if (names.size() < num_buttons) num_buttons = names.size();
			m_setpoint = new QWidget;
			QGridLayout* tmp = new QGridLayout;
			m_setpoint->setLayout(tmp);

			for (uint32_t i=0; i<num_buttons; ++i)
			{
				auto btn = new QRadioButton(names[i]);
				tmp->addWidget(btn, i/4, i%4);
				// Better Solution Needed
				// for now, set flag in class
				//connect(btn, &QRadioButton::toggled, [i,this](bool b){if (b) {m_var_ref.setValue(i);}});
			}
		}
		else if (type == SLIDER)
		{
			QSlider* tmp = new QSlider(Qt::Horizontal, nullptr);
			connect(tmp, &QSlider::valueChanged, this, &StateView::slotValueChanged);
			//connect(tmp, static_cast<void (QSlider::*)(double)>(&QSlider::valueChanged), [&](double d){m_var_ref.setValue(d);});
			//tmp->setTickPosition(QSlider::TicksBothSides);
			tmp->setTickInterval(10);
			tmp->setSingleStep(1);
			m_setpoint = tmp;
		}
		connect(&var, &StateVariable::sigSetpointChanged, this, &StateView::slotSetpointChanged);
	}

	// ----- units -----
	QComboBox* tmp = new QComboBox;
	for (auto& unit : m_var_ref.units)
		tmp->addItem(unit.getName());
	m_units = tmp;

	connect(&var, &StateVariable::sigUnitsChanged, this, &StateView::slotUnitsChanged);
	connect(tmp, SIGNAL(currentIndexChanged(int)), &m_var_ref, SLOT(setActiveUnit(int)));

	// ----- layout -----
	if (type == TABLE)
	{
		layout = new QHBoxLayout;
		layout->addWidget(m_name);
		if (var.isInput())
			layout->addWidget(m_value);
		if (var.isOutput())
			layout->addWidget(m_setpoint);
		layout->addWidget(m_units);

		setSizing({64,50,64,64});
	}
	else if (type == BUTTONS)
	{
		QVBoxLayout* mainLayout = new QVBoxLayout;
		QHBoxLayout* topLayout = new QHBoxLayout;
		QHBoxLayout* bottomLayout = new QHBoxLayout;
		topLayout->addWidget(m_name);
		bottomLayout->addWidget(m_setpoint);

		mainLayout->addLayout(topLayout);
		mainLayout->addLayout(bottomLayout);

		layout = mainLayout;
	}
	else if (type == SLIDER)
	{
		QVBoxLayout* mainLayout = new QVBoxLayout;
		QHBoxLayout* topLayout = new QHBoxLayout;
		QHBoxLayout* bottomLayout = new QHBoxLayout;

		topLayout->addWidget(m_name);
		if (var.isInput())
			topLayout->addWidget(m_value);
		if (var.isOutput())
			bottomLayout->addWidget(m_setpoint);
		bottomLayout->addWidget(m_units);

		mainLayout->addLayout(topLayout);
		mainLayout->addLayout(bottomLayout);

		layout = mainLayout;
	}

	slotNameChanged();
	//slotValueChanged();
	if (var.isOutput())
		slotSetpointChanged();
	slotUnitsChanged();
}

std::vector<int> StateView::getSizingInfo()
{
	return std::vector<int>();
}

void StateView::setSizing(std::vector<int> sizes)
{
	if (getType() == TABLE)
	{
		if (sizes.size() != 4)
			throw DETAILEDEXCEPTION("TableView sizing requires 4 sizes");

		m_name->setMaximumWidth(sizes[0]);
		m_name->setMinimumWidth(sizes[0]);

		if (m_var_ref.isInput())
		{
			m_value->setMaximumWidth(sizes[1]);
			m_value->setMinimumWidth(sizes[1]);
		}

		if (m_var_ref.isOutput())
		{
			m_setpoint->setMaximumWidth(sizes[2]);
			m_setpoint->setMinimumWidth(sizes[2]);
		}

		m_units->setMaximumWidth(sizes[3]);
		m_units->setMinimumWidth(sizes[3]);
	}
}

StateView::TYPE StateView::getType()
{
	return m_type;
}

void StateView::slotGetSetpoint()
{
	assert(m_var_ref.isOutput());

	double out = 0;

	if (getType() == TABLE)
	{
		QDoubleSpinBox* tmp = reinterpret_cast<QDoubleSpinBox*>(m_setpoint);
		out = tmp->value();
	}
	else if (getType() == BUTTONS)
	{
		// TODO: new features needed
	}
	else if (getType() == SLIDER)
	{
		QSlider* tmp = reinterpret_cast<QSlider*>(m_setpoint);
		// TODO: slider mapped value
		out = tmp->value();
	}

	m_var_ref.setSetpoint(out);
}

void StateView::slotNameChanged()
{
	if (m_var_ref.isEditable())
	{
		QLineEdit* tmp = reinterpret_cast<QLineEdit*>(m_name);
		tmp->setText(m_var_ref.getName());
	}
	else
	{
		QLabel* tmp = reinterpret_cast<QLabel*>(m_name);
		tmp->setText(m_var_ref.getName());
	}
}

void StateView::slotValueChanged()
{
	assert(m_var_ref.isInput());
	QLabel* tmp = reinterpret_cast<QLabel*>(m_value);
	tmp->setText(QString::number(m_var_ref.getValue()));
}

void StateView::slotSetpointChanged()
{
	assert(m_var_ref.isOutput());

	// need to update display values, limits, step
	if (getType() == TABLE)
	{
		QDoubleSpinBox* tmp = reinterpret_cast<QDoubleSpinBox*>(m_setpoint);
		tmp->setValue(m_var_ref.getSetpoint());
	}
	else if (getType() == BUTTONS)
	{
		//TODO: need new features for this
	}
	else if (getType() == SLIDER)
	{
		QSlider* tmp = reinterpret_cast<QSlider*>(m_setpoint);
		// TODO: slider mapped value
		//tmp->setValue(m_var_ref.getValue());
	}
}

void StateView::slotUnitsChanged()
{
	QComboBox* tmp = reinterpret_cast<QComboBox*>(m_units);
	tmp->setCurrentIndex(m_var_ref.getActiveUnit());

	// need to update display values, limits, step
	if (getType() == TABLE)
	{
		if (m_var_ref.isInput())
		{
			QLabel* tmp = reinterpret_cast<QLabel*>(m_value);
			tmp->setText(QString::number(m_var_ref.getValue()));
		}
		if (m_var_ref.isOutput())
		{
			QDoubleSpinBox* tmp = reinterpret_cast<QDoubleSpinBox*>(m_setpoint);
			tmp->setValue(m_var_ref.getSetpoint());
			tmp->setMaximum(m_var_ref.getMaximum());
			tmp->setMinimum(m_var_ref.getMinimum());
			tmp->setSingleStep(m_var_ref.getStepSize());
		}
	}
	else if (getType() == BUTTONS)
	{
		// buttons should only have raw units
	}
	else if (getType() == SLIDER)
	{
		// slider has setpoint only
		if (m_var_ref.isOutput())
		{
			QSlider* tmp = reinterpret_cast<QSlider*>(m_setpoint);
			// TODO: slider mapped value
			//tmp->setValue(m_var_ref.getValue());
		}
	}
}


// ===== StateGroup =====


StateGroup::StateGroup(QWidget* parent)
	:  QWidget(parent)
{
	static int id=0;
	QString name = QString("StateGroup%1").arg(id);
	this->setObjectName(name);
	//this->setStyleSheet("background-color: rgba(150,150,170,130);");//QString("QWidget#%1 { background-color: rgba(150,150,170,130); }").arg(name));
	++id;
}

void StateGroup::setup(std::vector<QString> colNames, QString groupName)
{
	QVBoxLayout* mainLayout = new QVBoxLayout;
	QVBoxLayout* titleLayout = new QVBoxLayout;
	QHBoxLayout* colTitleLayout = new QHBoxLayout;
	QFont BankGothic("BankGothic Md BT");

	auto addLine = [&]() {
		QFrame* line_top = new QFrame();
		line_top->setFrameShape(QFrame::HLine);
		line_top->setFrameShadow(QFrame::Plain);
		titleLayout->addWidget(line_top);
		// line color
	};

	auto createLabel = [&](QString text, uint32_t size, QString stroke, QString background, uint32_t width = 0) {
		QString style = "<html><head/><body><p><span style=\" font-size:%1pt;color:#%2\">%3</span></p></body></html>";
		QLabel* widget = new QLabel(style.arg(size).arg(stroke).arg(text));
		widget->setAlignment(Qt::AlignHCenter);
		widget->setFont(BankGothic);
		if (width > 0)
		{
			widget->setMaximumWidth(width);
			widget->setMaximumWidth(width);
		}
		//lbl_title->setFont(BankGothic);
		if (background != "")
			widget->setStyleSheet(QString("background-color: %1;").arg(background));
		return widget;
	};

	// ----- TITLE -----
	if (groupName != "")
	{
		addLine();
		titleLayout->addWidget(createLabel(groupName, 13, "583f22", "rgb(226, 215, 204)"));
		addLine();
		titleLayout->setSpacing(0);
		mainLayout->addLayout(titleLayout);
	}

	// ----- COLUMN LABELS -----

	if (colNames.size() > 0)
	{
		for (auto& str : colNames)
		{
			colTitleLayout->addWidget(createLabel(str, 11, "000000", "", 56));
		}
		mainLayout->addLayout(colTitleLayout);
	}

	// ----- SIZING -----
	// widget.setFixedSize(x,y)

	// ----- VARIABLES -----
	for (auto& view : m_views)
	{
		mainLayout->addLayout(view->layout);
	}

	mainLayout->setSpacing(4);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
	this->setMinimumSize(this->sizeHint());
}
