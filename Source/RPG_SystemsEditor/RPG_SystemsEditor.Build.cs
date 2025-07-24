using UnrealBuildTool;

public class RPG_SystemsEditor : ModuleRules
{
    public RPG_SystemsEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "RPG_Systems","UMG","ToolMenus", "EasyEditorExtend",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "Slate",
                "SlateCore",
                "AdvancedPreviewScene",
                "InputCore",
            }
        );
    }
}