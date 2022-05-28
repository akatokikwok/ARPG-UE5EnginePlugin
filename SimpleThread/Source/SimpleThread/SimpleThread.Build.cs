// Copyright (C) RenZhai.2019.All Rights Reserved.

/* The following website explains how to use this set of plug-ins 
 * DocURL：			    https://zhuanlan.zhihu.com/p/78023533            
 * Here's how to develop the current plug-in            
 * MarketplaceURL :     http://www.aboutcg.org/course/tut_ue4suite_190813/?action=            
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

public class SimpleThread : ModuleRules
{
	public SimpleThread(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		// bEnforceIWYU = false;
		//bUsePrecompiled = true;
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
				"CoreUObject",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{

				//"Engine",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		//如果是在项目里面想用到该内容 就把它打开
	//	PublicDefinitions.Add("PLATFORM_PROJECT");
		if (PublicDefinitions.Contains("PLATFORM_PROJECT"))
        {
			PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine"
			});
		}

		//bUsePrecompiled = true;
		//PrecompileForTargets = PrecompileTargetsType.Any;
	}
}
