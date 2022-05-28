// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "Connetion/Core/SimpleConnetion.h"
#include "SocketSubsystem.h"
#include "Channel/SimpleChannel.h"
#include "Protocols/SimpleNetProtocols.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "Log/SimpleNetChannelLog.h"
#include "SimpleNetManage.h"
#include "Core/EncryptionAndDecryption/SimpleEncryptionAndDecryption.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

FSimpleConnetion::FSimpleConnetion()
	:State(ESimpleConnetionLinkType::LINK_UNINITIALIZED)
	, Socket(NULL)
	, bLock(false)
	, HeartTime(0.0)
	, LastTime(0.0)
	, RequiredReconnectionTime(0.0)
	, TimeoutLink(0.0)
	, Manage(nullptr)
{
	ConnetionType = ESimpleConnetionType::CONNETION_LISTEN;

	Channels.SetNum(FSimpleNetGlobalInfo::Get()->GetInfo().MaxChannels);//Pre allocation strategy

	if (ISocketSubsystem* SocketSubsystem = GetSocketSubsystem())
	{
		LocalAddr = SocketSubsystem->CreateInternetAddr();
	}	
}

void FSimpleConnetion::SetConnetionType(ESimpleConnetionType InType)
{
	ConnetionType = InType;
}

void FSimpleConnetion::SetManage(FSimpleNetManage* InManage)
{
	Manage = InManage;
}

void FSimpleConnetion::GetChannelActiveID(TArray<FGuid>& InIDs)
{
	for(int32 i = 0;i < Channels.Num();i++)
	{
		if (Channels[i].IsValid())
		{
			InIDs.Add(Channels[i].GetGuid());
		}
	}
}

void FSimpleConnetion::Init()
{
	for (auto &Tmp : Channels)
	{
		Tmp.SetConnetion(this->AsShared());
	}

	//Register the main channel first
	if (FSimpleChannel *MainChannel = GetMainChannel())
	{
		MainChannel->SpawnController();
		MainChannel->Init();
	}
}

void FSimpleConnetion::Close()
{
	//Pre closing
	for (auto& Tmp : Channels)
	{
		Tmp.PreClose();
	}

	//Send broken link
	if (FSimpleChannel* Channel = GetMainChannel())
	{
		//Client request disconnect
		SIMPLE_PROTOCOLS_SEND(SP_Close)
	}

	UE_LOG(LogSimpleNetChannel,
		Display,
		TEXT("[Close] Connetion Close.Socket :[IP:%s Port:%d]"),
		*GetAddr()->ToString(false),
		GetAddr()->GetPort());

	HeartTime = 0.0;
	bHeartBeat = false;

	//Turn off channels first
	for (auto& Tmp : Channels)
	{
		Tmp.Close();
	}

	State = ESimpleConnetionLinkType::LINK_UNINITIALIZED;

	//Release the last one
	if (ISocketSubsystem* SocketSubsystem = GetSocketSubsystem())
	{
		LocalAddr = SocketSubsystem->CreateInternetAddr();
	}
}

void FSimpleConnetion::Tick(float DeltaSeconds)
{
	for (auto &Tmp : Channels)
	{
		if (Tmp.IsValid())
		{
			Tmp.Tick(DeltaSeconds);
		}
	}

	//Time overflow
	if (LinkState == ESimpleNetLinkState::LINKSTATE_LISTEN)
	{
		if (ConnetionType == ESimpleConnetionType::CONNETION_LISTEN) //Non main channel
		{
			CheckTimeOut();
		}
	}
	else
	{
		if (State == ESimpleConnetionLinkType::LINK_JOIN)
		{
			if (bHeartBeat)
			{
				HeartTime += DeltaSeconds;
				if (HeartTime >= FSimpleNetGlobalInfo::Get()->GetInfo().HeartBeatTimeTnterval)
				{
					HeartTime = 0.0;

					SendHeartBeat();
				}
			}
		}
	}
}

void FSimpleConnetion::CheckLoginTimeout(float DeltaSeconds)
{
	if (LinkState == ESimpleNetLinkState::LINKSTATE_CONNET)
	{
		if (State != ESimpleConnetionLinkType::LINK_JOIN && //Is the connection successful
			State != ESimpleConnetionLinkType::LINK_UNINITIALIZED)//It's an empty one
		{
			RequiredReconnectionTime += DeltaSeconds;
			if (RequiredReconnectionTime >= FSimpleNetGlobalInfo::Get()->GetInfo().HeartBeatTimeTnterval)
			{
				RequiredReconnectionTime = 0.0;

				ConnectVerification();

				UE_LOG(LogSimpleNetChannel,
					Display,
					TEXT("Client trying to link [Hello] to the server[IP:%s Port:%i]."),
					*LocalAddr->ToString(false),
					LocalAddr->GetPort());
			}

			TimeoutLink += DeltaSeconds;
			if (TimeoutLink >= FSimpleNetGlobalInfo::Get()->GetInfo().OutTimeLink)
			{
				TimeoutLink = 0.0;

				FString ErrorString = FString::Printf(TEXT("The server is not responding,Maybe the link address is wrong or the server is wrong [IP:%s Port:%i]"),
					*LocalAddr->ToString(false),
					LocalAddr->GetPort());

				UE_LOG(LogSimpleNetChannel, Error, TEXT("%s"), *ErrorString);

				//Callback
				if (Manage)
				{
					Manage->LinkTimeout(ErrorString);
				}
			}
		}
		else
		{
			RequiredReconnectionTime = 0.0;
			TimeoutLink = 0.0;
		}
	}

}

ISocketSubsystem* FSimpleConnetion::GetSocketSubsystem()
{
	return ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
}

TSharedRef<FInternetAddr> FSimpleConnetion::GetAddr()
{
	return LocalAddr.ToSharedRef();
}

void FSimpleConnetion::StartSendHeartBeat()
{
	bHeartBeat = true;
}

void FSimpleConnetion::SendHeartBeat()
{
	if (FSimpleChannel* Channel = GetMainChannel())
	{
		if (Channel->IsValid())
		{
			SIMPLE_PROTOCOLS_SEND(SP_HeartBeat)

			if (FSimpleNetGlobalInfo::Get()->GetInfo().bPrintHeartBeat)
			{
				UE_LOG(LogSimpleNetChannel,
					Display,
					TEXT("[HeartBeat] Send heartbeat. Server [IP:%s Port:%d]"),
					*GetAddr()->ToString(false),
					GetAddr()->GetPort());
			}
		}
	}
}

void FSimpleConnetion::CheckTimeOut()
{
	double CurrentTime = FPlatformTime::Seconds();
	if (CurrentTime - LastTime > FSimpleNetGlobalInfo::Get()->GetInfo().OutTimeLink)
	{
		Close();
	}
}

void FSimpleConnetion::Send(TArray<uint8>& InData)
{
	SimpleEncryptionAndDecryption::Encryption(InData);
}

void FSimpleConnetion::Receive(const FGuid& InChannelID,TArray<uint8>& InData)
{
	for (auto &Tmp : Channels)
	{
		if (Tmp.IsValid())
		{
			if (Tmp.GetGuid() == InChannelID)
			{
				Tmp.AddMsg(InData);
				break;
			}
		}
	}
}

void FSimpleConnetion::ResetHeartBeat()
{
	LastTime = FPlatformTime::Seconds();
}

void FSimpleConnetion::Analysis(uint8* InData, int32 BytesNumber)//Location of system resolution
{
	FSimpleBunchHead Head = *(FSimpleBunchHead*)InData;
	if (BytesNumber >= sizeof(FSimpleBunchHead))
	{
		//Update our upper business
		auto UpdateObject = [&]()
		{
			if (FSimpleChannel* Channel = GetChannel(Head.ChannelID))
			{
				if (Head.ParamNum > 0)
				{
					TArray<uint8> InNewData(InData, BytesNumber);
					Channel->AddMsg(InNewData);
				}

				//Transfer protocol and data to upper layer
				Channel->RecvProtocol(Head.ProtocolsNumber);
			}
		};
		if (GetLinkState() == ESimpleNetLinkState::LINKSTATE_LISTEN)
		{
			switch (Head.ProtocolsNumber)
			{
				case SP_HeartBeat:
				{
					ResetHeartBeat();

					if (FSimpleNetGlobalInfo::Get()->GetInfo().bPrintHeartBeat)
					{
						UE_LOG(LogSimpleNetChannel,
							Display,
							TEXT("[HeartBeat] Recv heartbeat. Client [IP:%s Port:%d]"),
							*GetAddr()->ToString(false),
							GetAddr()->GetPort());
					}

					break;
				}
				case SP_Close:
				{
					Close();
					break;
				}
				default:
				{
					UpdateObject();
					break;
				}
			}
		}
		else if (GetLinkState() == ESimpleNetLinkState::LINKSTATE_CONNET)
		{
			switch (Head.ProtocolsNumber)
			{
				case SP_HeartBeat:
				{

					break;
				}
				default:
				{
					//UE_LOG(LogSimpleNetChannel, Log, TEXT("recv %s byte"), BytesNumber);
					UpdateObject();
					break;
				}
			}
		}
	}
}

void FSimpleConnetion::SetState(ESimpleConnetionLinkType InState)
{
	State = InState;
}

void FSimpleConnetion::ConnectVerification()
{
	if (FSimpleChannel *Channel = GetMainChannel())
	{
		FString LocalVersion = FSimpleNetGlobalInfo::Get()->GetInfo().Version;
		SIMPLE_PROTOCOLS_SEND(SP_Hello,LocalVersion);
		
		SetState(ESimpleConnetionLinkType::LINK_VERIFICATION);
		
		UE_LOG(LogSimpleNetChannel,
			Display,
			TEXT("The client sends [Hello] to the server[IP:%s Port:%i]."),
			*LocalAddr->ToString(false), 
			LocalAddr->GetPort());
	}
}

void FSimpleConnetion::SetSocket(FSocket* InSocket)
{
	Socket = InSocket;
}

FSimpleChannel* FSimpleConnetion::GetMainChannel()
{
	if (Channels.IsValidIndex(0))
	{
		return &Channels[0];
	}

	return NULL;
}

FSimpleChannel* FSimpleConnetion::GetChannel(const FGuid& InChannelGuid)
{
	for (auto &Tmp : Channels)
	{
		if (Tmp.GetGuid() == InChannelGuid)
		{
			return &Tmp;
		}
	}

	return NULL;
}

void FSimpleConnetion::SetLinkState(ESimpleNetLinkState NewLinkState)
{
	LinkState = NewLinkState;
}

void FSimpleConnetion::Lock()
{
	bLock = true;
}

void FSimpleConnetion::UnLock()
{
	bLock = false;
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif