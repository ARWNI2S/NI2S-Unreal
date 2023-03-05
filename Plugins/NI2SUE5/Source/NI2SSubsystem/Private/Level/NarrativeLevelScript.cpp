// Fill out your copyright notice in the Description page of Project Settings.


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
