// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleNetChannelType.generated.h"

UENUM(BlueprintType)
enum class ESimpleNetLinkState :uint8
{
	LINKSTATE_LISTEN,
	LINKSTATE_CONNET,
};

UENUM(BlueprintType)
enum class ESimpleConnetionType :uint8
{
	CONNETION_MAIN_LISTEN,
	CONNETION_LISTEN,
};

UENUM(BlueprintType)
enum class ESimpleSocketType :uint8
{
	SIMPLESOCKETTYPE_UDP,
	SIMPLESOCKETTYPE_TCP,
};

UENUM(BlueprintType)
enum class ESimpleConnetionLinkType :uint8
{
	LINK_UNINITIALIZED,//
	LINK_VERIFICATION, //
	LINK_LOGIN, //
	LINK_JOIN, //
};

struct SIMPLENETCHANNEL_API FSimplePackageHead
{
	FSimplePackageHead();

	uint32 Protocol;//
	uint32 PackageIndex;
	FGuid PackageID;
	FGuid ChannelID;
	uint32 PackageSize;
	bool bAck;
	bool bForceSend;
};

struct SIMPLENETCHANNEL_API FSimpleBunchHead
{
	FSimpleBunchHead();

	uint32 ProtocolsNumber;
	FGuid ChannelID;
	uint8 ParamNum;
};

struct SIMPLENETCHANNEL_API FSimpleAddr
{
	FSimpleAddr();
	FSimpleAddr(const FString& InIP, uint32 InPort);

	uint32 IP;
	uint32 Port;
};

struct SIMPLENETCHANNEL_API FSimpleAddrInfo
{
	FSimpleAddrInfo();

	FSimpleAddr Addr;
	FGuid ChannelID;

	bool operator==(const FSimpleAddrInfo &InInfo)
	{
		return	ChannelID == InInfo.ChannelID	&&
				Addr.IP == InInfo.Addr.IP		&&
				Addr.Port == InInfo.Addr.Port;
	}
};

USTRUCT(BlueprintType)
struct FSimpleConfigInfo
{
	GENERATED_BODY()

	FSimpleConfigInfo();

	UPROPERTY(Config)
	FString Version;

	UPROPERTY(Config)
	FString URL;

	UPROPERTY(Config)
	int32 Port;

	UPROPERTY(Config)
	int32 RecvDataNumber;

	UPROPERTY(Config)
	int32 SendDataNumber;

	UPROPERTY(Config)
	int32 MaxConnections;

	UPROPERTY(Config)
	int32 MaxChannels;

	UPROPERTY(Config)
	int32 RepackagingFrequency;

	UPROPERTY(Config)
	bool bAsynchronous;

	UPROPERTY(Config)
	bool bPrintHeartBeat;

	UPROPERTY(Config)
	bool bSlidingWindow;

	UPROPERTY(Config)
	bool bRepackaging;

	UPROPERTY(Config)
	bool bShowCompletePackProtocolInfo;

	UPROPERTY(Config)
	bool bShowSendDebug;

	UPROPERTY(Config)
	float RepackagingTime;

	UPROPERTY(Config)
	float OutTimeLink;

	UPROPERTY(Config)
	float HeartBeatTimeTnterval;

	UPROPERTY(Config)
	FString PublicIP;
	 
	UPROPERTY(Config)
	FString SecretKey;
};

UENUM(BlueprintType)
enum ESimpleNetErrorType
{
	NET_NONE,
	TIME_OUT,
	INIT_FAIL,
	INIT_SUCCESS,
	HAND_SHAKE_SUCCESS,
	HAND_SHAKE_FAIL,
	INVALID_VALIDATION,
};