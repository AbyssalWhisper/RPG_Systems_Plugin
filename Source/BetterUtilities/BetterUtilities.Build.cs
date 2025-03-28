// Some copyright should be here...

using UnrealBuildTool;

public class BetterUtilities : ModuleRules
{
	public BetterUtilities(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "JsonBlueprintUtilities",
				// ... add other public dependencies that you statically link with here ...
			}
			);
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"GameplayAbilities",
				"GameplayTags",
                "PhysicsCore","Json",
                "JsonUtilities","Niagara",
                "Chaos","EnhancedInput",
                "JsonBlueprintUtilities"
				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	if (Target.Type == TargetType.Editor)
		{
			PublicDependencyModuleNames.Add("UnrealEd"); 
			PublicDependencyModuleNames.Add("GraphEditor"); 
			PublicDependencyModuleNames.Add("BlueprintGraph"); 
			PublicDependencyModuleNames.Add("KismetCompiler"); 
			// Adiciona o módulo UnrealEd apenas para builds de Editor
		}
		
	}
}
