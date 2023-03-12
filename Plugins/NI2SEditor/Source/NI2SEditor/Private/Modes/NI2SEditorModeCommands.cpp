// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modes/NI2SEditorModeCommands.h"
#include "Modes/NI2SEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "NI2SEditorModeCommands"

FNI2SEditorModeCommands::FNI2SEditorModeCommands()
	: TCommands<FNI2SEditorModeCommands>("NI2SEditorMode",
		NSLOCTEXT("NI2SEditorMode", "NI2SEditorModeCommands", "NI2S Editor Mode"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
{
}

void FNI2SEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FNI2SEditorModeCommands::GetCommands()
{
	return FNI2SEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
