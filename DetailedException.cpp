#include "DetailedException.h"

const QString DetailedException::getDetails()
{
    if (m_file == "") return QString();

    return QString("Error thrown in file: %1 on line: %2").arg(m_file.section('\\', -1)).arg(m_line);
}

DebugOutput DebugOutput::m_dbg;