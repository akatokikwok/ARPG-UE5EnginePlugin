// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../../SimpleNetChannelType.h"
#include "../../Channel/SimpleChannel.h"
#include "../../Connetion/Core/SimpleConnetion.h"
#include "SimpleNetManage.h"
#include "SimpleNetObject.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FSimpleNetRecvDelegate,uint32, FSimpleChannel*)

UCLASS(Blueprintable)
class SIMPLENETCHANNEL_API USimpleNetworkObject :public UObject
{
	GENERATED_BODY()

	friend FSimpleChannel;

	UPROPERTY()
	ESimpleNetLinkState LinkState;
public:
	FSimpleNetRecvDelegate RecvDelegate;

	FORCEINLINE ESimpleNetLinkState GetLinkState() { return LinkState; }
	FSimpleChannel* GetChannel();
	TSharedPtr<FSimpleConnetion > GetConnetion();
	FSimpleNetManage* GetManage();

	UE_DEPRECATED(4.25, "This method has been discarded,Please use GetRemoteAddrInfo(...) on the network object instead.")
	bool GetAddrInfo(FSimpleAddrInfo &InAddrInfo);

	UE_DEPRECATED(4.25, "This method has been discarded,Please use GetLocalAddrInfo(...) on the net network object instead.")
	bool GetServerAddrInfo(FSimpleAddrInfo& InAddrInfo);

	bool GetRemoteAddrInfo(FSimpleAddrInfo& InAddrInfo);
	bool GetLocalAddrInfo(FSimpleAddrInfo& InAddrInfo);

	void DestroySelf();
protected:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();

	virtual void RecvProtocol(uint32 InProtocol);

protected:
	FSimpleChannel* Channel;
};
