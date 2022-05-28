// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreTypes.h"

//Windows native thread, this thread scheme can be used in special cases 
struct SIMPLETHREAD_API FWindowsPlatformThread
{
	static void Show();

	static void Hide();

	static bool IsShown();

	static FSimpleDelegate CompletedDelegate;

	static FSimpleDelegate RunDelegate;
};
