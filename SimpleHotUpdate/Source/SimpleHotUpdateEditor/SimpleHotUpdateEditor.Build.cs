// Copyright (C) RenZhai.2021.All Rights Reserved.

using UnrealBuildTool;

public class SimpleHotUpdateEditor : ModuleRules
{
	public SimpleHotUpdateEditor(ReadOnlyTargetRules Target) : base(Target)
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
				// ... add other public dependencies that you statically link with here ...
			}
			);

		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			});

		PublicDefinitions.Add("UE5_ENIGNE");//如果是UE5引擎 需要打开此选项，如果是UE4就把它注掉
		if (PublicDefinitions.Contains("UE5_ENIGNE"))
        {
			//如果使用UE4版本，请把它注掉，如果用UE5 请打开它 目前没办法进行只能操作 通过手动方式实现
			PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EditorFramework",
			});
		}

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",	
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ContentBrowser",
				"Settings",
				"PropertyEditor",
				"SimpleHotUpdate",
				"SimpleHotPak",
				"AssetRegistry"
				// ... add private dependencies that you statically link with here ...	
			});

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			});
	}
}
