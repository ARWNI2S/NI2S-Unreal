// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "NI2SFrameworkModule.h"
#include "Misc/ConfigCacheIni.h"
#include "Modules/ModuleManager.h"
#include "NI2SFrameworkPrivate.h"
//#include "PaperRenderSceneProxy.h"

DEFINE_STAT(STAT_NI2S_ServiceStatus);

//////////////////////////////////////////////////////////////////////////
// FNI2SFrameworkModule

class FNI2SFrameworkModule : public INI2SFrameworkModuleInterface
{
public:
	virtual void StartupModule() override
	{
		//TODO: CHECK NETWORKING FROM SETTINGS
	}

	virtual void ShutdownModule() override
	{
		//TODO: CHECK NETWORKING FROM SETTINGS
	}
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FNI2SFrameworkModule, NI2SFramework);
DEFINE_LOG_CATEGORY(LogNI2SFramework);
