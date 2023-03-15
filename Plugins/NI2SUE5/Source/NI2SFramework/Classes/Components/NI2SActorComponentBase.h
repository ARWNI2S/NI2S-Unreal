// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NI2SActorComponentBase.generated.h"


UCLASS(Abstract, ClassGroup=(NI2S), meta=(BlueprintSpawnableComponent))
class NI2SFRAMEWORK_API UNI2SActorComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNI2SActorComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};