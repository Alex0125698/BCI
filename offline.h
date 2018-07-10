#pragma once

#include <string>
#include <memory>
#include "bciinterface.h"

class CSVreader;

namespace bci {

class Offline : public bci::Interface
{
public:
	Offline(const std::string& fileName);
	Offline(const std::string&& fileName) : Offline( fileName ) {}
	~Offline() { disconnect(); }
	void connect();
	void disconnect();
	void update();

private:
	const std::string m_file_name;
	std::unique_ptr<CSVreader> m_in;
};

}