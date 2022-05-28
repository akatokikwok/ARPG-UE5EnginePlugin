// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleNetObject.h"
#include "SimplePlayer.generated.h"

UCLASS(Blueprintable)
class SIMPLENETCHANNEL_API USimplePlayer :public USimpleNetworkObject
{
	GENERATED_BODY()

public:

protected:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();

	virtual void RecvProtocol(uint32 InProtocol);
};