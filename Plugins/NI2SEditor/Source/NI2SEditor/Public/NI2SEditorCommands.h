// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "NI2SEditorStyle.h"

class FNI2SEditorCommands : public TCommands<FNI2SEditorCommands>
{
public:

	FNI2SEditorCommands()
		: TCommands<FNI2SEditorCommands>(TEXT("NI2SEditor"), NSLOCTEXT("Contexts", "NI2SEditor", "NI2SEditor Plugin"), NAME_None, FNI2SEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> TestButtonAction;

	TSharedPtr<FUICommandInfo> OpenTestWindow;
};
