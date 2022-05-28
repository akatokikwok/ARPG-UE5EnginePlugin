// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Core/Manage/ManageBase/ThreadManageBase.h"
#include "Interface/ThreadManageInterface.h"

//Bind call main thread            
//Create binding arbitrary thread 
struct SIMPLETHREAD_API FThreadGraphManage : FTreadTemplate<IGraphContainer, FGraphEventRef>
{
	static void Wait(const FGraphEventRef &EventRef);
	static void Wait(const FGraphEventArray &EventRef);
};