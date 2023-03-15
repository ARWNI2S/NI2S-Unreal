// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NI2SCharacter.generated.h"

UCLASS(Abstract)
class NI2SFRAMEWORK_API ANI2SCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANI2SCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
