#pragma once

#include "csvio.h"
#include "timer.h"
#include <map>
#include <vector>
#include <string>


class Profiler
{
public:
	static auto& get() { return p; }
	void add(std::string&& name);
	void recordTime(std::string&& name);
	void resetTimer(std::string&& name);
	void generateResults();

private:
	struct stuff
	{
		Timer timer;
		std::vector<double> times;
	};

private:
	static Profiler p;
	//CSVwriter out;
	std::map<std::string, stuff> m_data;


};