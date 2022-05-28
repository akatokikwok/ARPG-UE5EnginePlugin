// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

//Encapsulate the UE4 event, in order to obtain the suspend status better 
class FEvent;
struct FSimpleSemaphore
{
	FSimpleSemaphore();
	~FSimpleSemaphore();

	//Suspend
	void Wait();

	//Pending time 
	void Wait(uint32 WaitTime, const bool bIgnoreThreadIdleStats = false);

	//awaken 
	void Trigger();

	FORCEINLINE bool IsWait() { return bWait; }
private:
	FEvent *Event;
	uint8 bWait : 1;
};