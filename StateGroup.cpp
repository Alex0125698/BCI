#include "StateGroup.h"

#include <QGridLayout>
#include "StateVariable.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QFrame>

// #include 

StateGroup::StateGroup(QWidget* parent)
	:  QWidget(parent)
{
}

void StateGroup::addData(StateVariable stateArr[], unsigned int length, QString GroupName, QString col1, QString col2, QString col3, QString col4)
{
	// ===== Main GroupBox Title =====
	//this->setTitle(GroupName);

	// ===== Should we add a setpoint column ??? =====
	bool groupHasSetpoint = false;
	for (int i = 0; i < length; ++i)
	{
		if (stateArr[i].hasSetpoint())
		{
			groupHasSetpoint = true;
			break;
		}
	}
	QFont BankGothic("BankGothic Md BT");
	QVBoxLayout* layout_main = new QVBoxLayout;
	layout_main->setSpacing(0);

	QGridLayout* layout = new QGridLayout;

	QFrame* line_top = new QFrame();
	line_top->setFrameShape(QFrame::HLine);
	line_top->setFrameShadow(QFrame::Plain);
	layout_main->addWidget(line_top);

	QString style_title = "<html><head/><body><p><span style=\" font-size:15pt;\">%1</span></p></body></html>";
	QLabel* lbl_title = new QLabel(style_title.arg(GroupName));
	lbl_title->setAlignment(Qt::AlignHCenter);
	lbl_title->setFont(BankGothic);
	lbl_title->setStyleSheet("background-color: rgb(226, 215, 204);");
	layout_main->addWidget(lbl_title);


	QFrame* line_bottom = new QFrame();
	line_bottom->setFrameShape(QFrame::HLine);
	line_bottom->setFrameShadow(QFrame::Plain);
	layout_main->addWidget(line_bottom);

	if (length > 0)
	{
		// ===== Add Column titles =====

		QString style = "<html><head/><body><p><span style=\" font-size:12.5pt; color:#583f22;\">%1</span></p></body></html>";
		// add column titles

		QLabel* lbl_c1 = new QLabel;
		lbl_c1->setText(style.arg(col1));
		lbl_c1->setFont(BankGothic);
		//lbl_c1->setAlignment(Qt::AlignHCenter);
		layout->addWidget(lbl_c1, 0, 0);
		QLabel* lbl_c2 = new QLabel;
		lbl_c2->setText(style.arg(col2));
		lbl_c2->setFont(BankGothic);
		lbl_c2->setAlignment(Qt::AlignHCenter);
		layout->addWidget(lbl_c2, 0, 1);
		if (groupHasSetpoint)
		{
			QLabel* lbl_c3 = new QLabel;
			lbl_c3->setText(style.arg(col3));
			layout->addWidget(lbl_c3, 0, 2);
			lbl_c3->setAlignment(Qt::AlignHCenter);
			lbl_c3->setFont(BankGothic);
			QLabel* lbl_c4 = new QLabel;
			lbl_c4->setText(style.arg(col4));
			layout->addWidget(lbl_c4, 0, 3);
			lbl_c4->setAlignment(Qt::AlignHCenter);
			lbl_c4->setFont(BankGothic);
		}
		else
		{
			QLabel* lbl_c4 = new QLabel;
			lbl_c4->setText(style.arg(col4));
			layout->addWidget(lbl_c4, 0, 2);
			lbl_c4->setAlignment(Qt::AlignHCenter);
			lbl_c4->setFont(BankGothic);
		}

		// ===== Add the Widgets =====
		for (int i = 0; i < length; ++i)
		{
			layout->addWidget(stateArr[i].m_name, i + 1, 0);
			layout->addWidget(stateArr[i].m_value, i + 1, 1);

			if (groupHasSetpoint)
			{
				layout->addWidget(stateArr[i].m_setpoint, i + 1, 2);
				layout->addWidget(stateArr[i].m_unit, i + 1, 3);
			}
			else
			{
				layout->addWidget(stateArr[i].m_unit, i + 1, 2);
			}
		}
	}

	layout->setSpacing(1);
	this->setMinimumSize(this->sizeHint());
	//this->setMinimumSize(this->minimumSize()+QSize(10, 250));
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	layout_main->addLayout(layout);
	this->setLayout(layout_main);
}

StateGroup::~StateGroup()
{
}
