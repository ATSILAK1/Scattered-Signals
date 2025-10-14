// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Scattered_Signals : ModuleRules
{
	public Scattered_Signals(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
