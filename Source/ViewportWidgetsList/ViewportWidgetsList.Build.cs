// Copyright kiruru002. All Rights Reserved.

using UnrealBuildTool;

public class ViewportWidgetsList : ModuleRules
{
    public ViewportWidgetsList(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "SlateCore",
                "Core",
                "UnrealEd",
                "EditorSubsystem",
                "DeveloperSettings",
                "UMG",
            }
        );
    }
}
