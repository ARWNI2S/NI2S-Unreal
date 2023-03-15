// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "Components/NI2SActorComponentBase.h"

// Sets default values for this component's properties
UNI2SActorComponentBase::UNI2SActorComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNI2SActorComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNI2SActorComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

