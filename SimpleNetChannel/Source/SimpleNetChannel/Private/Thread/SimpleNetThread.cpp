// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "SimpleNetThread.h"

int32 ThreadCount = 0;
FSimpleNetThread::FSimpleNetThread()
{
	FString RunnableName = *FString::Printf(TEXT("SimpleNetThread-%i"), ThreadCount);
	Thread = FRunnableThread::Create(this, *RunnableName,0, TPri_BelowNormal);
	
	ThreadCount++;
}

FSimpleNetThread *FSimpleNetThread::Bind(FSimpleDelegate InDelegate)
{
	FSimpleNetThread * NetThread = new FSimpleNetThread();
	NetThread->Delegate = InDelegate;

	return NetThread;
}

uint32 FSimpleNetThread::Run()
{
	Delegate.ExecuteIfBound();

	return true;
}