// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GrappleTarget : TargetRules
{
	public GrappleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        bUsesSteam = true;
        ExtraModuleNames.AddRange( new string[] { "Grapple" } );
        GlobalDefinitions.Add("UE4_PROJECT_STEAMGAMEDESC=\"Fadeout Underground\"");
        GlobalDefinitions.Add("UE4_PROJECT_STEAMPRODUCTNAME=\"fadeout\"");
        GlobalDefinitions.Add("UE4_PROJECT_STEAMGAMEDIR=\"fadeout\"");
    }
}
