// Copyright (c) 2023 ManOguaR's.


#include "GameFramework/NI2SPawn.h"

// Establece valores predeterminados
ANI2S_Pawn::ANI2S_Pawn()
{
	// Establezca esta pawn para llamar. Marca() cada fotograma. Puede desactivar esta opción para mejorar el rendimiento si no lo necesita.
	PrimaryActorTick.bCanEverTick = true;
}

// Se llama cuando se inicia el juego o cuando se genera
void ANI2S_Pawn::BeginPlay()
{
	Super::BeginPlay();
}

// Llamada a cada fotograma
void ANI2S_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Se llama para enlazar la funcionalidad a la entrada
void ANI2S_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

