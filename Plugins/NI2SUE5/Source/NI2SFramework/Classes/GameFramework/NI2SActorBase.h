// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NI2SActorBase.generated.h"

/*
* Abstract AActor class for every narrative-relevant Actor in game,
* encapsulates all NI2SActor automations and is not inheritable by blueprints.
* TODO: Cast Issue
*/
UCLASS(Abstract, NotBlueprintable)
class NI2SFRAMEWORK_API ANI2SActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANI2SActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
