// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "NI2SEditorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogNI2SPlugin, Log, All);

namespace NI2SEditorHelpers
{
	bool InitializeNI2SPlugin(bool& IsInitialized)
	{
		bool bResult = false;

		// TODO: SYSTEM INITIALIZATION
		bResult = true;

		IsInitialized = bResult;
		return bResult;
	}

	bool ResetNI2SPlugin(bool& IsInitialized)
	{
		bool bResult = IsInitialized;

		// TODO: SYSTEM RESET
		bResult = false;

		IsInitialized = bResult;
		return bResult;
	}
}
