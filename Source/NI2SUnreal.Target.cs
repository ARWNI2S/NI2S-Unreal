// Copyright Dragon Corp. Games, S.L. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class NI2SUnrealTarget : TargetRules
{
	public NI2SUnrealTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange(new string[] { "NI2SUnreal", "NI2SClient" });
    }
}
