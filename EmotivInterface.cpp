#include "EmotivInterface.h"

#include "lib/edk.h" // Library


const EE_InputChannels_t electrodes[] =
{
	EE_CHAN_CMS, EE_CHAN_DRL, EE_CHAN_FP1, EE_CHAN_AF3, EE_CHAN_F7,
	EE_CHAN_F3, EE_CHAN_FC5, EE_CHAN_T7, EE_CHAN_P7, EE_CHAN_O1,
	EE_CHAN_O2, EE_CHAN_P8, EE_CHAN_T8, EE_CHAN_FC6, EE_CHAN_F4,
	EE_CHAN_F8, EE_CHAN_AF4, EE_CHAN_FP2
};

const std::string electrodeNames[] =
{
	"CMS", "DRL", "FP1", "AF3", "F7", "F3", "FC5", "T7", "P7", "O1",
	"O2", "P8", "T8", "FC6", "F4", "F8", "AF4", "FP2"
};

bci::EmotivInterface::EmotivInterface()
{
	// Allocate memory for the Emotiv API
	mEvent = EE_EmoEngineEventCreate();
	mState = EE_EmoStateCreate();

	// Connect to the Emotiv Engine
	if (EE_EngineConnect() != EDK_OK)
		throw DETAILEDEXCEPTION("Could not start Emotiv Engine");

	// Attempt to find the device
	mDataHandle = EE_DataCreate();
	if (mDataHandle == 0)
		throw DETAILEDEXCEPTION("Could not get EE data handle");

	std::cout << "Successfully started Emotiv Engine !!!" << std::endl;
}

bci::EmotivInterface::~EmotivInterface()
{
	// Deallocate device data memory
	if (mDataHandle)
	{
		EE_DataFree(mDataHandle);
		mDataHandle = nullptr;
	}

	// Disconnect from the Emotiv Engine
	EE_EngineDisconnect();

	// Deallocate the memory for the Emotiv Engine API
	if (mState)
	{
		EE_EmoStateFree(mState);
		mState = nullptr;
	}
	if (mEvent)
	{
		EE_EmoEngineEventFree(mEvent);
		mEvent = nullptr;
	}
}

void bci::EmotivInterface::connect()
{
	// Create a one second buffer
	if (EE_DataSetBufferSizeInSec(1) != EDK_OK)
		throw DETAILEDEXCEPTION("Could not set Emotiv Engine buffer size");

	// wait for a user added event
	Timer timeout;
	int loop_count = 0;
	bool connect_success = false;
	bool headset_on = false;

	while (timeout.getDuration() < 2.0)
	{
		++loop_count;
		if (EE_EngineGetNextEvent(mEvent) == EDK_OK)
		{
			std::cout << "Successfuly retrieved an event from an Emotiv headset !!!" << std::endl;
			std::cout << "Time to connect = " + std::to_string(timeout.getDuration()) + " sec ; number of attempts = " + std::to_string(loop_count) << std::endl;

			auto eventType = EE_EmoEngineEventGetType(mEvent);
			EE_EmoEngineEventGetUserId(mEvent, &userID);

			if (userID != 0)
				throw DETAILEDEXCEPTION("More than one user connected. The current software only supports 1 user");

			// eventTypes are flags - they are not mutually exclusive

			switch (eventType)
			{
			case EE_UnknownEvent:
				throw DETAILEDEXCEPTION("Unknown event detected");

			case EE_EmulatorError:
				throw DETAILEDEXCEPTION("Emulator error. Please restart application");

			case EE_ReservedEvent:

			case EE_UserAdded:
				EE_DataAcquisitionEnable(userID, true);
				std::cout << "Emotiv User Connected.  Waiting for Headset on..." << std::endl;
				
			case EE_UserRemoved:
				EE_DataAcquisitionEnable(userID, false);
				throw DETAILEDEXCEPTION("Emotiv User disconnected. Please reconnect and restart data aquisition");

			case EE_EmoStateUpdated:
				EE_EmoEngineEventGetEmoState(mEvent, mState);

				// If we have a headset on, we're ready to go
				if (ES_GetHeadsetOn(mState))
				{
					std::cout << "Headset on. Starting data aquisition" << std::endl;
					headset_on = true;
					goto exit_loop;
				}

			//case EE_ProfileEvent:

			//case EE_CognitivEvent:

			//case EE_ExpressivEvent:

			//case EE_InternalStateChanged:
	
			}
			connect_success = true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
exit_loop:

	if (!connect_success)
		throw DETAILEDEXCEPTION("No events from an Emotiv headset were detected\nTimout period = 2.0 sec ; number of attempts = " + QString(loop_count));

	if (!headset_on)
		throw DETAILEDEXCEPTION("Events detected but headset not on (waited 2s). Data aquision cannot be started");

	// get Sampling Rate
	// TODO: record this
	unsigned int samplingRate = 0;
	if (EE_DataGetSamplingRate(userID, &samplingRate) == EDK_OK)
		std::cout << "Sampling Rate is = " << samplingRate << std::endl;
	else
		throw DETAILEDEXCEPTION("Could not retreive Emotiv sampling rate ");
}

bci::BCI_Packet& bci::EmotivInterface::getData()
{
	Timer timeout;
	while (true)
	{
		// check that we can still collect data
		if (EE_EngineGetNextEvent(mEvent) == EDK_OK)
		{
			EE_Event_t eventType = EE_EmoEngineEventGetType(mEvent);

			if (eventType == EE_UserRemoved)
			{
				EE_DataAcquisitionEnable(userID, false);
				throw DETAILEDEXCEPTION("Emotiv User disconnected. Please reconnect and restart data aquisition");
			}

			// User is connected, but state has been updated
			if (eventType == EE_EmoStateUpdated)
			{
				// check if headset is still on
				EE_EmoEngineEventGetEmoState(mEvent, mState);
				if (!ES_GetHeadsetOn(mState))
					throw DETAILEDEXCEPTION("Headset turned off. Please reconnect and restart data aquisition");

				// TODO: check signal quality
				//int numKnownChannels = sizeof(electrodeNames) / sizeof(*electrodeNames);
				//int	numQueriedCh = min(numKnownChannels, ES_GetNumContactQualityChannels(mState));
				// these are just integers (values 1 to 4 - see enum)
				//EE_EEG_ContactQuality_t sigQualities[<num electrode channels>];
				//int size = ES_GetContactQualityFromAllChannels(mState, &sigQualities, numQueriedCh);
				//ES_GetContactQualityFromAllChannels(EmoStateHandle state, EE_EEG_ContactQuality_t* contactQuality, size_t numChannels);

				// TODO: check battery level
				//ES_GetBatteryChargeLevel(mState, &charge, &maxCharge);

				// TODO: other stuff to check ???
			}
		}

		// this is where data aquisition occurs - it is independent of the event stuff

		// Update the system
		if (EE_DataUpdateHandle(0, mDataHandle) != EDK_OK)
			throw DETAILEDEXCEPTION("Data handle could not be updated");

		// Find out how many samples have accurred
		unsigned int num_samples = 0;
		EE_DataGetNumberOfSample(mDataHandle, &num_samples);

		if (num_samples > 0)
		{
			for (int i = 0; i < BCI_Channels::END_OF_DATA; ++i)
			{
				double data;
				if (EE_DataGet(mDataHandle, EE_DataChannel_t(EE_DataChannels_enum::ED_AF3 + i), &data, 1) != EDK_OK)
					throw DETAILEDEXCEPTION("Could not get data. Could be caused by overbuffering");

				m_data[i] = data;
			}

			// TODO: get gyro data
			/*
			double gyrox, gyroy;
			if (EE_DataGet(mDataHandle, ED_GYROX, data, nSamplesTaken) != EDK_OK)
			bcierr << "Emotiv Error collecting GyroX Data.  Could not be calibrated..." << endl;
			gyrox = (float)data[nSamplesTaken - 1];

			if (EE_DataGet(mDataHandle, ED_GYROY, data, nSamplesTaken) != EDK_OK)
			bcierr << "Emotiv Error collecting GyroY Data.  Could not be calibrated..." << endl;
			gyroy = (float)data[nSamplesTaken - 1];

			// Get CQ States
			if (EE_DataGet(mDataHandle, ED_RAW_CQ, data, nSamplesTaken) != EDK_OK)
			bcierr << "Emotiv Error getting Contact Quality" << endl;
			*/

			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			break;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(0));
		}

		if (timeout.getDuration() > 3.0)
			throw DETAILEDEXCEPTION("Data aquisition timeout");
	}

	return m_data;
}

