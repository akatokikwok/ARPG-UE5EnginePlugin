// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace SimpleUnrealPakEditor
{
	struct FPak
	{
		bool PakFile(const TArray<FString>& InPaths);
		bool PakMapFile(const TArray<FString>& InPaths);
		void ListPak(const FString& InPakPath);
	};
}