#pragma once

#include "resources.h"
#include "bciinterface.h"

namespace bci{

class EmotivInterface : public bci::Interface
{
public:
	EmotivInterface();
	~EmotivInterface() { disconnect(); }
	void connect();
	void disconnect();
	void update();

private:
	// forward declarations
	typedef void* EmoEngineEventHandle;
	typedef void* EmoStateHandle;
	typedef void* DataHandle;
	EmoEngineEventHandle m_event{ nullptr };
	EmoStateHandle m_state{ nullptr };
	DataHandle m_data_handle{ nullptr };
	unsigned int userID = 0;
};

}
