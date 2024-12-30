// Copyright Dragon Corp. Games, S.L. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class NI2SUnrealEditorTarget : TargetRules
{
	public NI2SUnrealEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange(new string[] { "NI2SUnreal", "NI2SClient", "NI2SServer" });
	}
}
