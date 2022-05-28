// Copyright // Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

//Mainly used as guid 
struct SIMPLETHREAD_API FSimpleThreadHandle : public TSharedFromThis<FSimpleThreadHandle>
{
	FSimpleThreadHandle();

	bool operator==(const FSimpleThreadHandle& SimpleThreadHandle)
	{
		return
			this->GUIDTypeA == SimpleThreadHandle.GUIDTypeA &&
			this->GUIDTypeB == SimpleThreadHandle.GUIDTypeB &&
			this->GUIDTypeC == SimpleThreadHandle.GUIDTypeC &&
			this->GUIDTypeD == SimpleThreadHandle.GUIDTypeD;
	}

protected:
	uint64 GUIDTypeA;
	uint64 GUIDTypeB;
	uint64 GUIDTypeC;
	uint64 GUIDTypeD;
};

//Thread state 
enum class EThreadState :uint8
{
	THREAD_LEISURELY,	
	THREAD_WORKING,	
	THREAD_ERROR,		
};

typedef TWeakPtr<FSimpleThreadHandle> FThreadHandle;
typedef TFunction<void()> FThreadLambda;

