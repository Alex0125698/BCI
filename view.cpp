#include "view.h"
#include "state.h"
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
				connect(tmp, &QLineEdit::editingFinished, var, [tmp, var]() {var->slotSetName(tmp->text()); }, Qt::QueuedConnection);
				connect(var, &Variable::sigNameChanged, tmp, [tmp, var]() {tmp->setText(QString::fromStdString(var->getName())); }, Qt::QueuedConnection);
				m_layout->addWidget(tmp);
				tmp->setMaximumWidth(70);
				tmp->setMinimumWidth(70);
			}
			else
			{
				auto tmp = new QLabel(QString::fromStdString(var->getName()));
				tmp->setAlignment(Qt::AlignHCenter);
				connect(var, &Variable::sigNameChanged, tmp, [tmp, var]() {tmp->setText(QString::fromStdString(var->getName())); }, Qt::QueuedConnection);
				m_layout->addWidget(tmp);
				tmp->setMaximumWidth(66);
				tmp->setMinimumWidth(66);
			}
		}

		// ----- ADD VARIABLES -----
		if (var->checkTag(Tag::USER_INPUT))
		{
			auto tmp = new QDoubleSpinBox;
			tmp->setMaximumWidth(60);
			tmp->setMinimumWidth(60);
			tmp->setMaximum(var->getMax());
			tmp->setMinimum(var->getMin());
			tmp->setSingleStep(var->getStepSize());
			tmp->setValue(var->getValue());
			tmp->setDecimals(3);
			connect(var, &Variable::sigUnitsChanged, [tmp,var]()
			{
				auto val = var->getValue();

				if (val > tmp->maximum())
				{
					tmp->setMaximum(var->getMax());
					tmp->setValue(val);
					tmp->setMinimum(var->getMin());
				}
				else if (val < tmp->minimum())
				{
					tmp->setMinimum(var->getMin());
					tmp->setValue(val);
					tmp->setMaximum(var->getMax());
				}
				else
				{
					tmp->setMinimum(var->getMin());
					tmp->setMaximum(var->getMax());
					tmp->setValue(val);
				}
				tmp->setValue(val);
				
				tmp->setSingleStep(var->getStepSize());
				
			});
			connect(tmp, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), var, &Variable::slotSetValue, Qt::QueuedConnection);
			connect(&bci::State::program, &bci::State::sigVarReset, tmp, [tmp,var]() {tmp->setValue(var->getValue()); });
			// TODO: connect var::value changed sig
			m_layout->addWidget(tmp);
		}
		else
		{
			auto tmp = new QLabel("N/A");
			tmp->setMaximumWidth(46);
			tmp->setMinimumWidth(46);
			connect(var, &Variable::sigValueChanged, [var,tmp]() {tmp->setText(QString::number(var->getValue(), 'f', 3)); });
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
			currUnitsBox->setMaximumWidth(55);
			currUnitsBox->setMinimumWidth(55);
			for (auto& name : vars[0]->getUnitNames()) currUnitsBox->addItem(QString::fromStdString(name));
			if (!singleUnit) m_layout->addWidget(currUnitsBox);
		}
		connect(currUnitsBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), var, &Variable::slotSetActiveUnit, Qt::QueuedConnection);
	}

	if (singleUnit) m_layout->addWidget(currUnitsBox);
}

SliderView::SliderView(const std::vector<Variable*>& vars, const std::vector<QString>&& titles)
	: View{ vars, std::move(titles) }
{
	assert(vars.size() == 1);
	m_layout = new QHBoxLayout;

	auto var = vars[0];
	// ensure that we have set the limits
	assert(var->getMax() != std::numeric_limits<double>::max());
	assert(var->getMin() != std::numeric_limits<double>::min());

	// ----- TITLE -----
	
	auto title = new QLabel(QString::fromStdString(var->getName() + " (%)"));
	title->setMaximumWidth(70);
	title->setMinimumWidth(70);
	title->setAlignment(Qt::AlignHCenter);
	connect(var, &Variable::sigNameChanged, title, [title, var]() {title->setText(QString::fromStdString(var->getName() + " (%)")); });
	// TODO: connect value changed
	m_layout->addWidget(title);

	QSlider* tmp = new QSlider(Qt::Horizontal, nullptr);
	tmp->setMaximumWidth(140);
	tmp->setMinimumWidth(140);
	tmp->setMaximum(1000);
	tmp->setMinimum(0);
	tmp->setValue(int(1000.0*(var->getValue() - var->getMin()) / (var->getMax() - var->getMin())));
	connect(tmp, &QSlider::valueChanged, this, [tmp, var]() {var->setValue((tmp->value()/1000.0)*(var->getMax() - var->getMin()) + var->getMin()); });
	connect(&bci::State::program, &bci::State::sigVarReset, tmp, [tmp, var]() {tmp->setValue(int(1000.0*(var->getValue() - var->getMin())/ (var->getMax() - var->getMin()))); });
	tmp->setTickInterval(100);
	tmp->setSingleStep(1);
	m_layout->addWidget(tmp);
}

ButtonsView::ButtonsView(const std::vector<Variable*>& vars, const std::vector<QString>&& titles)
	: View{ vars, std::move(titles) }
{
	assert(vars.size() == 1);
	assert(titles.size() > 0);
	m_layout = new QVBoxLayout;

	auto var = vars[0];
	
	// ----- TITLE -----

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
		auto lbl = new QLabel(QString::fromStdString(var->getName()));
		lbl->setAlignment(Qt::AlignHCenter);
		connect(var, &Variable::sigNameChanged, lbl, [lbl, var]() {lbl->setText(QString::fromStdString(var->getName())); });
		m_layout->addWidget(lbl);

	}

	// ----- BUTTONS -----

	auto buttonsLayout = new QGridLayout;
	for (size_t i=0; i<titles.size(); ++i)
	{
		auto tmp = new QRadioButton;
		tmp->setText(titles[i]);
		tmp->setChecked(i==var->getValue());
		buttonsLayout->addWidget(tmp, i / 3, i % 3);
		// TODO: set value
		connect(tmp, &QRadioButton::toggled, [tmp, var, i]() {if (tmp->isChecked()) var->setValue(i); });
		// TODO: reset val ??
	}
	reinterpret_cast<QVBoxLayout*>(m_layout)->addLayout(buttonsLayout);
}

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
		widget->setFont(QFont("BankGothic Md BT"));
		if (background != "")
			widget->setStyleSheet(QString("background-color: %1;").arg(background));
		return widget;
	};

	// ----- TITLE -----
	if (groupName != "")
	{
		//addLine();
		titleLayout->addWidget(createLabel(groupName, 13, "D9E3F7", "rgb(93,115,160)"));
		//addLine();
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
