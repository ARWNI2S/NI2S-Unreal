// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "NI2SNetworkSettings.generated.h"

/**
 * Implements the network settings for the NI2S plugin.
 */
UCLASS(config = Engine, defaultconfig)
class NI2SNETWORK_API UNI2SNetworkSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	// Skips network checks to local development node automatically while editing.
	UPROPERTY(EditAnywhere, config, Category = Experimental, meta = (ConfigRestartRequired = true))
	bool bIsStandaloneWorkstation;

	// Enables automatic resizing of various sprite data that is authored in texture space if the source texture gets resized (sockets, the pivot, render and collision geometry, etc...)
	UPROPERTY(EditAnywhere, config, Category = Settings)
	bool bTestOption;
};