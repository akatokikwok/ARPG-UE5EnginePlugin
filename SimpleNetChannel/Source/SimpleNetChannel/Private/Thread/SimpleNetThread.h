// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

class FSimpleNetThread :public FRunnable
{
public:
	FSimpleNetThread();

	static FSimpleNetThread *Bind(FSimpleDelegate InDelegate);

	virtual uint32 Run();

private:
	FRunnableThread* Thread;
	FSimpleDelegate Delegate;
};