// Copyright 2017-2018 HowToCompute. All Rights Reserved.

using UnrealBuildTool;

public class ConfigBPPlugin : ModuleRules
{
	public ConfigBPPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"ConfigBPPlugin/Public"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"ConfigBPPlugin/Private",
			}
			);
			
		
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
			}
			);

        // Force IWYU
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    }
}
