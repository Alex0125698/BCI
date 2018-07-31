#pragma once

#include "resources.h"
#include "bciinterface.h"

class CSVreader;

namespace bci {

class Offline : public bci::Interface
{
public:
	Offline(const std::string& fileName);
	Offline(const std::string&& fileName) : Offline( fileName ) {}
	virtual ~Offline() { disconnect(); }
	virtual void start();
	virtual void stop();

private:
	const std::string m_file_name;
	std::unique_ptr<CSVreader> m_in;
};

}