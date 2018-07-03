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

enum MSG_TYPE
{
    SUCCESS,
    MESSAGE,
    WARNING,
    DEXCEP,
    STDERROR,
    FATAL
};

class DetailedException
{
private:
    const QString m_msg;   // what the error was
    const int m_line;      // the line where the error was thrown
    const QString m_file;  // the file where the error was thrown

public:
    DetailedException(const QString msg, const int line, const QString file);
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
        DebugOutput::getDebug().emitMessage(msg, file.section('\\', -1), line, type);
    }
    static DebugOutput& getDebug() { return m_dbg; }

signals:
    void sigDebugMessage(const QString msg, const QString file, const int line, const int type);

private:
    void emitMessage(const QString& msg, const QString& file, const int& line, const int& type)
    {
        emit sigDebugMessage(msg, file, line, type);
    }
    DebugOutput() = default;
    DebugOutput(const DebugOutput&) = delete;
    DebugOutput& operator= (DebugOutput&) = delete;
    static DebugOutput m_dbg;
};

// use this to automatically print stuff in the debug window
#define DEBUG_PRINTLN(x) DebugOutput::println(x, __FILE__, __LINE__, MSG_TYPE::MESSAGE);
#define DEBUG_PRINTLNY(x,y) DebugOutput::println(x, __FILE__, __LINE__, y);
