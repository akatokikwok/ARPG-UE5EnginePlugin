// Copyright (C) RenZhai.2021.All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class SimpleHotUpdate : ModuleRules
{
    public SimpleHotUpdate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"HTTP"
				// ... add other public dependencies that you statically link with here ...
			}
			);

			PublicDefinitions.Add("SIMPLEHOTUPDATE_OPEN_ENGINE_MODULE");//如果在独立程序里面有应用 就把它注掉
			if (PublicDefinitions.Contains("SIMPLEHOTUPDATE_OPEN_ENGINE_MODULE"))
			{
				PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Engine"
				});
			}

			PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Slate",
				"SlateCore",
				"Json",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
