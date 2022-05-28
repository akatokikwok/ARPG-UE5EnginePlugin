// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "Core/Manage/ManageBase/ThreadManageBase.h"

#ifdef PLATFORM_PROJECT
//Create Events that can be notified after asynchronous end can be bound 
struct SIMPLETHREAD_API FResourceLoadingManage :FTreadTemplateBase<IStreamableContainer,TSharedPtr<struct FStreamableHandle>>
{
	typedef FTreadTemplateBase<
		IStreamableContainer,
		TSharedPtr<struct FStreamableHandle>> Super;

	FResourceLoadingManage();
	~FResourceLoadingManage();

protected:

	//Set resource path 
	virtual void SetObjectPath(const TArray<FSoftObjectPath> &InObjectPath) final;

	//Get resource path 
	virtual TArray<FSoftObjectPath> &GetObjectPath() final;

	//Get stream management 
	virtual FStreamableManager *GetStreamableManager()final;

private:
	TArray<FSoftObjectPath> SoftObjectPath;
	FStreamableManager *StreamableManager;
};
#endif