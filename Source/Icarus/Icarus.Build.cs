// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Icarus : ModuleRules
{
	public Icarus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bLegacyPublicIncludePaths = false;
		ShadowVariableWarningLevel = WarningLevel.Warning;
	
		//Icarus transitive dependencies
		PublicDependencyModuleNames.AddRange(new[] {
			"Core", "CoreUObject",
			"Engine",
			"DeveloperSettings",
			"PhysicsCore",
			"InputCore",
			"OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemEOS", "OnlineSubsystemUtils",
			"SignificanceManager",
			"APEX", "ApexDestruction",
			"AnimGraphRuntime",
			"PhysXVehicles",
			"AssetRegistry",
			"NavigationSystem",
			"ReplicationGraph",
			"AIModule",
			"GameplayTasks",
			"SlateCore", "Slate", "UMG",
			"RenderCore",
			"CinematicCamera",
			"Foliage",
            "Niagara",
		});
		
		PublicDependencyModuleNames.AddRange(new[] {
			"AbstractInstance",
		});
	}
}
