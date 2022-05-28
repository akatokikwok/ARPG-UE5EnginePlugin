// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"

//Synchronous asynchronous thread, which is easy to use 
struct SIMPLETHREAD_API FSimpleAbandonable :FNonAbandonableTask
{
	FSimpleAbandonable(const FSimpleDelegate &InThreadDelegate);

	//Where threads really execute logic 
	void DoWork();

	//ID
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSimpleAbandonable, STATGROUP_ThreadPoolAsyncTasks);
	}
protected:

	//Bound events 
	FSimpleDelegate ThreadDelegate;
};