#pragma once

#include "resources.h"
#include "error.h"

namespace bci
{
	class BCI_Packet;

	namespace translation
	{
		BCI_Packet& test(BCI_Packet& data);
	}
}