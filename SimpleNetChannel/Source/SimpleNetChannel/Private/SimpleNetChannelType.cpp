// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "SimpleNetChannelType.h"
#include "SocketSubsystem.h"
#include "Connetion/Core/SimpleConnetion.h"

FSimpleConfigInfo::FSimpleConfigInfo()
	: Version(TEXT("1.0.1"))
	, URL(TEXT("127.0.0.1"))
	, Port(11223)
	, RecvDataNumber(10240)
	, SendDataNumber(1024)
	, MaxConnections(2000)
	, MaxChannels(5)
	, RepackagingFrequency(2000)
	, bAsynchronous(false)
	, bPrintHeartBeat(false)
	, bSlidingWindow(true)
	, bRepackaging(true)
	, bShowCompletePackProtocolInfo(false)
	, bShowSendDebug(false)
	, RepackagingTime(3.f)
	, OutTimeLink(360)
	, HeartBeatTimeTnterval(30.f)
	, PublicIP(TEXT("127.0.0.1"))
{
}

FSimplePackageHead::FSimplePackageHead()
	:Protocol(0)
	,PackageIndex(0)
	,PackageID(FGuid::NewGuid())
	,PackageSize(0)
	,bAck(false)
	,bForceSend(false)
{

}

FSimpleBunchHead::FSimpleBunchHead()
	:ProtocolsNumber(0)
	, ParamNum(0)
{

}

FSimpleAddrInfo::FSimpleAddrInfo()
{
}

FSimpleAddr::FSimpleAddr()
	:IP(0)
	, Port(0)
{
}

FSimpleAddr::FSimpleAddr(const FString& InIP, uint32 InPort)
	: IP(0)
	, Port(0)
{
	if (ISocketSubsystem* SocketSubsystem = FSimpleConnetion::GetSocketSubsystem())
	{
		TSharedRef<FInternetAddr> TmpAddr = SocketSubsystem->CreateInternetAddr();
		bool bIsValid = false;
		TmpAddr->SetIp(*InIP, bIsValid);
		TmpAddr->GetIp(IP);
		Port = InPort;
	}
}
