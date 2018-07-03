#include "csvio.h"
#include "error.h"
#include <QString>

void CSVreader::open(const char name[])
{
    close();
    m_in.open(name);

    if (!m_in.is_open())
        throw DETAILEDEXCEPTION(QString("file: \"%1\" could not be opened").arg(name));
}

void CSVreader::close()
{
    if (m_in.is_open())	m_in.close();
}

std::string CSVreader::read()
{
    std::string result;
    while(true)
    {
        char c = m_in.get();

        if (c == '\n')
        {
            if (result == "")
                result = "\n";
            else
                m_in.putback(c);
            break;
        }
        else if (c == ',')
        {
            break;
        }
        result += c;
    }
    return result;
}

void CSVwriter::open(const char name[])
{
    close();
    m_outf.open(name);

    if (!m_outf.is_open())
        throw DETAILEDEXCEPTION(QString("file: \"%1\" could not be created").arg(name));

    m_start_file = true;
}

void CSVwriter::close()
{
    if (m_outf.is_open()) m_outf.close();
}

void CSVwriter::flush()
{
    m_outf << std::flush;
}

void CSVwriter::addRow()
{
    m_start_row = true;
}
