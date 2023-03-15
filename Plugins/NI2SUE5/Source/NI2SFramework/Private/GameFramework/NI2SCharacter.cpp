// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "GameFramework/NI2SCharacter.h"

// Sets default values
ANI2SCharacter::ANI2SCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANI2SCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANI2SCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANI2SCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

