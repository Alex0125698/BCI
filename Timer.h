/* ****************************************************************


**************************************************************** */

#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point m_time;

public:
	Timer() { restart(); }
	inline void restart();
	inline double getDuration();
};

void Timer::restart()
{
	m_time = std::chrono::high_resolution_clock::now();
}

double Timer::getDuration()
{
	auto tmp = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double>(tmp - m_time).count();
}