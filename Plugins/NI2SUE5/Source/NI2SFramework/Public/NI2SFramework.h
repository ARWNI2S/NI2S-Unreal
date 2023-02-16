// Copyright (c) 2023 ManOguaR's.


#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FNI2SFrameworkModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
