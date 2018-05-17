/******************************************************************************************
*																						  *
*	Authour: A.S. Woodcock																  *
******************************************************************************************/
#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <exception>
#include <climits>
#include <chrono>
#include <cassert>
#include <thread>
#include <mutex>

#include "Uncopyable.h"
#include "DetailedException.h"
#include "Timer.h"


enum MSG_TYPE
{
    FATAL,
    STD,
    NON_FATAL,
    WARNING,
    MESSAGE,
    GOOD_NEWS
};
