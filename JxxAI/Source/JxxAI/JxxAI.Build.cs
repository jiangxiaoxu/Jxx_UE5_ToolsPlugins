// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JxxAI : ModuleRules
{
	public JxxAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        //设置为cpp20也可,但是为了方便用上最新的标准还是设置为Latest
        CppStandard = CppStandardVersion.Latest;

        //development build 也不需要优化代码.
        OptimizeCode = CodeOptimization.InShippingBuildsOnly;

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
				"Core",
                "StateTreeModule",
                "AIModule",
                "NavigationSystem",
                "GameplayTasks",
                "GameplayStateTreeModule",
                "StructUtils","InstancedStructWrapper",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore", 
                
               "GameplayAbilities","GameplayTags",
               "SmartObjectsModule", "GameplayBehaviorsModule","GameplayBehaviorSmartObjectsModule",
               "AIModule",
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
