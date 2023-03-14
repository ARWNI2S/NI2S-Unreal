// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "GameFramework/NI2SActor.h"

// Sets default values
ANI2S_Actor::ANI2S_Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANI2S_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANI2S_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

