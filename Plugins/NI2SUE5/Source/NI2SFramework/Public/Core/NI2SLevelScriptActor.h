// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "NI2SLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class NI2SFRAMEWORK_API ANI2SLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	ANI2SLevelScriptActor(const FObjectInitializer& ObjectInitializer);

};
