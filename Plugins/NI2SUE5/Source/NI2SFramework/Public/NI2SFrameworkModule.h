// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"
#include "Modules/ModuleInterface.h"

#define NI2SFRAMEWORK_MODULE_NAME "NI2SFramework"

//////////////////////////////////////////////////////////////////////////
// INI2SFrameworkModuleInterface

class INI2SFrameworkModuleInterface : public IModuleInterface
{
};

//////////////////////////////////////////////////////////////////////////
// NI2S stats

DECLARE_STATS_GROUP(TEXT("NI2S"), STATGROUP_NI2SFRAMEWORK, STATCAT_Advanced);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Service Status (Ping)"), STAT_NI2S_ServiceStatus, STATGROUP_NI2SFRAMEWORK, NI2SFRAMEWORK_API);
