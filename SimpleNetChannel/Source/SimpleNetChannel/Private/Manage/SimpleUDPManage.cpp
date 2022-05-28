// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "SimpleUDPManage.h"
#include "../Connetion/SimpleUDPConnetion.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "../Log/SimpleNetChannelLog.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "Thread/SimpleNetThread.h"
#include "Protocols/SimpleNetProtocols.h"
#include "Core/EncryptionAndDecryption/SimpleEncryptionAndDecryption.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

FSimpleUDPManage::FSimpleUDPManage(ESimpleNetLinkState InType)
	:Super()
{
	Socket = NULL;

	bEndThread = false;

	LinkState = InType;
}

bool FSimpleUDPManage::Init(int32 InPort)
{
	return Init(0, InPort);
}

bool FSimpleUDPManage::CloseSocket()
{
	ISocketSubsystem* SocketSubsystem = FSimpleConnetion::GetSocketSubsystem();
	if (!SocketSubsystem)
	{
		FString ErrorInfo = TEXT("Failed to create socketSubsystem.");
		NetManageMsgDelegate.ExecuteIfBound(ESimpleNetErrorType::INIT_FAIL, ErrorInfo);
		UE_LOG(LogSimpleNetChannel, Error, TEXT("%s"), *ErrorInfo);

		return false;
	}

	if (Socket)
	{
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;

		return true;
	}

	return false;
}

bool FSimpleUDPManage::Init(const FString& InIP, uint32 InPort)
{
	ISocketSubsystem* SocketSubsystem = FSimpleConnetion::GetSocketSubsystem();
	if (SocketSubsystem)
	{
		auto NewAddrTmp = SocketSubsystem->CreateInternetAddr();

		bool bBindAddr = false;
		NewAddrTmp->SetIp(*InIP, bBindAddr);
		uint32 UintIP;
		NewAddrTmp->GetIp(UintIP);

		return Init(UintIP, InPort);
	}

	return false;
}

bool FSimpleUDPManage::Init(uint32 InIP, uint32 InPort)
{
	Super::Init(InIP, InPort);

	ResetLocalConnetion();

	if (Net.LocalConnetion)
	{
		ISocketSubsystem* SocketSubsystem = FSimpleConnetion::GetSocketSubsystem();
		if (!SocketSubsystem)
		{
			FString ErrorInfo = TEXT("Failed to create socketSubsystem.");
			NetManageMsgDelegate.ExecuteIfBound(ESimpleNetErrorType::INIT_FAIL, ErrorInfo);
			UE_LOG(LogSimpleNetChannel, Error, TEXT("%s"), *ErrorInfo);

			return false;
		}

		if (!CloseSocket())
		{
			//UE_LOG(LogSimpleNetChannel, Display, TEXT("Failed to close the socket. It may have been closed. This message can be ignored."));
		}

		Socket = SocketSubsystem->CreateSocket(NAME_DGram, TEXT("RenZhai"));
		if (!Socket)
		{
			FString ErrorInfo = TEXT("Failed to create Socket.");
			NetManageMsgDelegate.ExecuteIfBound(ESimpleNetErrorType::INIT_FAIL, ErrorInfo);
			UE_LOG(LogSimpleNetChannel, Error, TEXT("%s"), *ErrorInfo);

			return false;
		}

		int32 RecvSize = 0;
		int32 SendSize = 0;
		Socket->SetReceiveBufferSize(FSimpleNetGlobalInfo::Get()->GetInfo().RecvDataNumber, RecvSize);
		Socket->SetSendBufferSize(FSimpleNetGlobalInfo::Get()->GetInfo().SendDataNumber, SendSize);

		//Is it monitoring
		switch (LinkState)
		{
		case ESimpleNetLinkState::LINKSTATE_LISTEN://Server
		{
			Net.LocalConnetion->GetAddr()->SetAnyAddress();
			Net.LocalConnetion->GetAddr()->SetPort(InPort == 0 ? FSimpleNetGlobalInfo::Get()->GetInfo().Port : InPort);

			int32 BoundPort = SocketSubsystem->BindNextPort(Socket, *Net.LocalConnetion->GetAddr(), 1, 1);
			if (!BoundPort)
			{
				FString ErrorInfo = TEXT("Server failed to bind port.");
				NetManageMsgDelegate.ExecuteIfBound(ESimpleNetErrorType::INIT_FAIL, ErrorInfo);

				UE_LOG(LogSimpleNetChannel, Error, TEXT("%s"), *ErrorInfo);
				SocketSubsystem->DestroySocket(Socket);
				return false;
			}

			break;
		}
		case ESimpleNetLinkState::LINKSTATE_CONNET://Client
		{
			bool bBindAddr = false;
			if (InIP == 0)
			{
				Net.LocalConnetion->GetAddr()->SetIp(*FSimpleNetGlobalInfo::Get()->GetInfo().URL, bBindAddr);
			}
			else
			{
				Net.LocalConnetion->GetAddr()->SetIp(InIP);
			}
			
			Net.LocalConnetion->GetAddr()->SetPort(InPort == 0 ? FSimpleNetGlobalInfo::Get()->GetInfo().Port : InPort);
			break;
		}
		}

		if (!FSimpleNetGlobalInfo::Get()->GetInfo().bAsynchronous)
		{
			if (!Socket->SetNonBlocking())
			{
				FString ErrorInfo = TEXT("Setting non blocking mode failed.");
				NetManageMsgDelegate.ExecuteIfBound(ESimpleNetErrorType::INIT_FAIL, ErrorInfo);

				UE_LOG(LogSimpleNetChannel, Error, TEXT("%s"), *ErrorInfo);
				SocketSubsystem->DestroySocket(Socket);
				return false;
			}
		}

		//Register socket
		Net.LocalConnetion->SetSocket(Socket);

		Net.LocalConnetion->SetLinkState(LinkState);

		Net.LocalConnetion->Init();

		//Client ready to connect
		if (LinkState == ESimpleNetLinkState::LINKSTATE_CONNET)
		{
			Net.LocalConnetion->ConnectVerification();
		}
		else if (LinkState == ESimpleNetLinkState::LINKSTATE_LISTEN)
		{
			for (int32 i = 0; i < FSimpleNetGlobalInfo::Get()->GetInfo().MaxConnections; i++)//Pre allocated memory
			{
				Net.RemoteConnetions.Add(MakeShareable(new FSimpleUDPConnetion()));
				TSharedPtr<FSimpleConnetion> InNewConnetion = Net.RemoteConnetions.Last();
				InNewConnetion->SetSocket(Socket);
				InNewConnetion->SetLinkState(LinkState);
				InNewConnetion->SetManage(this);

				//	InNewConnetion->Init();
			}

			//Set up local direct join
			Net.LocalConnetion->SetState(ESimpleConnetionLinkType::LINK_JOIN);
			Net.LocalConnetion->GetMainChannel()->InitController();

			//Create and bind threads
			if (FSimpleNetGlobalInfo::Get()->GetInfo().bAsynchronous)
			{
				auto ReturnThreadValue = FSimpleNetThread::Bind(FSimpleDelegate::CreateRaw(this, &FSimpleUDPManage::Run));
			}
		}

		bInit = true;
	}

	return true;
}

void FSimpleUDPManage::Tick(float DeltaTime)//Both the client and the server listen for messages
{
	Super::Tick(DeltaTime);

	if (bInit) //Initialization completed
	{
		if (Socket)
		{
			if (Net.LocalConnetion.IsValid())
			{
				//tick
				if (Net.LocalConnetion->GetState() == ESimpleConnetionLinkType::LINK_JOIN)
				{
					Net.LocalConnetion->Tick(DeltaTime);
				}
				else //Check whether the client times out
				{
					if (LinkState == ESimpleNetLinkState::LINKSTATE_CONNET)
					{
						Net.LocalConnetion->CheckLoginTimeout(DeltaTime);
					}
				}

				for (auto& Tmp : Net.RemoteConnetions)
				{
					if (Tmp->GetState() == ESimpleConnetionLinkType::LINK_JOIN)
					{
						Tmp->Tick(DeltaTime);
					}
				}

				//Do you want to turn on asynchronous
				if (!FSimpleNetGlobalInfo::Get()->GetInfo().bAsynchronous)
				{
					Listen();
				}
			}
		}
	}
}

void FSimpleUDPManage::Listen()
{
	if (!Socket)
	{
		return;
	}

	int32 RecvDataNumber = FSimpleNetGlobalInfo::Get()->GetInfo().RecvDataNumber;
#if PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX
	uint8 Data[8196 * 1024] = { 0 };
#elif /*PLATFORM_IOS || */PLATFORM_ANDROID
	uint8 Data[8196 * 10] = { 0 };
#else
	uint8 Data[8196 * 100] = { 0 };
#endif
	int32 BytesRead = 0;
	ISocketSubsystem* SocketSubsystem = FSimpleConnetion::GetSocketSubsystem();
	TSharedPtr<FInternetAddr> RemoteAddr = SocketSubsystem->CreateInternetAddr();
	bool bRecvFrom = Socket->RecvFrom(Data, RecvDataNumber, BytesRead, *RemoteAddr);
	if (bRecvFrom)
	{
		SimpleEncryptionAndDecryption::Decryption(Data, BytesRead);

		FGuid InGUID;
		if (LinkState == ESimpleNetLinkState::LINKSTATE_LISTEN)
		{
			if (TSharedPtr<FSimpleConnetion> NewConnetion = Net[RemoteAddr])
			{	
				uint8* NewRecvData = nullptr;
				int32 NewRecvLen = 0;
				if (IsCompletePackage(BytesRead, Data, NewConnetion, InGUID, NewRecvData, NewRecvLen))
				{
					if (NewConnetion->GetState() == ESimpleConnetionLinkType::LINK_JOIN)
					{
						NewConnetion->Analysis(NewRecvData, NewRecvLen);//Analysis
					}
					else
					{
						VerificatioConnetionInfo(NewConnetion, NewRecvData,NewRecvLen);
					}
				}
			}
			else
			{
				if (TSharedPtr<FSimpleConnetion> TmpConnetion = Net.GetEmptyConnetion(RemoteAddr))
				{
					uint8* NewRecvData = nullptr;
					int32 NewRecvLen = 0;
					if (IsCompletePackage(BytesRead, Data, TmpConnetion, InGUID, NewRecvData, NewRecvLen))
					{
						VerificatioConnetionInfo(TmpConnetion,NewRecvData, NewRecvLen);

						UE_LOG(LogSimpleNetChannel,
							Display,
							TEXT("[Hello] There are clients trying to join. Client[IP:%s Port:%d]"),
							*RemoteAddr->ToString(false),
							RemoteAddr->GetPort());
					}
				}
				else
				{
					UE_LOG(LogSimpleNetChannel,
						Error,
						TEXT("The number of connections is full. Client[IP:%s Port:%d]"),
						*RemoteAddr->ToString(false),
						RemoteAddr->GetPort());
				}
			}
		}
		else if (LinkState == ESimpleNetLinkState::LINKSTATE_CONNET) //The client can parse the data directly
		{
			uint8* NewRecvData = nullptr;
			int32 NewRecvLen = 0;
			if (IsCompletePackage(BytesRead, Data, Net.LocalConnetion, InGUID, NewRecvData, NewRecvLen))
			{
				if (Net.LocalConnetion->GetState() == ESimpleConnetionLinkType::LINK_JOIN)
				{
					Net.LocalConnetion->Analysis(NewRecvData, NewRecvLen);
				}
				else
				{
					VerificatioConnetionInfo(Net.LocalConnetion, NewRecvData,NewRecvLen);
				}
			}
		}

		if (InGUID != FGuid())
		{
			Caches.Remove(InGUID);
		}
	}
}

FSimpleUDPManage::FCache::FCache()
{
	Reset();
}

bool FSimpleUDPManage::IsCompletePackage(
	int32 InRecvNum,
	uint8* InData,
	TSharedPtr<FSimpleConnetion> InConnetion,
	FGuid & OutGUID, 
	uint8 *&OutData,
	int32 &OutLen)
{
	if (InConnetion.IsValid())
	{
		if (InRecvNum != 0)
		{
			bool bShowCompletePackProtocolInfo = FSimpleNetGlobalInfo::Get()->GetInfo().bShowCompletePackProtocolInfo;
			bool bSlidingWindow = FSimpleNetGlobalInfo::Get()->GetInfo().bSlidingWindow;
			if (bSlidingWindow)
			{
				FSimplePackageHead PackageHead = *(FSimplePackageHead*)InData;
				if (PackageHead.bForceSend)
				{			
					if (FSimpleUDPManage::FCache* InCache = Caches.Find(PackageHead.PackageID))
					{
						Caches.Remove(PackageHead.PackageID);

						if (bShowCompletePackProtocolInfo)
						{
							UE_LOG(LogSimpleNetChannel, Display, TEXT("ForceRecv PackageID=%s"),
								*PackageHead.PackageID.ToString());
						}
					}
					else
					{
						//Remove the head
						Caches.Add(PackageHead.PackageID, FCache());

						if (FSimpleUDPManage::FCache* InUPDCache = Caches.Find(PackageHead.PackageID))
						{
							int32 BoySize = InRecvNum - sizeof(FSimplePackageHead);
							InData += sizeof(FSimplePackageHead);
							int32 Pos = InUPDCache->Cache.AddUninitialized(BoySize);
							FMemory::Memcpy(&InUPDCache->Cache[Pos], InData, BoySize);
							OutData = InUPDCache->Cache.GetData();
							OutLen = InUPDCache->Cache.Num();
							OutGUID = PackageHead.PackageID;

							if (bShowCompletePackProtocolInfo)
							{
								UE_LOG(LogSimpleNetChannel, Display, TEXT("ForceRecv PackageID=%s RecvSize = %i BoySize = %i"),
									*PackageHead.PackageID.ToString(),
									InRecvNum,
									BoySize);
							}
						}
						else
						{
							UE_LOG(LogSimpleNetChannel, Error, TEXT("ForceRecv not found PackageID = %s "), *PackageHead.PackageID.ToString());
						}
					}
				}
				else
				{
					switch (PackageHead.Protocol)
					{
						case SP_HandshaketoSend:
						{
							PackageHead.Protocol = SP_ReadytoAccept;
							TArray<uint8> MyData;

							FSimpleIOStream IOStream(MyData);
							IOStream << PackageHead;

							//Create accepted cache pool
							Caches.Add(PackageHead.PackageID, FSimpleUDPManage::FCache());
							Caches[PackageHead.PackageID].TotalSize = PackageHead.PackageSize;

							InConnetion->Send(MyData);

							if (bShowCompletePackProtocolInfo)
							{
								UE_LOG(LogSimpleNetChannel, Display, TEXT("[SP_HandshaketoSend] PackageHead.PackageID=%s, ReadyRecv=%i"),
									*PackageHead.PackageID.ToString(),
									Caches[PackageHead.PackageID].TotalSize);
							}
							break;
						}
						case SP_ReadytoAccept:
						{
							if (FSimpleChannel* InChannel = InConnetion->GetChannel(PackageHead.ChannelID))
							{
								InChannel->SendBatch(PackageHead.PackageID, PackageHead.PackageIndex, SP_Recv);

								if (bShowCompletePackProtocolInfo)
								{
									UE_LOG(LogSimpleNetChannel, Display, TEXT("[SP_ReadytoAccept] PackageHead.ChannelID=%s,PackageHead.PackageIndex=%i"),
										*PackageHead.ChannelID.ToString(),
										PackageHead.PackageIndex);
								}
							}
							else
							{
								UE_LOG(LogSimpleNetChannel, Error, TEXT("[SP_ReadytoAccept] Not found PackageHead.ChannelID=%s"),
									*PackageHead.ChannelID.ToString());
							}

							break;
						}
						case SP_RecvComplete:
						{
							if (FSimpleChannel* InChannel = InConnetion->GetChannel(PackageHead.ChannelID))
							{
								InChannel->AcceptSuccess(PackageHead.PackageID, PackageHead.PackageIndex - 1, true, true);
								
								if (bShowCompletePackProtocolInfo)
								{
									UE_LOG(LogSimpleNetChannel, Display, TEXT("[SP_RecvComplete] PackageHead.ChannelID=%s,PackageHead.PackageIndex=%i"),
										*PackageHead.ChannelID.ToString(),
										PackageHead.PackageIndex);
								}
							}
							else
							{
								UE_LOG(LogSimpleNetChannel, Error, TEXT("[SP_RecvComplete] Not found PackageHead.ChannelID=%s"),
									*PackageHead.ChannelID.ToString());
							}

							break;
						}
						case SP_Send:
						{
							if (FSimpleChannel* InChannel = InConnetion->GetChannel(PackageHead.ChannelID))
							{
								//Last package accepted successfully
								InChannel->AcceptSuccess(PackageHead.PackageID, PackageHead.PackageIndex - 1, true);

								//Send the next packet
								InChannel->SendBatch(PackageHead.PackageID, PackageHead.PackageIndex, SP_Recv);
							
								if (bShowCompletePackProtocolInfo)
								{
									UE_LOG(LogSimpleNetChannel, Display, TEXT("[SP_Send] PackageHead.ChannelID=%s,PackageHead.PackageIndex=%i"),
										*PackageHead.ChannelID.ToString(),
										PackageHead.PackageIndex);
								}
							}
							else
							{
								UE_LOG(LogSimpleNetChannel, Error, TEXT("[SP_Send] Not found PackageHead.ChannelID=%s"),
									*PackageHead.ChannelID.ToString());
							}

							break;
						}
						case SP_Recv:
						{
							if (FSimpleUDPManage::FCache* InCache = Caches.Find(PackageHead.PackageID))
							{
								int32 InDataSize = InRecvNum - sizeof(FSimplePackageHead);
								int32 Pos = InCache->Cache.AddUninitialized(InDataSize);

								InData += sizeof(FSimplePackageHead);

								FMemory::Memcpy(&InCache->Cache[Pos], InData, InDataSize);

								TArray<uint8> MyData;
								if (InCache->Cache.Num() < (int32)InCache->TotalSize)//Representative data not accepted
								{
									//Continue sending
									PackageHead.Protocol = SP_Send;
									PackageHead.PackageIndex++;
								}
								else
								{
									PackageHead.Protocol = SP_RecvComplete;
									OutData = InCache->Cache.GetData();
									OutLen = InCache->Cache.Num();
									OutGUID = PackageHead.PackageID;
								}

								FSimpleIOStream IOStream(MyData);
								IOStream << PackageHead;

								InConnetion->Send(MyData);

								if (bShowCompletePackProtocolInfo)
								{
									UE_LOG(LogSimpleNetChannel, Display, TEXT("[SP_Recv] PackageHead.ChannelID=%s,PackageHead.PackageIndex=%i Protocol=%s"),
										*PackageHead.PackageID.ToString(),
										PackageHead.PackageIndex,
										PackageHead.Protocol == SP_Send ? TEXT("SP_Send") :
										PackageHead.Protocol == SP_RecvComplete ? TEXT("SP_RecvComplete") : TEXT("ERROR"));
								}					
							}

							break;
						}
					}
				}
			}
			else
			{
				OutData = InData;
				OutLen = InRecvNum;
			}
		}
	}

	return OutData != nullptr && OutLen != 0;
}

//iocp
void FSimpleUDPManage::Close()
{
	Super::Close();

	if (Socket)
	{
		if (ISocketSubsystem* SocketSubsystem = FSimpleConnetion::GetSocketSubsystem())
		{
			SocketSubsystem->DestroySocket(Socket);
			Socket = nullptr;
		}
	}

	bEndThread = true;
}

void FSimpleUDPManage::Close(const FSimpleAddrInfo& InCloseConnetion)
{
	Super::Close(InCloseConnetion);



}

void FSimpleUDPManage::ResetLocalConnetion()
{
	Net.LocalConnetion = MakeShareable(new FSimpleUDPConnetion());
	Net.LocalConnetion->SetConnetionType(ESimpleConnetionType::CONNETION_MAIN_LISTEN);
	Net.LocalConnetion->SetManage(this);
}

void FSimpleUDPManage::Run()
{
	while (!bEndThread)
	{
		Listen();
	}
}

void FSimpleUDPManage::FCache::Reset()
{
	TotalSize = 0;
	Cache.Empty();
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif
