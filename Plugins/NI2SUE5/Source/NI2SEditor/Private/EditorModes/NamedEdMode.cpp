// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorModes/NamedEdMode.h"
#include "EditorModes/NamedEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FNamedEdMode::EM_NamedEdModeId = TEXT("EM_NamedEdMode");

FNamedEdMode::FNamedEdMode()
{

}

FNamedEdMode::~FNamedEdMode()
{

}

void FNamedEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FNamedEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FNamedEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FNamedEdMode::UsesToolkits() const
{
	return true;
}




