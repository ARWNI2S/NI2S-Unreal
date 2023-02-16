// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SCommands.h"

#define LOCTEXT_NAMESPACE "FNI2SEditorModule"

void FNI2SCommands::RegisterCommands()
{
	//TODO: change text & friendly name
	UI_COMMAND(OpenPluginWindow, "PluginWin", "Bring up PluginWin window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PluginAction, "PluginBtn", "Execute PluginBtn action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
