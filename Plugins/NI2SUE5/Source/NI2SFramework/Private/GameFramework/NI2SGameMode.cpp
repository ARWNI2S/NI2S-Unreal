// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "GameFramework/NI2SGameMode.h"
#include "GameFramework/NI2SPlayerController.h"
#include "GameFramework/NI2SGameState.h"
#include "GameFramework/NI2SPlayerState.h"
#include "GameFramework/NI2SHUD.h"
#include "GameFramework/NI2SSpectatorPawn.h"

ANI2S_GameMode::ANI2S_GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ANI2S_PlayerController::StaticClass();

	//GameStateClass = ANI2S_GameState::StaticClass();
	PlayerStateClass = ANI2S_PlayerState::StaticClass();

	// set default pawn class to our Blueprinted character Blueprint'/NI2SUE5/GameFramework/BP_NI2SCharacter.BP_NI2SCharacter'
	static ConstructorHelpers::FClassFinder<APawn> NI2SPawnBPClass(TEXT("/NI2SUE5/GameFramework/BP_NI2SPawn"));
	if (NI2SPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = NI2SPawnBPClass.Class;
	}

	HUDClass = ANI2S_HUD::StaticClass();

	SpectatorClass = ANI2S_SpectatorPawn::StaticClass();
}
