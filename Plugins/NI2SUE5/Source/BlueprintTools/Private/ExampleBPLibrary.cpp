// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExampleBPLibrary.h"
#include "BlueprintTools.h"

UExampleBPLibrary::UExampleBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UExampleBPLibrary::BlueprintToolsSampleFunction(float Param)
{
	return -1;
}

