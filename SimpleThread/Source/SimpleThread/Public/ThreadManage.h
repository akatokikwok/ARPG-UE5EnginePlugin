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

#pragma once

#include "CoreMinimal.h"
#include "Core/Manage/ThreadProxyManage.h"
#include "Core/Manage/ThreadTaskManage.h"
#include "Core/Manage/ThreadAbandonableManage.h"
#include "Core/Manage/CoroutinesManage.h"
#include "Core/Manage/ResourceLoadingManage.h"
#include "Core/Manage/ThreadGraphManage.h"
#include "SimpleTreadPlatform.h"

#ifdef PLATFORM_PROJECT
#include "Tickable.h"
#endif
//Total management of threads 
namespace SimpleGThread
{
	//FThreadManagement 
	//It is thread safe and is not prone to deadlock 
	class SIMPLETHREAD_API FThreadManagement : public TSharedFromThis<FThreadManagement>
#ifdef PLATFORM_PROJECT
		,public FTickableGameObject
#endif
	{
	public:
		virtual ~FThreadManagement(){}
		static TSharedRef<FThreadManagement> Get();

		//The single instance needs to be deleted manually 
		static void Destroy();

		//Monitored by main thread 
		virtual void Tick(float DeltaTime);
		virtual TStatId GetStatId() const;

	public:
		static FThreadProxyManage		&GetProxy() { return Get()->ThreadProxyManage; }
		static FThreadTaskManagement	&GetTask() { return Get()->ThreadTaskManagement; }
		static FThreadAbandonableManage &GetAbandonable() { return Get()->ThreadAbandonableManage; }
		static FCoroutinesManage		&GetCoroutines() { return Get()->CoroutinesManage; }
		static FThreadGraphManage		&GetGraph() { return Get()->ThreadGraphManage; }

#ifdef PLATFORM_PROJECT
		static FResourceLoadingManage& GetResourceLoading() { return Get()->ResourceLoadingManage; }
#endif
	
	protected:
		//User defined thread creation, which can easily and directly create threads 
		FThreadProxyManage ThreadProxyManage;

		//Custom thread pool, which can drop tasks to the thread pool for execution 
		FThreadTaskManagement ThreadTaskManagement;

		//Take thread execution directly from UE4 thread pool 
		FThreadAbandonableManage ThreadAbandonableManage;

		// Association
		FCoroutinesManage CoroutinesManage;

		//Thread chart 
		FThreadGraphManage ThreadGraphManage;
#ifdef PLATFORM_PROJECT
		//Resource read 
		FResourceLoadingManage ResourceLoadingManage;
#endif
	private:
		static TSharedPtr<FThreadManagement> ThreadManagement;
	};
}
using namespace SimpleGThread;

typedef SimpleGThread::FThreadManagement GThread;