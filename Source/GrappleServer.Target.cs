// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GrappleServerTarget : TargetRules
{
	public GrappleServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
        bUsesSteam = true;
        bUseLoggingInShipping = true;
        ExtraModuleNames.AddRange( new string[] { "Grapple" } );
    }
}