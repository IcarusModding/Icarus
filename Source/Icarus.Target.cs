// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class IcarusTarget : TargetRules
{
	public IcarusTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		LinkType = TargetLinkType.Modular;

		ExtraModuleNames.AddRange( new string[] { "Icarus" } );
		DefaultBuildSettings = BuildSettingsVersion.V2;
	}
}
