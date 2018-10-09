#include "error.h"

DetailedException::DetailedException(const QString msg, const int line, const QString file)
    : m_msg{msg}, m_line{line}, m_file{file.section('\\', -1)}
{
}

const QString DetailedException::getDetails()
{
    if (m_file == "") return QString();

    return QString("Error thrown in file: %1 on line: %2").arg(m_file).arg(m_line);
}

DebugOutput DebugOutput::m_dbg;
