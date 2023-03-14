// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "Level/NarrativeLevelScript.h"
#include "Level/DefaultNarrator.h"

FName ANarrativeLevelScript::NarratorName(TEXT("DefaultNarrator"));


ANarrativeLevelScript::ANarrativeLevelScript(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (!Narrator)
	{
		Narrator = CreateDefaultSubobject<ADefaultNarrator>(ANarrativeLevelScript::NarratorName);
	}
}
