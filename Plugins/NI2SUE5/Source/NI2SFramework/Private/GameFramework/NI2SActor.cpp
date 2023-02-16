// Copyright (c) 2023 ManOguaR's.


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

