// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreTypes.h"

class FString;

namespace NI2SEditorHelpers
{
	/**
	Runs NI2S plugin initialization
	*/
	bool InitializeNI2SPlugin(bool& IsInitialized);

	/**
	Resets NI2S plugin
	*/
	bool ResetNI2SPlugin(bool& IsInitialized);
}