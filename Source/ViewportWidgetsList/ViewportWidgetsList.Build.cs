// Copyright kiruru002. All Rights Reserved.

using UnrealBuildTool;

public class ViewportWidgetsList : ModuleRules
{
    public ViewportWidgetsList(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePathModuleNames.AddRange(
            new string[]
            {
                "Blutility",
            }
        );
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine",
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "EditorSubsystem",
                "DeveloperSettings",
                "UMG",
                "UMGEditor",
                "ToolMenus",
                "Blutility",
            }
        );
    }
}
