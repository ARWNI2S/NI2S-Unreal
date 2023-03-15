// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "GameFramework/NI2SPawn.h"

// Establece valores predeterminados
ANI2SPawn::ANI2SPawn()
{
	// Establezca esta pawn para llamar. Marca() cada fotograma. Puede desactivar esta opción para mejorar el rendimiento si no lo necesita.
	PrimaryActorTick.bCanEverTick = true;
}

// Se llama cuando se inicia el juego o cuando se genera
void ANI2SPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Llamada a cada fotograma
void ANI2SPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Se llama para enlazar la funcionalidad a la entrada
void ANI2SPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

