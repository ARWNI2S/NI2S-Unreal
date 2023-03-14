// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "NI2SEditorSettings.generated.h"

/**
 * Implements the editor-only settings for the NI2S plugin.
 */
UCLASS(config = Editor)
class NI2SEDITOR_API UNI2SEditorSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	// Enables full automation for asset processing while editing.
	// TODO Category = ...
	UPROPERTY(EditAnywhere, config, Category = Developer, meta = (ConfigRestartRequired = true))
	bool bEnableFullAutomation;

	// Enables automatic resizing of various sprite data that is authored in texture space if the source texture gets resized (sockets, the pivot, render and collision geometry, etc...)
	UPROPERTY(EditAnywhere, config, Category = Settings)
	bool bTestOption;
};

/**
 * Implements global development settings for the NI2S plugin.
 */
UCLASS(config = Engine)
class NI2SEDITOR_API UNI2SPluginSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
	//~ End UObject Interface

	// Skips network checks to local development node automatically while editing.
	// TODO Category = ...
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, config, Category = Runtime, meta = (ConfigRestartRequired = true))
	bool bIsNI2SInitialized;

	// It's a test option.
	UPROPERTY(EditAnywhere, config, Category = Settings)
	bool bTestOption;
};