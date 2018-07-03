#pragma once

#include <cassert>
#include <cstdint>
#include <vector>
#include <QString>
#include "statevariable.h"

struct Input
{
	enum NAMES
	{
		NONE = -1,
		DISP,
		END_OF_DATA
	};
	uint32_t data[END_OF_DATA] {0};
	uint32_t& operator[](const size_t index) {assert(index<END_OF_DATA); return data[index];}
	static Input Vars;
};

struct Output
{
	enum NAMES
	{
		NONE = -1,
		MAX_TEMP,
		END_OF_DATA
	};
	uint32_t data[END_OF_DATA]{0};
	uint32_t& operator[](const size_t index) {assert(index<END_OF_DATA); return data[index];}
	static Output Vars;
};

const Unit m("m",1,0,false);
const Unit mm("mm",1,0,false);
const Unit N("N",1,0,false);
const Unit kN("kN",1,0,false);
const Unit K("K",1,0,false);
const Unit deg("deg",1,0,false);
const Unit mps2("m/s^2",1,0,false);
const Unit s("s",1,0,false);
const Unit mps("m/s",1,0,false);
const Unit kJ("kJ",1,0,false);
const Unit V("V",1,0,false);
const Unit Hz("Hz",1,0,false);

struct Var
{
	enum LOCATION
	{
		VARS,
	};
	LOCATION LOC;
	QString NAME;
	uint32_t SIZE_IN_BYTES;
	Input::NAMES IN_INDEX;
	Output::NAMES OUT_INDEX;
	uint32_t MIN;
	uint32_t MAX;
	bool NAME_EDITABLE;
	std::vector<Unit> UNITS;
	StateView::TYPE VIEW;
};

enum VAR_NAMES
{
};

const Var VAR[] = {
	{Var::VARS, "Disp.",   2, Input::DISP,   Output::NONE, 0, 65536, false, {m,mm},  StateView::TABLE},
};
