#pragma once

#include "bciinterface.h"

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
	void disconnect();
	virtual BCI_Packet& getData();
	

private:
	EmoEngineEventHandle mEvent{ nullptr };
	EmoStateHandle mState{ nullptr };
	DataHandle mDataHandle{ nullptr };

	unsigned int userID = 0;
};

}
