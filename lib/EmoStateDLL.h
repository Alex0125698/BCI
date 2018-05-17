/**
 * EmoState Interface
 * Copyright (c) 2009 Emotiv Systems, Inc.
 * 
 * Header file to define constants and interfaces to access the EmoState.
 *
 * EmoStates are generated by the Emotiv detection engine (EmoEngine) and represent
 * the emotional status of the user at a given time.
 *
 * EmoStateHandle is an opaque reference to an internal EmoState structure
 *
 * None of the EmoState interface functions are thread-safe.
 *
 * This header file is designed to be includable under C and C++ environment.
 *
 */


#ifndef EMOSTATE_DLL_H
#define EMOSTATE_DLL_H
#include <sys/types.h>

#if !defined(EDK_STATIC_LIB) && !defined(EDK_UTILS_ONLY)

#ifdef _WIN32
#ifdef EMOSTATE_DLL_EXPORTS
#define EMOSTATE_DLL_API __declspec(dllexport)
#else
#define EMOSTATE_DLL_API __declspec(dllimport)
#endif
#else
#define EMOSTATE_DLL_API
#endif

#else
#define EMOSTATE_DLL_API extern
#endif

/**
 * Defining EmoStateHandle as a void pointer
 */
typedef void* EmoStateHandle;

extern "C"
{
	/**
	 * Emotiv Detection Suite enumerator
	 */
	typedef enum EE_EmotivSuite_enum {

		EE_EXPRESSIV = 0, EE_AFFECTIV, EE_COGNITIV

	} EE_EmotivSuite_t;

	/**
	 * Expressiv facial expression type enumerator
	 */
	typedef enum EE_ExpressivAlgo_enum {

		EXP_NEUTRAL			= 0x0001,
		EXP_BLINK			= 0x0002,
		EXP_WINK_LEFT		= 0x0004,
		EXP_WINK_RIGHT		= 0x0008,
        EXP_HORIEYE			= 0x0010,
		EXP_EYEBROW			= 0x0020,
		EXP_FURROW			= 0x0040,
        EXP_SMILE			= 0x0080,
		EXP_CLENCH			= 0x0100,
		EXP_LAUGH			= 0x0200,
		EXP_SMIRK_LEFT		= 0x0400,
		EXP_SMIRK_RIGHT		= 0x0800

	} EE_ExpressivAlgo_t;
	
	/**
	 * Affectiv emotional type enumerator
	 */
	typedef enum EE_AffectivAlgo_enum {

		AFF_EXCITEMENT			= 0x0001,
		AFF_MEDITATION			= 0x0002,
		AFF_FRUSTRATION			= 0x0004,
		AFF_ENGAGEMENT_BOREDOM	= 0x0008

	} EE_AffectivAlgo_t;

	/**
	 * Cognitiv action type enumerator
	 */
	typedef enum EE_CognitivAction_enum {

		COG_NEUTRAL						= 0x0001,
		COG_PUSH						= 0x0002,
		COG_PULL						= 0x0004,
		COG_LIFT						= 0x0008,
		COG_DROP						= 0x0010,
		COG_LEFT						= 0x0020,
		COG_RIGHT						= 0x0040,
		COG_ROTATE_LEFT					= 0x0080,
		COG_ROTATE_RIGHT				= 0x0100,
		COG_ROTATE_CLOCKWISE			= 0x0200,
		COG_ROTATE_COUNTER_CLOCKWISE	= 0x0400,
		COG_ROTATE_FORWARDS				= 0x0800,
		COG_ROTATE_REVERSE				= 0x1000,
		COG_DISAPPEAR					= 0x2000

	} EE_CognitivAction_t;
	
	/**
	 * Wireless Signal Strength enumerator
	 */
	typedef enum EE_SignalStrength_enum {

		NO_SIGNAL = 0, BAD_SIGNAL, GOOD_SIGNAL
	
	} EE_SignalStrength_t;

	//! Logical input channel identifiers
	/*! Note: the number of channels may not necessarily match the number of
	    electrodes on your headset.  Signal quality and input data for some
		sensors will be identical: CMS = DRL, FP1 = AF3, F2 = AF4.
	*/
	typedef enum EE_InputChannels_enum {
		EE_CHAN_CMS = 0, EE_CHAN_DRL, EE_CHAN_FP1, EE_CHAN_AF3, EE_CHAN_F7, 
		EE_CHAN_F3, EE_CHAN_FC5, EE_CHAN_T7, EE_CHAN_P7, EE_CHAN_O1,
		EE_CHAN_O2, EE_CHAN_P8, EE_CHAN_T8, EE_CHAN_FC6, EE_CHAN_F4,
		EE_CHAN_F8, EE_CHAN_AF4, EE_CHAN_FP2
	} EE_InputChannels_t;

    //! EEG Electrode Contact Quality enumeration
    /*! Used to characterize the EEG signal reception or electrode contact
        for a sensor on the headset.  Note that this differs from the wireless
        signal strength, which refers to the radio communication between the 
        headset transmitter and USB dongle receiver.
     */
    typedef enum EE_EEG_ContactQuality_enum {
        EEG_CQ_NO_SIGNAL, EEG_CQ_VERY_BAD, EEG_CQ_POOR, 
        EEG_CQ_FAIR, EEG_CQ_GOOD } EE_EEG_ContactQuality_t;

	//! Create EmoState handle.
	/*!
        NOTE: THIS FUNCTION HAS BEEN DEPRECATED - please use EE_EmoStateCreate instead.

		ES_Init is called automatically after the creation of the EmoState handle.
		ES_Free must be called to free up resources during the creation of the EmoState handle.

		\return the EmoStateHandle if succeeded

		\sa EE_EmoStateCreate, ES_Free, ES_Init
	*/
	EMOSTATE_DLL_API EmoStateHandle	ES_Create();

	//! Free EmoState handle
	/*!
        NOTE: THIS FUNCTION HAS BEEN DEPRECATED - please use EE_EmoStateFree instead.

		\param state - EmoStateHandle that was created by ES_Create function call

		\sa EE_EmoStateFree, ES_Create
	*/
	EMOSTATE_DLL_API void ES_Free(EmoStateHandle state);

	//! Initialize the EmoState into neutral state
	/*!
		\param state - EmoStateHandle
		
		\sa ES_Create, ES_Free
	*/
	EMOSTATE_DLL_API void ES_Init(EmoStateHandle state);

	//! Return the time since EmoEngine has been successfully connected to the headset
	/*!
		If the headset is disconnected from EmoEngine due to low battery or weak
		wireless signal, the time will be reset to zero.

		\param state - EmoStateHandle

		\return float - time in second

	*/
	EMOSTATE_DLL_API float ES_GetTimeFromStart(EmoStateHandle state);

	//! Return whether the headset has been put on correctly or not
	/*!
		If the headset cannot not be detected on the head, then signal quality will not report
		any results for all the channels

		\param state - EmoStatehandle

		\return int - (1: On, 0: Off)
	*/
	EMOSTATE_DLL_API int	ES_GetHeadsetOn(EmoStateHandle state);

	//! Query the number of channels of available sensor contact quality data
	/*!
		\param state - EmoStateHandle
		\return number of channels for which contact quality data is available (int)

		\sa ES_GetNumContactQualityChannels
	*/
	EMOSTATE_DLL_API int ES_GetNumContactQualityChannels(EmoStateHandle state);

	//! Query the contact quality of a specific EEG electrode
	/*!
		\param state - EmoStateHandle
		\param electroIdx - The index of the electrode for query

		\return EE_EEG_ContactQuality_t - Enumerated value that characterizes the EEG electrode contact for the specified input channel

		\sa ES_GetContactQuality
	*/
	EMOSTATE_DLL_API EE_EEG_ContactQuality_t ES_GetContactQuality(EmoStateHandle state, int electroIdx);

	//! Query the contact quality of all the electrodes in one single call
	/*!
		The contact quality will be stored in the array, contactQuality, passed to the function.
		The value stored in contactQuality[0] is identical to the result returned by
		ES_GetContactQuality(state, 0)
		The value stored in contactQuality[1] is identical to the result returned by
		ES_GetContactQuality(state, 1). etc.
		The ordering of the array is consistent with the ordering of the logical input
		channels in EE_InputChannels_enum.

		\param state - EmoStateHandle
		\param contactQuality - array of 32-bit float of size numChannels
		\param numChannels - size (number of floats) of the input array

		\return Number of signal quality values copied to the contactQuality array.

		\sa ES_GetContactQualityFromAllChannels
	*/
	EMOSTATE_DLL_API int ES_GetContactQualityFromAllChannels(EmoStateHandle state, EE_EEG_ContactQuality_t* contactQuality, size_t numChannels);

	//! Query whether the user is blinking at the time the EmoState is captured.
	/*!
		\param state - EmoStateHandle

		\return blink status (1: blink, 0: not blink)

	*/
	EMOSTATE_DLL_API int ES_ExpressivIsBlink(EmoStateHandle state);

	//! Query whether the user is winking left at the time the EmoState is captured.
	/*!
		\param state - EmoStateHandle

		\return left wink status (1: wink, 0: not wink)

		\sa ES_ExpressivIsRightWink
	*/
	EMOSTATE_DLL_API int ES_ExpressivIsLeftWink(EmoStateHandle state);

	//! Query whether the user is winking right at the time the EmoState is captured.
	/*!
		\param state - EmoStateHandle

		\return right wink status (1: wink, 0: not wink)

		\sa ES_ExpressivIsLeftWink
	*/
	EMOSTATE_DLL_API int ES_ExpressivIsRightWink(EmoStateHandle state);

	//! Query whether the eyes of the user are opened at the time the EmoState is captured.
	/*!
		\param state - EmoStateHandle

		\return eye open status (1: eyes open, 0: eyes closed)

	*/
	EMOSTATE_DLL_API int ES_ExpressivIsEyesOpen(EmoStateHandle state);

	//! Query whether the user is looking up at the time the EmoState is captured.
	/*!
		\param state - EmoStateHandle

		\return eyes position (1: looking up, 0: not looking up)

		\sa ES_ExpressivIsLookingDown
	*/
	EMOSTATE_DLL_API int ES_ExpressivIsLookingUp(EmoStateHandle state);

	//! Query whether the user is looking down at the time the EmoState is captured.
	/*!
		\param state - EmoStateHandle

		\return eyes position (1: looking down, 0: not looking down)

		\sa ES_ExpressivIsLookingUp
	*/
	EMOSTATE_DLL_API int ES_ExpressivIsLookingDown(EmoStateHandle state);

	//! Query whether the user is looking left at the time the EmoState is captured.
	/*!
		\param state - EmoStatehandle

		\return eye position (1: looking left, 0: not looking left)

		\sa ES_ExpressivIsLookingRight
	*/
	EMOSTATE_DLL_API int ES_ExpressivIsLookingLeft(EmoStateHandle state);

	//! Query whether the user is looking right at the time the EmoState is captured.
	/*!
		\param state - EmoStatehandle

		\return eye position (1: looking right, 0: not looking right)

		\sa ES_ExpressivIsLookingLeft
	*/
	EMOSTATE_DLL_API int ES_ExpressivIsLookingRight(EmoStateHandle state);

	//! Query the eyelids state of the user
	/*!
		The left and right eyelid state are stored in the parameter leftEye and rightEye
		respectively. They are floating point values ranging from 0.0 to 1.0.
		0.0 indicates that the eyelid is fully opened while 1.0 indicates that the
		eyelid is fully closed.

		\param state - EmoStatehandle
		\param leftEye - the left eyelid state (0.0 to 1.0)
		\param rightEye - the right eyelid state (0.0 to 1.0)

	*/
	EMOSTATE_DLL_API void ES_ExpressivGetEyelidState(EmoStateHandle state, float* leftEye, float* rightEye);

	//! Query the eyes position of the user
	/*!
		The horizontal and vertical position of the eyes are stored in the parameter x and y
		respectively. They are floating point values ranging from -1.0 to 1.0.
		
		For horizontal position, -1.0 indicates that the user is looking left while
		1.0 indicates that the user is looking right.
		
		For vertical position, -1.0 indicates that the user is looking down while
		1.0 indicatest that the user is looking up.

		This function assumes that both eyes have the same horizontal or vertical positions.
		(i.e. no cross eyes)

		\param state - EmoStateHandle
		\param x - the horizontal position of the eyes
		\param y - the veritcal position of the eyes

	*/
	EMOSTATE_DLL_API void ES_ExpressivGetEyeLocation(EmoStateHandle state, float* x, float* y);

	//! Returns the eyebrow extent of the user (Obsolete function)
	/*!
		\param state - EmoStateHandle
		
		\return eyebrow extent value (0.0 to 1.0)

		\sa ES_ExpressivGetUpperFaceAction, ES_ExpressivGetUpperFaceActionPower
	*/
	EMOSTATE_DLL_API float ES_ExpressivGetEyebrowExtent(EmoStateHandle state);

	//! Returns the smile extent of the user (Obsolete function)
	/*!
		\param state - EmoStatehandle
		
		\return smile extent value (0.0 to 1.0)

		\sa ES_ExpressivGetLowerFaceAction, ES_ExpressivGetLowerFaceActionPower
	*/
	EMOSTATE_DLL_API float ES_ExpressivGetSmileExtent(EmoStateHandle state);

	//! Returns the clench extent of the user (Obsolete function)
	/*!
		\param state - EmoStatehandle

		\return clench extent value (0.0 to 1.0)

		\sa ES_ExpressivGetLowerFaceAction, ES_ExpressivGetLowerFaceActionPower
	*/
	EMOSTATE_DLL_API float ES_ExpressivGetClenchExtent(EmoStateHandle state);


	//! Returns the detected upper face Expressiv action of the user
	/*!
		\param state - EmoStatehandle

		\return pre-defined Expressiv action types

		\sa ES_ExpressivGetUpperFaceActionPower
	*/
	EMOSTATE_DLL_API EE_ExpressivAlgo_t ES_ExpressivGetUpperFaceAction(EmoStateHandle state);

	//! Returns the detected upper face Expressiv action power of the user
	/*!
		\param state - EmoStatehandle

		\return power value (0.0 to 1.0)

		\sa ES_ExpressivGetUpperFaceAction
	*/
	EMOSTATE_DLL_API float ES_ExpressivGetUpperFaceActionPower(EmoStateHandle state);

	//! Returns the detected lower face Expressiv action of the user
	/*!
		\param state - EmoStatehandle

		\return pre-defined Expressiv action types

		\sa ES_ExpressivGetLowerFaceActionPower
	*/
	EMOSTATE_DLL_API EE_ExpressivAlgo_t ES_ExpressivGetLowerFaceAction(EmoStateHandle state);

	//! Returns the detected lower face Expressiv action power of the user
	/*!
		\param state - EmoStatehandle

		\return power value (0.0 to 1.0)

		\sa ES_ExpressivGetLowerFaceAction
	*/
	EMOSTATE_DLL_API float ES_ExpressivGetLowerFaceActionPower(EmoStateHandle state);
	
	//! Query whether the signal is too noisy for Expressiv detection to be active
	/*!
		\param state - EmoStateHandle
		\param type  - Expressiv detection type

		\return detection state (0: Not Active, 1: Active)

		\sa EE_ExpressivAlgo_t
	*/
	EMOSTATE_DLL_API int ES_ExpressivIsActive(EmoStateHandle state, EE_ExpressivAlgo_t type);

	//! Returns the long term excitement level of the user
	/*!
		\param state - EmoStateHandle

		\return excitement level (0.0 to 1.0)

		\sa ES_AffectivGetExcitementShortTermScore
	*/
	EMOSTATE_DLL_API float ES_AffectivGetExcitementLongTermScore(EmoStateHandle state);

	//! Returns short term excitement level of the user
	/*!
		\param state - EmoStateHandle

		\return excitement level (0.0 to 1.0)

		\sa ES_AffectivGetExcitementLongTermScore
	*/
	EMOSTATE_DLL_API float ES_AffectivGetExcitementShortTermScore(EmoStateHandle state);
	
	//! Query whether the signal is too noisy for Affectiv detection to be active
	/*!
		\param state - EmoStateHandle
		\param type  - Affectiv detection type

		\return detection state (0: Not Active, 1: Active)

		\sa EE_AffectivAlgo_t
	*/
	EMOSTATE_DLL_API int ES_AffectivIsActive(EmoStateHandle state, EE_AffectivAlgo_t type);

	//! Returns meditation level of the user
	/*!
		\param state - EmoStateHandle

		\return meditation level (0.0 to 1.0)
	*/
	EMOSTATE_DLL_API float ES_AffectivGetMeditationScore(EmoStateHandle state);

	//! Returns frustration level of the user
	/*!
		\param state - EmoStateHandle

		\return frustration level (0.0 to 1.0)
	*/
	EMOSTATE_DLL_API float ES_AffectivGetFrustrationScore(EmoStateHandle state);

	//! Returns engagement/boredom level of the user
	/*!
		\param state - EmoStateHandle

		\return engagement/boredom level (0.0 to 1.0)
	*/
	EMOSTATE_DLL_API float ES_AffectivGetEngagementBoredomScore(EmoStateHandle state);

	//! Returns the detected Cognitiv action of the user
	/*!
		\param state - EmoStateHandle

		\return Cognitiv action type

		\sa EE_CognitivAction_t, ES_CognitivGetCurrentActionPower
	*/
	EMOSTATE_DLL_API EE_CognitivAction_t ES_CognitivGetCurrentAction(EmoStateHandle state);

	//! Returns the detected Cognitiv action power of the user
	/*!
		\param state - EmoStateHandle

		\return Cognitiv action power (0.0 to 1.0)

		\sa ES_CognitivGetCurrentAction
	*/
	EMOSTATE_DLL_API float ES_CognitivGetCurrentActionPower(EmoStateHandle state);
	
	//! Query whether the signal is too noisy for Cognitiv detection to be active
	/*!
		\param state - EmoStateHandle

		\return detection state (0: Not Active, 1: Active)
	*/
	EMOSTATE_DLL_API int ES_CognitivIsActive(EmoStateHandle state);


	//! Query of the current wireless signal strength
	/*!
		\param state - EmoStateHandle

		\return wireless signal strength [No Signal, Bad, Fair, Good, Excellent].

		\sa EE_SignalStrength_t
	*/
	EMOSTATE_DLL_API EE_SignalStrength_t ES_GetWirelessSignalStatus(EmoStateHandle state);

	//! Clone EmoStateHandle
	/*!
		\param a - Destination of EmoStateHandle
		\param b - Source of EmoStateHandle

		\sa ES_Create
	*/
	EMOSTATE_DLL_API void ES_Copy(EmoStateHandle a, EmoStateHandle b);

	//! Check whether two states are with identical 'emotiv' state
	/*!
		\param a - EmoStateHandle
		\param b - EmoStateHandle

		\return 1: Equal, 0: Different

		\sa ES_ExpressivEqual, ES_CognitivEqual, ES_EmoEngineEqual, ES_Equal
	*/
	EMOSTATE_DLL_API int ES_AffectivEqual(EmoStateHandle a, EmoStateHandle b);

	//! Check whether two states are with identical Expressiv state, i.e. are both state representing the same facial expression
	/*!
		\param a - EmoStateHandle
		\param b - EmoStateHandle

		\return 1: Equal, 0: Different

		\sa ES_AffectivEqual, ES_CognitivEqual, ES_EmoEngineEqual, ES_Equal
	*/
	EMOSTATE_DLL_API int ES_ExpressivEqual(EmoStateHandle a, EmoStateHandle b);

	//! Check whether two states are with identical Cognitiv state
	/*!
		\param a - EmoStateHandle
		\param b - EmoStateHandle

		\return 1: Equal, 0: Different

		\sa ES_AffectivEqual, ES_ExpressivEqual, ES_EmoEngineEqual, ES_Equal
	*/
	EMOSTATE_DLL_API int ES_CognitivEqual(EmoStateHandle a, EmoStateHandle b);

	//! Check whether two states are with identical EmoEngine state.
	/*!
		This function is comparing the time since EmoEngine start,
		the wireless signal strength and the signal quality of different channels

		\param a - EmoStateHandle
		\param b - EmoStateHandle

		\return 1: Equal, 0: Different

		\sa ES_AffectivEqual, ES_ExpressivEqual, ES_CognitivEqual, ES_Equal
	*/
	EMOSTATE_DLL_API int ES_EmoEngineEqual(EmoStateHandle a, EmoStateHandle b);

	//! Check whether two EmoStateHandles are identical
	/*!
		\param a - EmoStateHandle
		\param b - EmoStateHandle

		\return 1: Equal, 0: Different

		\sa ES_AffectivEqual, ES_ExpressivEqual, ES_EmoEngineEqual
	*/
	EMOSTATE_DLL_API int ES_Equal(EmoStateHandle a, EmoStateHandle b);

	//! Get the level of charge remaining in the headset battery
	/*!
		\param state			- EmoStateHandle
		\param chargeLevel		- the current level of charge in the headset battery
		\param maxChargeLevel	- the maximum level of charge in the battery

	*/
	EMOSTATE_DLL_API void ES_GetBatteryChargeLevel(EmoStateHandle state, int* chargeLevel, int* maxChargeLevel);
};

#endif // EMOSTATE_DLL_H

