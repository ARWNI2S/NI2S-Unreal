// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NI2SEditorStyle.h"

/**
 * Interface class that all module implementations should derive from.  This is used to initialize
 * a module after it's been loaded, and also to clean it up before the module is unloaded.
 */
class INI2SRegistrationsInterface
{

public:

	/**
	 * Note: Even though this is an interface class we need a virtual destructor here because modules are deleted via a pointer to this interface
	 */
	virtual ~INI2SRegistrationsInterface()
	{
	}

	/**
	 * Called right after the module DLL has been loaded and the module object has been created
	 * Load dependent modules here, and they will be guaranteed to be available during ShutdownModule. ie:
	 *
	 * FModuleManager::Get().LoadModuleChecked(TEXT("HTTP"));
	 */
	virtual void StartupModuleRegistrations()
	{
	}

};

class FGlobalEditorRegistrations : public INI2SRegistrationsInterface
{
public:

	/** INI2SRegistrations implementation */
	virtual void StartupModuleRegistrations() override;

};

class NI2SRegistrations
{
public:
	static void InitializeStyles() {
		// Initialize style
		FNI2SEditorStyle::Initialize();
		FNI2SEditorStyle::ReloadTextures();
	}
};