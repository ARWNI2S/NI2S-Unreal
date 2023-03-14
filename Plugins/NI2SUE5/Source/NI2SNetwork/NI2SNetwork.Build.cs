// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NI2SNetwork : ModuleRules
{
	public NI2SNetwork( ReadOnlyTargetRules Target ) : base( Target )
	{
        PublicIncludePaths.AddRange(
            new string[] {
                System.IO.Path.Combine(ModuleDirectory, ".."),
                System.IO.Path.Combine(ModuleDirectory, "Public")
            }
        );

        PrivateDependencyModuleNames.AddRange
			(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Sockets",
				"Networking"
			}
		);
	}
}