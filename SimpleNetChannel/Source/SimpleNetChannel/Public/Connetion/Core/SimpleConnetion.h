// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once

#include "SimpleNetChannelType.h"

class FSocket;
class FInternetAddr;
class FSimpleChannel;
class ISocketSubsystem;
class FSimpleNetManage;

class FSimpleConnetion :public TSharedFromThis<FSimpleConnetion>
{
protected:
	typedef FSimpleConnetion Super;
public:
	virtual ~FSimpleConnetion(){}
	FSimpleConnetion();

	virtual void Init();
	virtual void Tick(float DeltaSeconds);
	virtual void CheckLoginTimeout(float DeltaSeconds);
	virtual void Close();

	static ISocketSubsystem* GetSocketSubsystem();
	TSharedRef<FInternetAddr> GetAddr();

	void SendHeartBeat();
	void StartSendHeartBeat();
	virtual void Send(TArray<uint8>& InData);
	virtual void Receive(const FGuid &InChannelID,TArray<uint8> &InData);

	virtual void Analysis(uint8* InData, int32 BytesNumber);

	void SetState(ESimpleConnetionLinkType InState);

	void CheckTimeOut();

	virtual void ConnectVerification();
	void SetSocket(FSocket* InSocket);

	void ResetHeartBeat();

	FSimpleChannel* GetMainChannel();
	FSimpleChannel* GetChannel(const FGuid &InChannelGuid);

	void SetLinkState(ESimpleNetLinkState NewLinkState);
	void SetConnetionType(ESimpleConnetionType InType);
	void SetManage(FSimpleNetManage *InManage);

	void GetChannelActiveID(TArray<FGuid>& InIDs);
public:
	void Lock();
	bool IsLock()const { return bLock; }
	void UnLock();

public:
	FORCEINLINE FSimpleNetManage* GetManage() { return Manage; }
	FORCEINLINE ESimpleNetLinkState GetLinkState()const { return LinkState; }
	FORCEINLINE ESimpleConnetionType GetConnetionType()const { return ConnetionType; }
	FORCEINLINE ESimpleConnetionLinkType GetState() { return State; }
	FORCEINLINE	TArray<FSimpleChannel>& GetChannels() { return Channels; }
protected:
	TArray<FSimpleChannel> Channels;//
	ESimpleConnetionLinkType State;//初始化阶段
	ESimpleNetLinkState LinkState;//Connet的状态
	ESimpleConnetionType ConnetionType;

	FSocket* Socket;//TCP UDP
	TSharedPtr<FInternetAddr> LocalAddr;
	uint8 bLock : 1;
	uint8 bHeartBeat : 1;
	double HeartTime;
	double LastTime;

	double RequiredReconnectionTime;
	double TimeoutLink;

protected:
	FSimpleNetManage* Manage;
};