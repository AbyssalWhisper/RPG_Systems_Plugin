using UnrealBuildTool;

public class EasySettings : ModuleRules
{
    public EasySettings(ReadOnlyTargetRules Target) : base(Target)
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
                "RHI",
                "DeveloperSettings",
                "SlateCore"
            }
        );
    }
}