/*****************************************************************************************
*    File: DetailedException.h                                                           *
*                                                                                        *
*    DetailedException class - Used for handling errors and recording where the error    *
*    occurred. Should only catch problems which happen infrequently, otherwise it will   *
*    cause large performance issues                                                      *
*                                                                                        *
*    Author: A.S. Woodcock                                                               *
*****************************************************************************************/

#pragma once

#include <QString>
#include <QObject>

class DetailedException
{
private:
	const QString m_msg;   // what the error was
	const int m_line;      // the line where the error was thrown
	const QString m_file;  // the file where the error was thrown

public:
	DetailedException(const QString msg, const int line, const QString file)
		: m_msg{msg}, m_line{line}, m_file{file}
	{}

	const int getLine() {return m_line;}
	const QString& getFile() {return m_file;}
	const QString& getMessage(){return m_msg;}
	const QString getDetails();
};

// use this to automatically add line + file details
#define DETAILEDEXCEPTION(x) DetailedException(x, __LINE__, __FILE__)


class DebugOutput : public QObject
{
	Q_OBJECT

public:
	static void println(const QString& msg, const QString& file, const int& line, const int& type)
	{
		DebugOutput::getDebug().emitMessage(msg, file, line, type);
	}
	static DebugOutput& getDebug() { return m_dbg; }

signals:
	void output(const QString msg, const QString file, const int line, const int type);

private:
	void emitMessage(const QString& msg, const QString& file, const int& line, const int& type)
	{
		emit output(msg, file, line, type);
	}
	DebugOutput() = default;
	DebugOutput(const DebugOutput&) = delete;
	DebugOutput& operator= (DebugOutput&) = delete;
	static DebugOutput m_dbg;
};
