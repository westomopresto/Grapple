// Copyright 2018 Maksim Shestakov. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class BlueprintJson : ModuleRules
	{
		public BlueprintJson(ReadOnlyTargetRules Target) : base(Target)
		{
            DefaultBuildSettings = BuildSettingsVersion.V2;
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            //MinFilesUsingPrecompiledHeaderOverride = 1; 
            //bFasterWithoutUnity = true;
			
			PrivateDependencyModuleNames.AddRange(
                new string[] { 
                    "Core", 
                    "CoreUObject",
                    "Engine",
                    "Json"
                }
            );
		}
	}
}
