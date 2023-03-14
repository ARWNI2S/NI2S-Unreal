// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

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
