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
		CH1,
		CH2,
		CH3,
		CH4,
		CH5,
		CH6,
		CH7,
		CH8,
		CH9,
		CH10,
		CH11,
		CH12,
		CH13,
		CH14,
		CH15,
		CH16,
		CH17,
		CH18,
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
		NONE = -1,
		CHANNEL
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
	{ Var::CHANNEL, "CH1", 4, Input::CH1, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH2", 4, Input::CH2, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH3", 4, Input::CH3, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH4", 4, Input::CH4, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH5", 4, Input::CH5, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH6", 4, Input::CH6, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH7", 4, Input::CH7, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH8", 4, Input::CH8, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH9", 4, Input::CH9, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH10", 4, Input::CH10, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH11", 4, Input::CH11, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH12", 4, Input::CH12, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH13", 4, Input::CH13, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH14", 4, Input::CH14, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH15", 4, Input::CH15, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
	{ Var::CHANNEL, "CH16", 4, Input::CH16, Output::NONE, 0, uint64_t(-1), true,{ V }, StateView::TABLE },
};
