#pragma once
class Uncopyable
{
public:
	Uncopyable() = default;
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator= (Uncopyable&) = delete;
};

