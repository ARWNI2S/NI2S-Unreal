// Copyright (c) 2023 ManOguaR's.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NI2SPawn.generated.h"

UCLASS(Abstract)
class NI2SFRAMEWORK_API ANI2S_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Establece los valores predeterminados para las propiedades de este pawn
	ANI2S_Pawn();

protected:
	// Se llama cuando se inicia el juego o cuando se genera
	virtual void BeginPlay() override;

public:
	// Llamada a cada fotograma
	virtual void Tick(float DeltaTime) override;

	// Se llama para enlazar la funcionalidad a la entrada
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
