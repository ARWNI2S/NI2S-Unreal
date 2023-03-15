// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "Core/NI2SLevelScriptActor.h"
#include "NarrativeLevelScript.generated.h"

class ALevelNarrator;

/**
 * 
 */
UCLASS()
class NI2SSUBSYSTEM_API ANarrativeLevelScript : public ANI2SLevelScriptActor
{
	GENERATED_BODY()
	
public:
	ANarrativeLevelScript(const FObjectInitializer& ObjectInitializer);

public:
	/** Returns LevelNarrator subobject **/
	FORCEINLINE class ALevelNarrator* GetNarrator() const { return Narrator; }

	/** Name of the NarratorInstance. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName NarratorName;

private:
	UPROPERTY(Category = NI2S, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ALevelNarrator> Narrator;
};
