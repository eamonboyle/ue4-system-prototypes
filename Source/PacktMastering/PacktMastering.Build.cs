// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PacktMastering : ModuleRules
{
	public PacktMastering(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
