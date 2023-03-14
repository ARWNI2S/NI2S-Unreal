// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class NI2SUnrealServerTarget : TargetRules
{
    public NI2SUnrealServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        //IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        ExtraModuleNames.Add("NI2SUnreal");
    }

    //public override void SetupBinaries(TargetInfo Target,
    //    ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
    //    ref List<string> OutExtraModuleNames)
    //{
    //    OutExtraModuleNames.Add("NI2SUnreal");
    //}
}
