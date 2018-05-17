#pragma once

#include "resources.h"
#include "BCI_Interface.h"

// forward declarations
typedef void* EmoEngineEventHandle;
typedef void* EmoStateHandle;
typedef void* DataHandle;

namespace bci{

class EmotivInterface : public bci::BCI_Interface
{
public:
	EmotivInterface();
	~EmotivInterface();
	void connect();
	virtual BCI_Packet& getData();
	

private:
	EmoEngineEventHandle mEvent{ nullptr };
	EmoStateHandle mState{ nullptr };
	DataHandle mDataHandle{ nullptr };

	unsigned int userID = 0;
};

}
