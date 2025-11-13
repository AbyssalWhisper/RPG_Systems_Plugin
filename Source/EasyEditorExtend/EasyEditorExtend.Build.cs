using UnrealBuildTool;

public class EasyEditorExtend : ModuleRules
{
    public EasyEditorExtend(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "AssetRegistry", "UnrealEd",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore","ToolMenus","EditorSubsystem", "Blutility" ,"UMG","UMGEditor", "EditorScriptingUtilities"
            }
        );
         
    }
}