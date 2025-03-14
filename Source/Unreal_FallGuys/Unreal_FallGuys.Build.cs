﻿// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Unreal_FallGuys : ModuleRules
{
    public Unreal_FallGuys(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.AddRange(new string[] {
        "Core",
        "CoreUObject",
        "Engine",
        "InputCore",
        "EnhancedInput",
        "HairStrandsCore",
        "Slate",
        "SlateCore",
        "UMG",
        "Sockets",        // 소켓 관련 기능 추가
        "Networking"      // ISocketSubsystem 사용을 위해 필요
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
