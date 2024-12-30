// Copyright Dragon Corp. Games, S.L. All Rights Reserved.

#include "NI2SEditCore.h"

#define LOCTEXT_NAMESPACE "FNI2SEditCoreModule"

void FNI2SEditCoreModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNI2SEditCoreModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNI2SEditCoreModule, NI2SEditCore)