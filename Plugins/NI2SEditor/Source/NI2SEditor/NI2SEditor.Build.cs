// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

using UnrealBuildTool;

public class NI2SEditor : ModuleRules
{
	public NI2SEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivateDependencyModuleNames.AddRange(
			new string[] {
                "Core",
                "CoreUObject",
                "ApplicationCore",
				"Slate",
                "SlateCore",
                "Engine",
                "InputCore",
                "EditorStyle",
                "EditorFramework",
                "UnrealEd", // for AssetEditorSubsystem
                "KismetWidgets",
                "Kismet",  // for FWorkflowCentricApplication
				"PropertyEditor",
                "RenderCore",
                "ContentBrowser",
                "WorkspaceMenuStructure",
                "MeshPaint",
                "EditorWidgets",
                "Projects",
                "NavigationSystem",
                "ToolMenus",
				"InteractiveToolsFramework",
                "EditorInteractiveToolsFramework",
                "NI2SFramework",
                "NI2SBPTools",
                "NI2SNetwork",
                "NI2SSubsystem",
            });

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Json",
            }
            );		

        PrivateIncludePathModuleNames.AddRange(
			new string[] {
                "Settings",
                "AssetTools",
                "LevelEditor"
            }
            );

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                "AssetTools"
            }
            );

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
    }
}
