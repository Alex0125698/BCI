/* This class lists out the variables in formatted groups
 * with titles (and a group title)
 *
 * It is Graphical only - no special functions
 *
 * For not, promote to this class in ui file
 */

#pragma once

#include <QGroupBox>

class QObject;
class StateVariable;

class StateGroup : public QWidget
{
	Q_OBJECT

public:
	StateGroup(QWidget* parent);
	void addData(StateVariable stateArr[], unsigned int length, QString GroupName, QString col1, QString col2 = "", QString col3 = "", QString col4 = "");

	~StateGroup();
};

