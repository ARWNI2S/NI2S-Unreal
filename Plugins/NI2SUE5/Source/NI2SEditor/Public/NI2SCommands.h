// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "NI2SEditorStyle.h"

class FNI2SCommands : public TCommands<FNI2SCommands>
{
public:

	FNI2SCommands()
		: TCommands<FNI2SCommands>(TEXT("NI2SEditor"), NSLOCTEXT("Contexts", "NI2SEditor", "NI2SEditor Plugin"), NAME_None, FNI2SEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> OpenPluginWindow;
	TSharedPtr<FUICommandInfo> PluginAction;
};