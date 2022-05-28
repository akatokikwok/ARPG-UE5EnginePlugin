// Copyright (C) RenZhai.2019.All Rights Reserved.

/* The following website explains how to use this set of plug-ins
 * DocURL£º			    https://zhuanlan.zhihu.com/p/78023533
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
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimpleThreadModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
