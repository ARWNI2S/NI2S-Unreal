// Fill out your copyright notice in the Description page of Project Settings.

#include "NI2SEditorCommands.h"

#define LOCTEXT_NAMESPACE "FNI2SEditorModule"

void FNI2SEditorCommands::RegisterCommands()
{
	// Register TestButton Action
	UI_COMMAND(TestButtonAction, "TestButton Action", "Execute NI2SEditor test button action", EUserInterfaceActionType::Button, FInputChord());
	// Register Open TestWindow
	UI_COMMAND(OpenTestWindow, "Open TestWindow", "Bring up NI2SEditor test window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
