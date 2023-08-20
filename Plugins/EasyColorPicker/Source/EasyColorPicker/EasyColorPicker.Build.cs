// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class EasyColorPicker : ModuleRules {

    public EasyColorPicker(ReadOnlyTargetRules Target) : base(Target) {

      PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
      // PrivatePCHHeaderFile = "Private/EasyColorPickerPrivatePCH.h";
      PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
      PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

      PublicDependencyModuleNames.AddRange(new string[] {
        "Engine", "Core", "CoreUObject", "UMG", "SlateCore", "Slate"
      });

    }
}
