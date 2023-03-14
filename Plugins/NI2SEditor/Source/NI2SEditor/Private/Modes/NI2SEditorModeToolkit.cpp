// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#include "Modes/NI2SEditorModeToolkit.h"
#include "Modes/NI2SEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "NI2SEditorModeToolkit"

FNI2SEditorModeToolkit::FNI2SEditorModeToolkit()
{
}

void FNI2SEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FNI2SEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FNI2SEditorModeToolkit::GetToolkitFName() const
{
	return FName("NI2SEditorMode");
}

FText FNI2SEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "NI2SEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
