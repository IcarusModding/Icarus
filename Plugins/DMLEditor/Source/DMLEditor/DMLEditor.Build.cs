using UnrealBuildTool;
using System.IO;
using System;

public class DMLEditor : ModuleRules
{
    public DMLEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {"DML", "Icarus", "UnrealEd", "BlueprintGraph"});
        bLegacyPublicIncludePaths = false;
        
        //DML transitive dependencies
        PublicDependencyModuleNames.AddRange(new[] {
            "Json"
        });
        
        //Icarus transitive dependencies
        PublicDependencyModuleNames.AddRange(new[] {
            "Core", "CoreUObject",
            "Engine",
            "InputCore",
            "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemEOS", "OnlineSubsystemUtils",
            "SignificanceManager",
            "APEX", "ApexDestruction",
            "BlueprintGraph",
            "KismetCompiler",
            "AnimGraphRuntime",
            "PhysXVehicles",
            "AssetRegistry",
            "NavigationSystem",
            "ReplicationGraph",
            "AIModule",
            "GameplayTasks",
            "SlateCore", "Slate", "UMG",
            "Projects",
            "FunctionalTestingEditor"
        });
        
        PrivateDependencyModuleNames.AddRange(new[] {
            "DesktopPlatform"
        });
    }
}