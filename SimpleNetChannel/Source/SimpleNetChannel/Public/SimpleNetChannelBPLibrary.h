// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleNetChannelBPLibrary.generated.h"


UCLASS()
class USimpleNetChannelBPLibrary : public UObject
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "SimpleNetChannel sample test testing"), Category = "SimpleNetChannelTesting")
	static float SimpleNetChannelSampleFunction(float Param);
};
