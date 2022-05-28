// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "ManageBase/ThreadManageBase.h"

//Bind synchronization binding will block the startup thread and activate it after completing the task            
//Create asynchronous binding starts directly and automatically destroys after the task is completed 
struct SIMPLETHREAD_API FThreadAbandonableManage :FTreadTemplate<IAbandonableContainer>
{

};