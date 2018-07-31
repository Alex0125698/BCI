#include "view.h"
#include <QtWidgets>

TableView::TableView(const std::vector<Variable*>& vars, const std::vector<QString>&& titles)
	: View{ vars, std::move(titles) }
{
	assert(vars.size() > 0);
	m_layout = new QHBoxLayout;
	//QFont BankGothic("BankGothic Md BT");

	bool singleTitle = std::all_of(++vars.begin(),vars.end(), [&vars](Variable* var){ return var->getName() == vars[0]->getName(); });
	bool singleUnit = std::all_of(++vars.begin(),vars.end(), [&vars](Variable* var){ return var->getDimension() == vars[0]->getDimension(); });

	QComboBox* currUnitsBox = nullptr;
	bool firstTitle = true;

	for (auto& var : vars)
	{
		// ----- ADD TITLES -----
		// TODO: change all names
		if (!singleTitle || firstTitle)
		{
			firstTitle = false;
			if (var->checkTag(Tag::EDITABLE))
			{
				auto tmp = new QLineEdit(QString::fromStdString(var->getName()));
				tmp->setAlignment(Qt::AlignHCenter);
				connect(tmp, &QLineEdit::editingFinished, var, [tmp, var]() {var->slotSetName(tmp->text()); });
				connect(var, &Variable::sigNameChanged, tmp, [tmp, var]() {tmp->setText(QString::fromStdString(var->getName())); });
				m_layout->addWidget(tmp);
			}
			else
			{
				auto tmp = new QLabel(QString::fromStdString(var->getName()));
				tmp->setAlignment(Qt::AlignHCenter);
				connect(var, &Variable::sigNameChanged, tmp, [tmp, var]() {tmp->setText(QString::fromStdString(var->getName())); });
				m_layout->addWidget(tmp);

			}
		}

		// ----- ADD VARIABLES -----
		if (var->checkTag(Tag::USER_INPUT))
		{
			auto tmp = new QDoubleSpinBox;
			connect(var, &Variable::sigUnitsChanged, [tmp,var]()
			{
				tmp->setValue(var->getValue());
				tmp->setMaximum(var->getMax());
				tmp->setMinimum(var->getMin());
				tmp->setSingleStep(var->getStepSize());
			});
			connect(tmp, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), var, &Variable::slotSetValue);
			// TODO: connect var::value changed sig
			m_layout->addWidget(tmp);
		}
		else
		{
			auto tmp = new QLabel("N/A");
			connect(var, &Variable::sigValueChanged, [var,tmp]() {tmp->setText(QString::number(var->getValue())); });
			connect(var, &Variable::sigUnitsChanged, [tmp, var]()
			{
				tmp->setText(QString::number(var->getValue()));
			});
			tmp->setAlignment(Qt::AlignHCenter);
			m_layout->addWidget(tmp);
		}

		// ----- ADD UNITS -----
		if (!singleUnit || currUnitsBox == nullptr)
		{
			currUnitsBox = new QComboBox;
			for (auto& name : vars[0]->getUnitNames()) currUnitsBox->addItem(QString::fromStdString(name));
			if (!singleUnit) m_layout->addWidget(currUnitsBox);
		}
		connect(currUnitsBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), var, &Variable::slotSetActiveUnit);
	}

	if (singleUnit) m_layout->addWidget(currUnitsBox);


	/*
	// ----- value -----
	if (var.isInput() && (type != BUTTONS))
	{
		QLabel* tmp = new QLabel(" N/A ");
		tmp->setAlignment(Qt::AlignHCenter);
		m_value = tmp;
		connect(&var, &StateVariable::sigValueChanged, this, &StateView::slotValueChanged);
	}
	*/

	// ----- setpoint -----
	/*
		if (type == BUTTONS)
		{
			auto num_buttons = size_t(size_t(1) + round((var.getMaximum() - var.getMinimum()) / var.getStepSize()));
			if (num_buttons > size_t(16)) num_buttons = size_t(16);
			if (names.size() < num_buttons) num_buttons = names.size();
			m_setpoint = new QWidget;
			QGridLayout* tmp = new QGridLayout;
			m_setpoint->setLayout(tmp);

			for (uint32_t i = 0; i<num_buttons; ++i)
			{
				auto btn = new QRadioButton(names[i]);
				tmp->addWidget(btn, i / 4, i % 4);
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

		//setSizing({ 64,50,64,64 });
	
	if (type == BUTTONS)
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
	}*/
}

/*
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
*/

void StateGroup::setup(QString groupName, std::vector<QString> colNames, std::vector<View*> views)
{
	QVBoxLayout* mainLayout = new QVBoxLayout;
	QVBoxLayout* titleLayout = new QVBoxLayout;
	QHBoxLayout* colTitleLayout = new QHBoxLayout;

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
		//widget->setFont(QFont("BankGothic Md BT"));
		if (width > 0)
		{
			widget->setMaximumWidth(width);
			widget->setMaximumWidth(width);
		}
		//lbl_title->setFont(QFont("BankGothic Md BT"));
		if (background != "")
			widget->setStyleSheet(QString("background-color: %1;").arg(background));
		return widget;
	};

	// ----- TITLE -----
	if (groupName != "")
	{
		addLine();
		titleLayout->addWidget(createLabel(groupName, 13, "583f22", "rgb(80,100,130)"));
		addLine();
		titleLayout->setSpacing(0);
		mainLayout->addLayout(titleLayout);
	}

	// ----- COLUMN LABELS -----

	if (colNames.size() > 0)
	{
		for (auto& str : colNames)
			colTitleLayout->addWidget(createLabel(str, 11, "000000", "", 56));
		mainLayout->addLayout(colTitleLayout);
	}

	// ----- VARIABLES -----
	for (auto& view : views)
	{
		mainLayout->addLayout(view->getLayout());
	}

	mainLayout->setSpacing(4);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
	this->setMinimumSize(this->sizeHint());
}
