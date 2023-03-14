// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#include "Modes/NI2SEditorMode.h"
#include "Modes/NI2SEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "Modes/NI2SEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/NI2SEditorSimpleTool.h"
#include "Tools/NI2SEditorInteractiveTool.h"

// step 2: register a ToolBuilder in FNI2SEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "NI2SEditorMode"

const FEditorModeID UNI2SEditorMode::EM_NI2SEditorModeId = TEXT("EM_NI2SEditorMode");

FString UNI2SEditorMode::SimpleToolName = TEXT("NI2SEditor_ActorInfoTool");
FString UNI2SEditorMode::InteractiveToolName = TEXT("NI2SEditor_MeasureDistanceTool");


UNI2SEditorMode::UNI2SEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UNI2SEditorMode::EM_NI2SEditorModeId,
		LOCTEXT("ModeName", "NI2SEditor"),
		FSlateIcon(),
		true);
}


UNI2SEditorMode::~UNI2SEditorMode()
{
}


void UNI2SEditorMode::ActorSelectionChangeNotify()
{
}

void UNI2SEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FNI2SEditorModeCommands& SampleToolCommands = FNI2SEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UNI2SEditorSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UNI2SEditorInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UNI2SEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FNI2SEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UNI2SEditorMode::GetModeCommands() const
{
	return FNI2SEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
