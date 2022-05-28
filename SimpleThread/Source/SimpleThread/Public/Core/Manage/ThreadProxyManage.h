// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "ManageBase/ThreadManageBase.h"

//Bind creates threads and binds tasks, but does not execute. Join and detach determine whether to execute asynchronously or synchronously            
//Create is the quickest asynchronous way to create threads and execute tasks directly 
class SIMPLETHREAD_API FThreadProxyManage : public FTreadTemplate<IThreadProxyContainer, FThreadHandle>
{
public:
	~FThreadProxyManage();

	//synchronization 
	bool Join(FThreadHandle Handle);

	//asynchronous 
	bool Detach(FThreadHandle Handle);

	//Can I synchronize 
	EThreadState Joinable(FThreadHandle Handle);
};