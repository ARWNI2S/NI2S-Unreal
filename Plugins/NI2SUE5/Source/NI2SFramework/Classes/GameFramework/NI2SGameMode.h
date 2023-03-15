// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NI2SGameMode.generated.h"

/**
 * 
 */
UCLASS(meta = (ShortTooltip = "Game Mode Base defines the game being played, its rules, scoring, and other facets of the game type."))
class NI2SFRAMEWORK_API ANI2SGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ANI2SGameMode();
};
