// Some copyright should be here...

using UnrealBuildTool;

public class RPG_Systems : ModuleRules
{
	public RPG_Systems(ReadOnlyTargetRules Target) : base(Target)
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
                "CoreOnline","NavigationSystem","AIModule","Core"
                ,"GameplayTags", "GameplayAbilities","AIModule" 
                , "GameplayTasks", "Niagara" ,"PhysicsCore" 
                , "BetterUtilities", "Mover","Chaos","InputCore",
                "EnhancedInput","AsyncTickPhysics","MoverExamples"
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
				"Slate",
				"SlateCore"
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
