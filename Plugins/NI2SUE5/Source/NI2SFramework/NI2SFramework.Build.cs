// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

using UnrealBuildTool;

public class NI2SFramework : ModuleRules
{
    public NI2SFramework(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[] {
                System.IO.Path.Combine(ModuleDirectory, "Runtime"),
            });

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            });


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AIModule",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "NI2SNetwork",
            });
    }
}
