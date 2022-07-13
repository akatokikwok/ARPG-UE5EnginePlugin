// Copyright (C) RenZhai.2021.All Rights Reserved.


#pragma once

#include "SimpleNetChannelType.h"
#include "SimpleProtocolsDefinition.h"

#ifdef PLATFORM_PROJECT
#include "Tickable.h"
#endif

DECLARE_DELEGATE_TwoParams(FSimpleNetManageMsgDelegate,ESimpleNetErrorType,const FString&);

class USimpleController;
class FSimpleConnetion;
class FInternetAddr;
class USimpleNetworkObject;

class SIMPLENETCHANNEL_API FSimpleNetManage
#ifdef PLATFORM_PROJECT
	:public FTickableGameObject
#endif
{
public:
	FSimpleNetManageMsgDelegate NetManageMsgDelegate;

	UClass* NetworkObjectClass;
	UClass* SimplePlayerClass;
public:
	void LinkTimeout(const FString &InTimeoutMsg);
public:
	FSimpleNetManage();
	virtual ~FSimpleNetManage(){}

	static FSimpleNetManage *CreateManage(ESimpleNetLinkState InState, ESimpleSocketType InType);
	static void Destroy(FSimpleNetManage *InNetInstance);

	virtual bool Init(int32 InPort = 0);
	virtual bool Init(uint32 InIP,uint32 InPort);
	virtual bool Init(const FSimpleAddr& InAddr);
	virtual bool Init(const FString& InIP, uint32 InPort);

	virtual void Tick(float DeltaTime);
	virtual void Close();
	virtual void Close(const FSimpleAddrInfo &InCloseConnetion);
	virtual void Close(const TSharedPtr<FInternetAddr> &InternetAddr);

	USimpleController* GetController();
	USimpleNetworkObject* GetNetworkObject(uint32 InIP,uint32 InPort,const FGuid &InChannelID);

	FSimpleChannel* GetChannel();
	FSimpleChannel* GetChannel(uint32 InIP, uint32 InPort, const FGuid& InChannelID);

	int32 GetConnetionNum();
	FSimpleAddr GetAddr();

	static FString GetAddrString(const FSimpleAddr& InAddr);
	static FSimpleAddr GetSimpleAddr(const TCHAR* InIP,uint32 InPort);
	static void GetLocalIPAndPort(FString &InIP,uint32 &Port);

	//该API使用需要保证你当前的网络管理是服务器 它会遍历服务器的远端链接对象
	template<class T>
	void FindRemoteNetworkObjectByLambda(TFunction<ESimpleNetManageCallType(T*)> InImplement);

	//高效的自定义广播 前提是必须是服务器
	template<int32 InProtocols, class T, typename ...ParamTypes>
	void MulticastByPredicate(TFunction<bool(T*)> InImplement, ParamTypes &...Param);
protected:
	virtual bool CloseSocket();
public:
	static USimpleNetworkObject *GetNetManageNetworkObject(FSimpleNetManage* InSimpleNetManage, const FSimpleAddrInfo& AddrInfo);

public:
	FORCEINLINE ESimpleNetLinkState GetLinkState() { return LinkState; }

protected:
	virtual TStatId GetStatId()const;
	virtual void VerificatioConnetionInfo(TSharedPtr<FSimpleConnetion> InConnetion, uint8* InData, int32 InByteNumber);
protected:
	ESimpleNetLinkState LinkState;

	struct FNet
	{
		bool IsAddr(TSharedPtr<FInternetAddr> InternetAddr);
		int32 Add(TSharedPtr<FInternetAddr> InternetAddr);
		void Clear(int32 InIndex);
		TSharedPtr<FSimpleConnetion> GetEmptyConnetion(TSharedPtr<FInternetAddr> InternetAddr);
	public:
		TSharedPtr<FSimpleConnetion> operator[](TSharedPtr<FInternetAddr> InternetAddr);
		TSharedPtr<FSimpleConnetion> operator[](const FSimpleAddr &InternetAddr);
	public:
		TSharedPtr<FSimpleConnetion> LocalConnetion;
		TArray<TSharedPtr<FSimpleConnetion>> RemoteConnetions;
	}Net;

	bool bInit;
};


/*
* The following website explains how to use this set of plug-ins
* DocURL：
* Here's how to develop the current plug-in
* MarketplaceURL :     https://www.aboutcg.org/courseDetails/1086/introduce
* If you want to learn more about the UE4 tutorial, please refer to:
* URL :				   https://zhuanlan.zhihu.com/p/60117613
* If you want to learn about the series of tutorials and related plug-ins, you can follow my blog
* URL :				   http://renzhai.net/
* If you want to know our next course arrangement,
* you can search the WeChat public house for your attention, then we will push a lot of technical articles.
* Sina blog            https://weibo.com/bzrz/profile?S=6cm7d0 // this blog hasn't been used for three or four years now.
* Maybe you can re enable the first connection later
*/
#include "Channel/SimpleChannel.h"

class USimpleNetworkObject;
template<class T>
void FSimpleNetManage::FindRemoteNetworkObjectByLambda(TFunction<ESimpleNetManageCallType(T*)> InImplement)
{
	for (auto& Tmp : Net.RemoteConnetions)
	{
		ESimpleNetManageCallType SimpleNetManageCallType = ESimpleNetManageCallType::INPROGRESS;
		for (auto& TmpChannel : Tmp->GetChannels())
		{
			if (TmpChannel.IsValid())
			{
				SimpleNetManageCallType = InImplement(TmpChannel.GetNetObject<T>());
				if (SimpleNetManageCallType == ESimpleNetManageCallType::PROGRESS_COMPLETE)
				{
					break;
				}	
			}
		}

		if (SimpleNetManageCallType == ESimpleNetManageCallType::PROGRESS_COMPLETE)
		{
			break;
		}
	}
}

//广播指定的链接端
template<int32 InProtocols,class T, typename ...ParamTypes>
void FSimpleNetManage::MulticastByPredicate(TFunction<bool(T*)> InImplement,ParamTypes &...Param)
{
	for (auto& Tmp : Net.RemoteConnetions)
	{
		ESimpleNetManageCallType SimpleNetManageCallType = ESimpleNetManageCallType::INPROGRESS;
		for (auto& TmpChannel : Tmp->GetChannels())
		{
			if (TmpChannel.IsValid())
			{
				//是否满足可以广播的条件
				if (InImplement(TmpChannel.GetNetObject<T>()))
				{
					FSimpleProtocols<InProtocols>::Send(&TmpChannel,Param...);
				}
			}
		}
	}
}
