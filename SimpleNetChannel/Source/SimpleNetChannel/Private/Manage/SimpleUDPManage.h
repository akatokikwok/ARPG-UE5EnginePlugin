// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once

#include "SimpleNetManage.h"

class FSocket;
class FSimpleUDPManage :public FSimpleNetManage
{
	typedef FSimpleNetManage Super;
public:	
	FSimpleUDPManage(ESimpleNetLinkState InType);

	virtual bool Init(int32 InPort = 0) override;
	virtual bool Init(uint32 InIP, uint32 InPort) override;
	virtual bool Init(const FString& InIP, uint32 InPort) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Close()override;
	virtual void Close(const FSimpleAddrInfo& InCloseConnetion)override;
	void ResetLocalConnetion();

	//Asynchronous
	void Run();
protected:
	void Listen();

	bool IsCompletePackage(int32 InRecvNum,
		uint8* InData,
		TSharedPtr<FSimpleConnetion> InConnetion,
		FGuid& OutGUID,
		uint8*& OutData,
		int32& OutLen);

	virtual bool CloseSocket();

protected:
	FSocket* Socket;

	bool bEndThread;

	struct FCache
	{
		FCache();

		uint32 TotalSize;
		TArray<uint8> Cache;

		void Reset();
	};

	TMap<FGuid,FCache> Caches;
};