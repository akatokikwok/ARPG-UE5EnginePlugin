// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Interface/ProxyInterface.h"
#include "Core/SimpleSemaphore.h"

//UE4 RunnableThreads, you can create thread instances 
class SIMPLETHREAD_API FThreadRunnable : public FRunnable, public IThreadProxy
{
public:
	FThreadRunnable(bool InSuspendAtFirst = false);
	virtual ~FThreadRunnable();

	//Create a secure thread 
	virtual void CreateSafeThread();

	//Wake up thread 
	virtual void WakeupThread();

	//Whether the thread is suspended 
	virtual bool IsSuspend();

	//Let the startup thread wait until the current task is completed 
	virtual void WaitAndCompleted();

	//Block start thread 
	virtual void BlockingAndCompletion();

private:

	//Where threads actually execute 
	virtual uint32 Run();

	virtual bool Init();
	virtual void Stop();
	virtual void Exit();

private:

	
	FThreadSafeCounter					StopTaskCounter; //Thread safety count .
	bool								bSuspendAtFirst; //Whether to block the thread for the first time .
	class FRunnableThread*				Thread;			 //Thread specific instance 
	FName								RunnableName;	 //Thread name will be extended later 
	FSimpleSemaphore					ThreadEvent;     //Semaphore blocking thread 
	FSimpleSemaphore					StartUpEvent;	 //Semaphore block start thread 
	FSimpleSemaphore					WaitExecuteEvent;//Semaphore suspend start thread 
	FCriticalSection					Mutex;			 //lock 

	static int32						ThreadCount;	 //Thread sequence count 
};