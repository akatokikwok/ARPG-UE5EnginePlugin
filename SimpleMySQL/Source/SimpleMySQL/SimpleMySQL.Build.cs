// Copyright (C) RenZhai.2020.All Rights Reserved.
 /*
 /* The following website explains how to use this set of plug-ins 
 * DocURL：			                
 * Here's how to develop the current plug-in            
 * MarketplaceURL :     https://www.aboutcg.org/courseDetails/979/introduce          
 * If you want to learn more about the UE4 tutorial, please refer to:            
 * URL :				https://zhuanlan.zhihu.com/p/60117613            
 * If you want to learn about the series of tutorials and related plug-ins, you can follow my blog            
 * URL :				http://renzhai.net/            
 * If you want to know our next course arrangement, 
 * you can search the WeChat public house for your attention, then we will push a lot of technical articles.           
 * Sina blog            https://weibo.com/bzrz/profile?S=6cm7d0 // this blog hasn't been used for three or four years now.
 * Maybe you can re enable the first connection later 
 */
using UnrealBuildTool;
using System.IO;

public class SimpleMySQL : ModuleRules
{
    public string ProjectDirectory
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory,"../../../../"));//项目路径
        }
    }

    private void CopyToProjectBinaries(string FilePathName, ReadOnlyTargetRules Target)
    {
        string BineriesDirectory = Path.Combine(ProjectDirectory, "Binaries", Target.Platform.ToString());
        string Filename = Path.GetFileName(FilePathName);

        if(!Directory.Exists(BineriesDirectory))
        {
            Directory.CreateDirectory(BineriesDirectory);
        }

        string ProjectFileFullName = Path.Combine(BineriesDirectory, Filename);
        if (!File.Exists(ProjectFileFullName))
        {
            File.Copy(FilePathName, ProjectFileFullName,true);
        }

        RuntimeDependencies.Add(ProjectFileFullName);
    }

	public SimpleMySQL(ReadOnlyTargetRules Target) : base(Target)
    {
       // bUsePrecompiled = true;
        bEnableUndefinedIdentifierWarnings = false;
        PublicDefinitions.Add("NTDDI_WIN7SP1");

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		if (Target.Platform == UnrealBuildTool.UnrealTargetPlatform.Win64)
		{
			string PluginsDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));//路径
			string SQLLibDirectory = Path.Combine(PluginsDirectory, "MySQL"); //MySQL路径
			string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";

			string MysqlPath = Path.Combine(SQLLibDirectory, PlatformString);

			PublicIncludePaths.Add(MysqlPath);

			RuntimeDependencies.Add(Path.Combine(MysqlPath, "libmysql.dll"));
			PublicAdditionalLibraries.Add(Path.Combine(MysqlPath, "libmysql.lib"));

			CopyToProjectBinaries(Path.Combine(MysqlPath, "libmysql.dll"), Target);
			CopyToProjectBinaries(Path.Combine(MysqlPath, "libmysql.lib"), Target);
		}

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
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "CoreUObject",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			});

		if (PublicDefinitions.Contains("PLATFORM_PROJECT"))
		{
			PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine"
			});
		}

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		//bUsePrecompiled = true;
		//PrecompileForTargets = PrecompileTargetsType.Any;
	}
}
