// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"

/**
 * Integrate NI2S actions associated with existing engine types into the content browser
 */
class FNI2SContentBrowserExtensions
{
public:
	static void InstallHooks();
	static void RemoveHooks();
};
