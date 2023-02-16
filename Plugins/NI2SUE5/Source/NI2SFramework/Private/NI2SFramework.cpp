// Copyright (c) 2023 ManOguaR's.


#include "NI2SFramework.h"

#define LOCTEXT_NAMESPACE "FNI2SFrameworkModule"

void FNI2SFrameworkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNI2SFrameworkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNI2SFrameworkModule, NI2SFramework)