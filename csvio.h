/* ****************************************************************

This class writes spreadsheets using the CSV format
(see http://en.wikipedia.org/wiki/Comma-separated_values)


Use 'addElement' to put an element into the next column. Use 'addRow'
to move to the next row. Formatting is not avaliable.

TODO:
> template addElement
> convert to QFile
> check is directory exists before saving; append number if necessary

**************************************************************** */

#pragma once

#include "resources.h"
#include "error.h"
#include <fstream>

class CSVreader
{
public:
	void open(const std::string& name);
	void open(const std::string&& name) { open(name); }
	void close();
	bool isOpen(){return m_in.is_open();}
	// read next element/newline
	std::string read();

private:
	std::ifstream m_in;
};

class CSVwriter
{
public:
	void open(const std::string& name);
	void open(const std::string&& name) { open(name); }
	void close();
	bool isOpen(){return m_outf.is_open();}
	void flush();
	template<typename T>
	void addElement(T data);
	void addRow();

private:
	std::ofstream m_outf;
	bool m_start_file{true};
	bool m_start_row{false};
};

template<typename T>
void CSVwriter::addElement(T data)
{
	if (m_start_file)
	{
		m_outf << data;
		m_start_file = false;
	}
	else if (m_start_row)
	{
		m_outf << '\n' << data;
		m_start_row = false;
	}
	else
		m_outf << ',' << data;
}
