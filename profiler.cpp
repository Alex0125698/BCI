#include "resources.h"
#include "profiler.h"

Profiler Profiler::p;

void Profiler::add(std::string&& name)
{
	m_data[name].timer.restart();
}

void Profiler::recordTime(std::string&& name)
{
	auto& tmp = m_data[name];

	tmp.times.emplace_back(tmp.timer.getDuration());
	tmp.timer.restart();
}

void Profiler::resetTimer(std::string&& name)
{
	m_data[name].timer.restart();
}

void Profiler::generateResults()
{/*
	if (!m_data.empty())
	{
		out.open("times.csv");
		for (auto& m : m_data)
		{
			out.addElement(m.first);
			for (auto& v : m.second.times)
			{
				out.addElement(v);
			}
			out.addRow();
		}
		out.close();
	}*/
}