// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "NI2SRuntimeSettings.generated.h"

/**
 * Implements the settings for the Paper2D plugin.
 */
UCLASS(config = NI2SUE5, defaultconfig)
class NI2SFRAMEWORK_API UNI2SRuntimeSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	// Enables experimental *incomplete and unsupported* texture atlas groups that sprites can be assigned to
	UPROPERTY(EditAnywhere, config, Category = Experimental)
	bool bTestOption;

	// Enables experimental *incomplete and unsupported* 2D terrain spline editing. Note: You need to restart the editor when enabling this setting for the change to fully take effect.
	UPROPERTY(EditAnywhere, config, Category = Experimental, meta = (ConfigRestartRequired = true))
	bool bTestOptionRestart;
};