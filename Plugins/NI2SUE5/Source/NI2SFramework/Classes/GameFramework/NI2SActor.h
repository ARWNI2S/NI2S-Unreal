// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/NI2SActorBase.h"
#include "NI2SActor.generated.h"

/*
* Blueprintable Actor class for extensible NI2S Actor in blueprints.
*/
UCLASS(Blueprintable)
class NI2SFRAMEWORK_API ANI2SActor : public ANI2SActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANI2SActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

