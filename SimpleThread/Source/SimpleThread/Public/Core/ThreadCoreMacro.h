// Copyri// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once
#include "Misc/ScopeLock.h"
#include "Abandonable/SimpleAbandonable.h"
#include "Async/TaskGraphInterfaces.h"

#define DDD(...) __VA_ARGS__

DEFINE_LOG_CATEGORY_STATIC(SimpleThreadLOG, Log, All);

#define MUTEX_LOCL FScopeLock ScopeLock(&Mutex) 

#define USE_UE_THREAD_POOL_ASYNCTASK(ThreadDelegate) \
(new FAutoDeleteAsyncTask<FSimpleAbandonable>(ThreadDelegate))->StartBackgroundTask()

#define ASYNCTASK_UOBJECT(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define ASYNCTASK_Raw(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define ASYNCTASK_SP(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define ASYNCTASK_Lambda(...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define ASYNCTASK_UFunction(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))

#define USE_UE_THREAD_POOL_SYNCTASK(ThreadDelegate) \
{FAsyncTask<FSimpleAbandonable> *SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(ThreadDelegate); \
SimpleAbandonable->StartBackgroundTask(); \
SimpleAbandonable->EnsureCompletion(); \
delete SimpleAbandonable; }

#define SYNCTASK_UOBJECT(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define SYNCTASK_Raw(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define SYNCTASK_SP(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define SYNCTASK_Lambda(...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define SYNCTASK_UFunction(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))

//The calling thread can set the pre task 
#define CALL_THREAD(OtherTask,CallThreadName,InTaskDeletegate) \
FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(InTaskDeletegate,TStatId(),OtherTask,CallThreadName);

#define CALL_THREAD_UOBJECT(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define CALL_THREAD_Raw(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define CALL_THREAD_SP(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define CALL_THREAD_Lambda(OtherTask,CallThreadName,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define CALL_THREAD_UFunction(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))


#define WAITING_OTHER_THREADS_COMPLETED(EventRef) \
FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);

#define WAITING_OTHER_THREADS_COMPLETED_MULTI(EventRef) \
FTaskGraphInterface::Get().WaitUntilTasksComplete(EventRef);

#define SIMPLE_THREAD_INFO_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(SimpleThreadLOG, Log, TEXT("%s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define SIMPLE_THREAD_INFO_MSG_WARNING(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(SimpleThreadLOG, Error, TEXT("%s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define SIMPLE_THREAD_INFO_MSG_ERROR(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(SimpleThreadLOG, Error, TEXT("%s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}