// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleThreadType.h"

//Request for process 
struct SIMPLETHREAD_API FCoroutinesRequest
{
	FCoroutinesRequest(float InIntervalTime);

	//Complete request or not 
	bool bCompleteRequest;

	//Time interval of each frame 
	float IntervalTime;
};

//Program interface object 
class SIMPLETHREAD_API ICoroutinesObject :public TSharedFromThis<ICoroutinesObject>
{
	friend class ICoroutinesContainer;
public:
	ICoroutinesObject();
	virtual ~ICoroutinesObject(){}

	bool operator==(const ICoroutinesObject& SimpleThreadHandle)
	{
		return this->Handle == SimpleThreadHandle.Handle;
	}

	//Wakeup process 
	FORCEINLINE void Awaken() { bAwaken = true; }
protected:

	virtual void Update(FCoroutinesRequest &CoroutinesRequest) = 0;

protected:

	//Process object container, which stores process objects 
	static TArray<TSharedPtr<ICoroutinesObject>> Array;
	uint8 bAwaken : 1;
	FSimpleThreadHandle Handle;
};

//Process handle 
typedef TWeakPtr<ICoroutinesObject> FCoroutinesHandle;

//Process object 
class SIMPLETHREAD_API FCoroutinesObject :public ICoroutinesObject
{
public:
	FCoroutinesObject(const FSimpleDelegate &InSimpleDelegate);
	FCoroutinesObject(float InTotalTime,const FSimpleDelegate &InSimpleDelegate);

	//Once registered, every frame will be updated 
	virtual void Update(FCoroutinesRequest &CoroutinesRequest) final;
private:

	//Agent required for registration 
	FSimpleDelegate SimpleDelegate;

	//Total waiting time 
	const float TotalTime;

	//Current time, mainly used to record time 
	float RuningTime;
};