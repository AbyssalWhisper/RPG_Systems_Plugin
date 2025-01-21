using UnrealBuildTool;

public class RPG_DialogSystemEditor : ModuleRules
{
    public RPG_DialogSystemEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "RPG_DialogSystemRuntime",
                "AssetTools","UnrealEd", "RPG_DialogSystemRuntime"
            }
        );
    }
}