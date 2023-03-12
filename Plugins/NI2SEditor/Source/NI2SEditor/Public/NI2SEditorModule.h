// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * This is the module definition for the editor mode. You can implement custom functionality
 * as your plugin module starts up and shuts down. See IModuleInterface for more extensibility options.
 */
class FNI2SEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** These functions will be bound to Commands. */
	void TestButtonClicked();
	void OpenTestWindowClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnTestTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> TestCommands;

};
