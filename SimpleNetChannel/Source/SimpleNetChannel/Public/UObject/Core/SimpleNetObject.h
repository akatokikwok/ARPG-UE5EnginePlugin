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
	//接受代理 主要为更上层逻辑调用
	FSimpleNetRecvDelegate RecvDelegate;
	//该对象对应的网络通过
	FSimpleChannel* GetChannel();
	//该对象对应的网络链接
	TSharedPtr<FSimpleConnetion > GetConnetion();

	//主要网络驱动管理
	FSimpleNetManage* GetManage();

	UE_DEPRECATED(4.25, "This method has been discarded,Please use GetRemoteAddrInfo(...) on the network object instead.")
	bool GetAddrInfo(FSimpleAddrInfo &InAddrInfo);

	UE_DEPRECATED(4.25, "This method has been discarded,Please use GetLocalAddrInfo(...) on the net network object instead.")
	bool GetServerAddrInfo(FSimpleAddrInfo& InAddrInfo);
	
	//获取自己链接的远端地址
	bool GetRemoteAddrInfo(FSimpleAddrInfo& InAddrInfo);
	//获取本地的主链接地址
	bool GetLocalAddrInfo(FSimpleAddrInfo& InAddrInfo);
	//把自己干掉
	void DestroySelf();
public:
	/*一个链接是否为主要链接对应的对象
	/*如果项目中出现多个主链接，请检查是不是创建了多个网络管理的实例都链接到相同对象上面导致
	/*有可能你的链接管理是其他网络管理对象，比如中心服务器，还有一个数据库客户端，数据库客户端里面的
	/*主链接也是true 本地的中心服务器也是true
	*/
	bool IsMainConnetion();

	//是客户端链接还是服务端链接
	FORCEINLINE ESimpleNetLinkState GetLinkState() { return LinkState; }
protected:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();

	virtual void RecvProtocol(uint32 InProtocol);

protected:
	FSimpleChannel* Channel;
};
