// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Modules/ModuleInterface.h"

#define NI2S_EDITOR_MODULE_NAME "NI2SEditor"

//////////////////////////////////////////////////////////////////////////
// INI2SEditorModule

class INI2SEditorModule : public IModuleInterface
{
public:
	/** These functions will be bound to Commands. */
	virtual void TestButtonClicked() { }
	virtual void OpenTestWindowClicked() { }

	virtual TSharedPtr<class FExtensibilityManager> GetNI2SSceneEditorMenuExtensibilityManager() { return nullptr; }
	virtual TSharedPtr<class FExtensibilityManager> GetNI2SSceneEditorToolBarExtensibilityManager() { return nullptr; }

	virtual TSharedPtr<class FExtensibilityManager> GetNI2SPlotEditorMenuExtensibilityManager() { return nullptr; }
	virtual TSharedPtr<class FExtensibilityManager> GetNI2SPlotEditorToolBarExtensibilityManager() { return nullptr; }

	virtual uint32 GetNI2SAssetCategory() const = 0;
};