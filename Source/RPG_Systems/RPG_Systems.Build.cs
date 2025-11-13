// Some copyright should be here...

using System.IO;
using UnrealBuildTool;

public class RPG_Systems : ModuleRules
{
	public RPG_Systems(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		string EngineSourceDirectory = Path.GetFullPath(Target.RelativeEnginePath);
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				//Path.Combine(ModuleDirectory, "Shaders", "Private")
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				System.IO.Path.Combine(GetModuleDirectory("Engine"), "Private"),
				Path.Combine(EngineSourceDirectory, "Source/Runtime/Engine/Private/Materials")
			
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "CoreOnline","NavigationSystem","AIModule","Core"
                ,"GameplayTags", "GameplayAbilities","AIModule" 
                , "GameplayTasks", "Niagara" ,"PhysicsCore" 
                , "BetterUtilities", "Mover","Chaos","InputCore",
                "EnhancedInput","AsyncTickPhysics",
                "MoverExamples",
                "OnlineSubsystem",
                "JsonBlueprintUtilities","NetCore"
                ,"Networking","OnlineSubsystem"
				// ... add other public dependencies that you statically link with here ...
			}
			);

        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "FunctionalTesting", });
        }
        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"UMG",
				"Slate",
				"SlateCore",
				"RenderCore",
				"Renderer",
				"RHI",
				"DeveloperSettings", "ChaosMover"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		 
	}
}
