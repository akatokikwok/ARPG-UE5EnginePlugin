// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleNetObject.h"
#include "SimpleController.generated.h"

UCLASS(Blueprintable)
class SIMPLENETCHANNEL_API USimpleController :public USimpleNetworkObject
{
	GENERATED_BODY()

public:

protected:
	virtual void Init();
	virtual void Tick(float DeltaTime);

	virtual void RecvProtocol(uint32 InProtocol);

	//Debug
protected:
	bool DebugServerPrintString(const FString& InData);
};