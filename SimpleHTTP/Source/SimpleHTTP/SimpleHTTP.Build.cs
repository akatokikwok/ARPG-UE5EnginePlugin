// // Copyright (C) RenZhai.2020.All Rights Reserved.
/*
The following website explains how to use this set of plug-ins
DocURL： https://zhuanlan.zhihu.com/p/82195344

The following website explains how to develop the current plug-in
MarketplaceURL : https://www.aboutcg.org/courseDetails/682/introduce

If you want to learn more about UE4 tutorial, please refer to:
URL : https://zhuanlan.zhihu.com/p/60117613

If you want to systematically understand the Renzhai series of tutorials and related plug-ins iterative update, you can pay attention to my blog
URL : http://renzhai.net/

If you want to know our next course arrangement, you can search the official account in WeChat public address, then we will push many technical articles.
Sina blog https://weibo.com/BZRZ/profile?s=6cm7D0 //This blog hasn't been used for three or four years since it was 16 years ago. Maybe we can reactivate the first link in the future
*/

using UnrealBuildTool;

public class SimpleHTTP : ModuleRules
{
	public SimpleHTTP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
	//	bUsePrecompiled = true;
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
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
            {
                "CoreUObject",
				// ... add private dependencies that you statically link with here ...	
			}
			);

    //    PublicDefinitions.Add("PLATFORM_PROJECT");
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
