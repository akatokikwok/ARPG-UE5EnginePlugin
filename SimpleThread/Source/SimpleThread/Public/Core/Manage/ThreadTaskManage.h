// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "ManageBase/ThreadManageBase.h"

//Bind is added to the task queue. If there is an empty thread, the task can be executed directly            
//Create directly runs the current task if there are idle threads in the process pool 
class SIMPLETHREAD_API FThreadTaskManagement : public FTreadTemplate<IThreadTaskContainer>
{
public:
	FThreadTaskManagement();
	~FThreadTaskManagement();

	//Initialization is mainly to initialize the thread pool 
	void Init(int32 ThreadNum);

	//Check for idle threads 
	void Tick(float DeltaTime);
};

