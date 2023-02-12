// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Interface/ProxyInterface.h"
#include "Containers/Queue.h"
#include "Core/ThreadCoreMacro.h"
#include "Abandonable/SimpleAbandonable.h"
#include "Coroutines/SimpleCoroutines.h"
#include "Async/TaskGraphInterfaces.h"
#include "Runnable/ThreadRunnableProxy.h"
#include "SimpleTreadPlatform.h"

#ifdef PLATFORM_PROJECT
#include "Engine/StreamableManager.h"
#endif

//Thread container interface 
class IThreadContainer
{
public:
	IThreadContainer() {}
	virtual ~IThreadContainer() {}

protected:
	FCriticalSection Mutex;
};

//Threads can be created freely without restriction. The created threads will not be destroyed immediately. If necessary, they can be used again,            
//It has synchronous and asynchronous functions and is generally used in small scenes 
class IThreadProxyContainer :public TArray<TSharedPtr<IThreadProxy>>, public IThreadContainer
{
protected:
	typedef TArray<TSharedPtr<IThreadProxy>>	TProxyArray;

public:
	IThreadProxyContainer &operator<<(const TSharedPtr<IThreadProxy> &ThreadProxy)
	{
		MUTEX_LOCL;

		ThreadProxy->CreateSafeThread();
		this->Add(ThreadProxy);

		return *this;
	}

	FThreadHandle operator>>(const FSimpleDelegate &ThreadProxy)
	{
		FThreadHandle ThreadHandle = nullptr;
		{
			MUTEX_LOCL;
			for (auto &Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					ThreadHandle = Tmp->GetThreadHandle();
					Tmp->WakeupThread();

					break;
				}
			}
		}

		if (!ThreadHandle.IsValid())
		{
			TSharedPtr<IThreadProxy> Proxy = MakeShareable(new FThreadRunnable(true));
			Proxy->GetThreadDelegate() = ThreadProxy;
			*this << Proxy;

			ThreadHandle = Proxy->GetThreadHandle();
		}

		return ThreadHandle;
	}

	FThreadHandle operator<<(const FSimpleDelegate &ThreadProxy)
	{
		FThreadHandle ThreadHandle = nullptr;
		{
			MUTEX_LOCL;
			for (auto &Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					ThreadHandle = Tmp->GetThreadHandle();

					break;
				}
			}
		}

		if (!ThreadHandle.IsValid())
		{
			ThreadHandle = *this << MakeShareable(new FThreadRunnable) << ThreadProxy;
		}

		return ThreadHandle;
	}

	TSharedPtr<IThreadProxy> operator>>(const FThreadHandle &Handle)
	{
		MUTEX_LOCL;
		for (auto &Tmp : *this)
		{
			if (Tmp->GetThreadHandle() == Handle)
			{
				return Tmp;
			}
		}

		return NULL;
	}
};

//Thread task management can automatically manage tasks, automatically allocate idle thread pool, and realize efficient utilization of thread pool characteristics 
class IThreadTaskContainer :public TQueue<FSimpleDelegate>, public TArray<TSharedPtr<IThreadProxy>>, public IThreadContainer
{
protected:
	typedef TArray<TSharedPtr<IThreadProxy>>	TProxyArray;
	typedef TQueue<FSimpleDelegate>				TEventQueue;

public:
	void operator<<(const FSimpleDelegate &ThreadProxy)
	{
		MUTEX_LOCL;

		this->Enqueue(ThreadProxy);
	}

	bool operator<<=(FSimpleDelegate &ThreadProxy)
	{
		MUTEX_LOCL;

		return this->Dequeue(ThreadProxy);
	}

	IThreadTaskContainer& operator<<(const TSharedPtr<IThreadProxy> &ThreadProxy)
	{
		MUTEX_LOCL;

		this->Add(ThreadProxy);
		ThreadProxy->CreateSafeThread();

		return *this;
	}

	void operator>>(const FSimpleDelegate &ThreadProxy)
	{
		bool bSuccessful = false;
		{
			MUTEX_LOCL;
			for (auto &Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					Tmp->WakeupThread();

					bSuccessful = true;
					break;
				}
			}
		}

		if (!bSuccessful)
		{
			*this << ThreadProxy;
		}
	}
};

//Synchronous asynchronous thread interface 
class IAbandonableContainer :public IThreadContainer
{
protected:
	//Synchronous binding 
	void operator<<(const FSimpleDelegate& ThreadDelegate)
	{
		FAsyncTask<FSimpleAbandonable> *SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(ThreadDelegate);
		SimpleAbandonable->StartBackgroundTask();
		SimpleAbandonable->EnsureCompletion();
		delete SimpleAbandonable;
	}

	//Asynchronous binding 
	void operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		(new FAutoDeleteAsyncTask<FSimpleAbandonable>(ThreadDelegate))->StartBackgroundTask();
	}
};

//Coroutine interface 
class ICoroutinesContainer 
{
public:
	ICoroutinesContainer()
	 :TmpTotalTime(0.f)
	{

	}
	virtual ~ICoroutinesContainer()
	{
		ICoroutinesObject::Array.Empty();
	}

	ICoroutinesContainer &operator<<(float TotalTime)
	{
		TmpTotalTime = TotalTime;

		return *this;
	}

	ICoroutinesContainer &operator<<(const FSimpleDelegate& ThreadDelegate)
	{
		ICoroutinesObject::Array.Add(MakeShareable(new FCoroutinesObject(TmpTotalTime, ThreadDelegate)));

		return *this;
	}

	void operator<<=(float Time)
	{
		TArray<TSharedPtr<ICoroutinesObject>> RemoveObject;
		for (int32 i = 0 ;i < ICoroutinesObject::Array.Num();i++)//改成i的形式 是为了安全考虑
		{
			if (auto Tmp = ICoroutinesObject::Array[i])
			{
				FCoroutinesRequest Request(Time);

				Tmp->Update(Request);
				if (Request.bCompleteRequest)
				{
					RemoveObject.Add(Tmp);
				}
			}
		}

		for (auto &RemoveInstance : RemoveObject)
		{
			ICoroutinesObject::Array.Remove(RemoveInstance);
		}
	}

	FCoroutinesHandle operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		ICoroutinesObject::Array.Add(MakeShareable(new FCoroutinesObject(ThreadDelegate)));

		return ICoroutinesObject::Array[ICoroutinesObject::Array.Num() - 1];
	}
private:
	float TmpTotalTime;
};

//Graph thread interface 
class IGraphContainer :public IThreadContainer
{
protected:

	//Call thread
	FGraphEventRef operator<<(const FSimpleDelegate& ThreadDelegate)
	{
		MUTEX_LOCL;
		return FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			ThreadDelegate,
			TStatId(),
			nullptr,
			ENamedThreads::GameThread);
	}

	//Call any thread
	FGraphEventRef operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		MUTEX_LOCL;
		return FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			ThreadDelegate,
			TStatId(),
			nullptr,
			ENamedThreads::AnyThread);
	}
};
#ifdef PLATFORM_PROJECT
//Resource management interface 
class IStreamableContainer
{
public:
	virtual ~IStreamableContainer() {}

	//Storage path 
	IStreamableContainer &operator>>(const TArray<FSoftObjectPath> &InObjectPath)
	{
		SetObjectPath(InObjectPath);

		return *this;
	}
	//Asynchronous resources 
	TSharedPtr<struct FStreamableHandle> operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		return GetStreamableManager()->RequestAsyncLoad(GetObjectPath(), ThreadDelegate);
	}

	//Synchronous read 
	TSharedPtr<struct FStreamableHandle> operator<<(const TArray<FSoftObjectPath> &InObjectPath)
	{
		return GetStreamableManager()->RequestSyncLoad(InObjectPath);
	}

protected:
	virtual void SetObjectPath(const TArray<FSoftObjectPath> &InObjectPath) = 0;
	virtual TArray<FSoftObjectPath> &GetObjectPath() = 0;

	virtual FStreamableManager* GetStreamableManager() = 0;

}; 
#endif