// Copyright (C) RenZhai.2019.All Rights Reserved.

#include "ThreadManage.h"
#include "Core/ThreadCoreMacro.h"

TSharedPtr<FThreadManagement> FThreadManagement::ThreadManagement = nullptr;

FCoroutinesManage::FCoroutinesManage()
	:FTreadTemplateBase<ICoroutinesContainer, FCoroutinesHandle>()
{}

TSharedRef<FThreadManagement> FThreadManagement::Get()
{
	if (!ThreadManagement.IsValid())
	{
		ThreadManagement = MakeShareable(new FThreadManagement);
	}

	return ThreadManagement.ToSharedRef();
}

void FThreadManagement::Destroy()
{
	if (ThreadManagement.IsValid())
	{
		ThreadManagement = nullptr;
	}
}

void FThreadManagement::Tick(float DeltaTime)
{
	//Thread task 
	ThreadTaskManagement.Tick(DeltaTime);

	//Coordination task 
	CoroutinesManage.Tick(DeltaTime);
}

TStatId FThreadManagement::GetStatId() const
{
	return TStatId();
}

FThreadTaskManagement::~FThreadTaskManagement()
{
	for (auto &Tmp : *this)
	{
		Tmp->WaitAndCompleted();
	}
}

FThreadTaskManagement::FThreadTaskManagement()
{
	Init(CPUThreadNumber);
//	SIMPLE_THREAD_INFO_MSG(TEXT("ThreadTaskManagement Initialize thread pool,%i thread pool"), CPUThreadNumber);
}

void FThreadTaskManagement::Init(int32 ThreadNum)
{
	for (int32 i = 0; i < ThreadNum; i++)
	{
		*this << MakeShareable(new FThreadRunnable);
	}

	FPlatformProcess::Sleep(0.1f);
}

void FThreadTaskManagement::Tick(float DeltaTime)
{
	TSharedPtr<IThreadProxy> ThreadProxy = nullptr;
	{
		MUTEX_LOCL;
		for (auto &Tmp : *this)
		{
			if (Tmp->IsSuspend())
			{
				ThreadProxy = Tmp;
				break;
			}
		}
	}

	if (ThreadProxy.IsValid())
	{
		if (!((TEventQueue *)this)->IsEmpty())
		{
			FSimpleDelegate SimpleDelegate;
			if (*this <<= SimpleDelegate)
			{
				MUTEX_LOCL;
				ThreadProxy->GetThreadDelegate() = SimpleDelegate;
				ThreadProxy->WakeupThread();
			}
		}
	}
}

FThreadProxyManage::~FThreadProxyManage()
{
	for (auto &Tmp : *this)
	{
		Tmp->WaitAndCompleted();
	}
}

bool FThreadProxyManage::Join(FThreadHandle Handle)
{
	TSharedPtr<IThreadProxy> ThreadProxy = *this >> Handle;

	if (ThreadProxy.IsValid())
	{
		ThreadProxy->BlockingAndCompletion();
		return true;
	}

	return false;
}

bool FThreadProxyManage::Detach(FThreadHandle Handle)
{
	TSharedPtr<IThreadProxy> ThreadProxy = *this >> Handle;

	if (ThreadProxy.IsValid())
	{
		ThreadProxy->WakeupThread();
		return true;
	}

	return false;
}

EThreadState FThreadProxyManage::Joinable(FThreadHandle Handle)
{
	TSharedPtr<IThreadProxy> ThreadProxy = *this >> Handle;

	if (ThreadProxy.IsValid())
	{
		if (ThreadProxy->IsSuspend())
		{
			return EThreadState::THREAD_LEISURELY;
		}
		else
		{
			return EThreadState::THREAD_WORKING;
		}
	}

	return EThreadState::THREAD_ERROR;
}

#ifdef PLATFORM_PROJECT
FResourceLoadingManage::FResourceLoadingManage()
	:Super()
	, StreamableManager(new FStreamableManager())
{

}

FResourceLoadingManage::~FResourceLoadingManage()
{
	delete StreamableManager;
}

void FResourceLoadingManage::SetObjectPath(const TArray<FSoftObjectPath> &InObjectPath)
{
	SoftObjectPath = InObjectPath;
}

TArray<FSoftObjectPath> & FResourceLoadingManage::GetObjectPath()
{
	return SoftObjectPath;
}

FStreamableManager * FResourceLoadingManage::GetStreamableManager()
{
	return StreamableManager;
}
#endif
void FCoroutinesManage::Tick(float DeltaTime)
{
	*this <<= DeltaTime;
}

void FThreadGraphManage::Wait(const FGraphEventRef &EventRef)
{
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);
}

void FThreadGraphManage::Wait(const FGraphEventArray &EventRef)
{
	FTaskGraphInterface::Get().WaitUntilTasksComplete(EventRef);
}