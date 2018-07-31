#include "csvio.h"

void CSVreader::open(const std::string& name)
{
	close();
	m_in.open(name);

	if (!m_in.is_open())
		throw DETAILEDEXCEPTION(QString("file: \"%1\" could not be opened").arg(QString::fromStdString(name)));
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
		if (!m_in)
		{
			if (result.size() > 0) break;

			if (m_in.eof())
				return "";
			else if (m_in.bad())
				throw DETAILEDEXCEPTION("File Read Failed: Bad bit set");
			else
				throw DETAILEDEXCEPTION("File Read Failed: Fail bit set");
		}

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

void CSVwriter::open(const std::string& name)
{
	close();
	m_outf.open(name);

	if (!m_outf.is_open())
		throw DETAILEDEXCEPTION(QString("file: \"%1\" could not be created").arg(QString::fromStdString(name)));

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
