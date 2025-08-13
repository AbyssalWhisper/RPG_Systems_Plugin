using UnrealBuildTool;

public class Mods : ModuleRules
{
    public Mods(ReadOnlyTargetRules Target) : base(Target)
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
                "Slate","PakFile","AssetRegistry",
                "RenderCore",
                "RHI",
                "SlateCore", "BetterUtilities",
            }
        );
    }
}