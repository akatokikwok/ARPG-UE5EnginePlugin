// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once

#include "UObject/StrongObjectPtr.h"
#include "../SimpleNetChannelType.h"
DECLARE_DELEGATE_RetVal(UClass*, FSimpleReturnDelegate);

class USimpleNetworkObject;
class FSimpleConnetion;
class SIMPLENETCHANNEL_API FSimpleChannel
{
public:
	static FSimpleReturnDelegate SimpleControllerDelegate;
	static FSimpleReturnDelegate SimplePlayerDelegate;

	FSimpleChannel();
	virtual ~FSimpleChannel() {}

	virtual void Init();

	virtual void Tick(float DeltaSeconds);
	virtual void Close();
	virtual void PreClose();

	virtual void RecvProtocol(uint32 InProtocolsNumber);

	void Send(TArray<uint8>& InData,bool bForceSend = false);

	bool Receive(TArray<uint8>& InData);

	void SetConnetion(TWeakPtr<FSimpleConnetion> InConnetion);

	TSharedPtr<FSimpleConnetion >GetConnetion();

	void AddMsg(TArray<uint8> &InData);

	void InitController();
	void SpawnController();
	void SpawnPlayer();

	USimpleNetworkObject* GetNetObject();

	template<class T>
	T* GetNetObject()
	{
		return Cast<T>(Object.Get());
	}
	
	bool GetAddrInfo(FSimpleAddrInfo& InAddrInfo);
	void AcceptSuccess(const FGuid &InDataID,uint32 InIndex,bool bAck,bool bAllSuccessful = false);
	void SendBatch(const FGuid& InDataID, uint32 InIndex,uint32 InProtocol);
protected:
	USimpleNetworkObject* SpawnObject(UClass *InClass);
	void RegisterObject(FSimpleReturnDelegate InDelegate,UClass *InObjectClass);
	void RegisterObject(UClass *InClass, UClass* InObjectClass);
public:
	bool IsValid()const;
	const FGuid &GetGuid() const;
	void SetGuid(const FGuid &NewGuid);
	void DestroySelf();
protected:
	struct FBatch
	{
		FBatch()
			:bAck(false)
		{}
		struct FElement
		{
			FElement()
				:bStartUpRepackaging(false)
				,bAck(false)
				,RepeatCount(0)
				,CurrentTime(0.f)
			{}

			bool bStartUpRepackaging;//
			bool bAck;
			uint8 RepeatCount;//
			float CurrentTime;
		
			TArray<uint8> Package;
		};

		bool bAck;
		TMap<int32,FElement> Sequence;
	};
	TMap<FGuid,FBatch> Batchs;

protected:
	TWeakPtr<FSimpleConnetion> ConnetionPtr;
	TArray<TArray<uint8>> MsgQueue;
	TStrongObjectPtr<USimpleNetworkObject> Object;
	FGuid ID;
};