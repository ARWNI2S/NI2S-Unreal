// Copyright (c) 2023 ManOguaR's.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NI2SActor.generated.h"

UCLASS(Abstract)
class NI2SFRAMEWORK_API ANI2S_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANI2S_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
