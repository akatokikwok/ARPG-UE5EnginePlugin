// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "Channel/SimpleChannel.h"
#include "Connetion/Core/SimpleConnetion.h"
#include "UObject/SimpleController.h"
#include "UObject/SimplePlayer.h"
#include "SocketSubsystem.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "Protocols/SimpleNetProtocols.h"
#include "../Log/SimpleNetChannelLog.h"
#include "Async/TaskGraphInterfaces.h"

 FSimpleReturnDelegate FSimpleChannel::SimpleControllerDelegate;
 FSimpleReturnDelegate FSimpleChannel::SimplePlayerDelegate;

FSimpleChannel::FSimpleChannel()
{

}

void FSimpleChannel::Init()
{

}

void FSimpleChannel::PreClose()
{
	if (Object.IsValid())
	{
		Object->Close();
	}
}

void FSimpleChannel::Close()
{	
	if (Object.IsValid())
	{
		Object->MarkPendingKill();
		Object.Reset();
		Object = NULL;
	}

	ID = FGuid();
}

void FSimpleChannel::Send(TArray<uint8>& InData, bool bForceSend)
{
	if (ConnetionPtr.IsValid())
	{	
		if (InData.Num())
		{
			if (!FSimpleNetGlobalInfo::Get()->GetInfo().bSlidingWindow)
			{
				ConnetionPtr.Pin()->Send(InData);
			}
			else
			{
				int32 SendDataNumber = FSimpleNetGlobalInfo::Get()->GetInfo().SendDataNumber;
				FSimpleBunchHead* Head = (FSimpleBunchHead*)InData.GetData();
				FSimplePackageHead PackageHead;
				PackageHead.PackageSize = InData.Num();
				PackageHead.ChannelID = Head->ChannelID;
				if (!bForceSend)
				{
					Batchs.Add(PackageHead.PackageID, FSimpleChannel::FBatch());
					FSimpleChannel::FBatch& Batch = Batchs[PackageHead.PackageID];

					PackageHead.Protocol = SP_HandshaketoSend;

					TArray<uint8> HandshakeData;
					HandshakeData.AddUninitialized(sizeof(FSimplePackageHead));
					FMemory::Memcpy(HandshakeData.GetData(), &PackageHead, sizeof(FSimplePackageHead));

					//包体过大开始拆分
					if (InData.Num() > SendDataNumber)
					{
						int32 BatchsNumber = FMath::CeilToInt((float)InData.Num() / (float)SendDataNumber);
						int32 LastNumber = BatchsNumber - 1;
						for (int32 i = 0; i < BatchsNumber; i++)
						{
							PackageHead.PackageIndex = i;

							Batch.Sequence.Add(i, FSimpleChannel::FBatch::FElement());
							FSimpleChannel::FBatch::FElement& Tmp = Batch.Sequence[i];

							Tmp.Package.AddUninitialized(sizeof(FSimplePackageHead));
							FMemory::Memcpy(Tmp.Package.GetData(), &PackageHead, sizeof(FSimplePackageHead));
							if (i == LastNumber)
							{
								int32 Surplus = InData.Num() - SendDataNumber * LastNumber;
								int32 Pos = Tmp.Package.AddUninitialized(Surplus);

								FMemory::Memcpy(&Tmp.Package[Pos], &InData[LastNumber * SendDataNumber], Surplus);
							}
							else
							{
								int32 Pos = Tmp.Package.AddUninitialized(SendDataNumber);
								FMemory::Memcpy(&Tmp.Package[Pos], &InData[i * SendDataNumber], SendDataNumber);
							}
						}
					}
					else
					{
						Batch.Sequence.Add(0, FSimpleChannel::FBatch::FElement());
						FSimpleChannel::FBatch::FElement& Tmp = Batch.Sequence[0];

						Tmp.Package.AddUninitialized(sizeof(FSimplePackageHead));
						FMemory::Memcpy(Tmp.Package.GetData(), &PackageHead, sizeof(FSimplePackageHead));

						int32 Pos = Tmp.Package.AddUninitialized(InData.Num());
						FMemory::Memcpy(&Tmp.Package[Pos], InData.GetData(), InData.Num());
					}

					ConnetionPtr.Pin()->Send(HandshakeData);
				}
				else
				{
					PackageHead.bForceSend = true;

					int32 PackageHeadSize = sizeof(FSimplePackageHead);
					TArray<uint8> ForceSendData;

					int32 Pos = ForceSendData.AddUninitialized(sizeof(FSimplePackageHead));
					FMemory::Memcpy(ForceSendData.GetData(), &PackageHead, sizeof(FSimplePackageHead));

					Pos = ForceSendData.AddUninitialized(InData.Num());
					FMemory::Memcpy(&ForceSendData[Pos], InData.GetData(), InData.Num());

					ConnetionPtr.Pin()->Send(ForceSendData);
				}
			}
		}
	}
}

bool FSimpleChannel::Receive(TArray<uint8>& InData)
{
	if (MsgQueue.IsValidIndex(0))
	{
		InData = MoveTemp(MsgQueue[0]);
		MsgQueue.RemoveAt(0);

		return true;
	}

	return false;
}

void FSimpleChannel::AddMsg(TArray<uint8>& InData)
{
	MsgQueue.Empty();

	MsgQueue.Add(MoveTemp(InData));
}

void FSimpleChannel::InitController()
{
	if (GetNetObject())
	{
		GetNetObject()->Init();
	}
}

USimpleNetworkObject* FSimpleChannel::GetNetObject()
{
	return Object.Get();
}

void FSimpleChannel::SpawnController()
{
	auto SpawnMyObject = [&]()
	{
		if (ConnetionPtr.IsValid())
		{
			if (ConnetionPtr.Pin()->GetManage()->NetworkObjectClass)
			{
				RegisterObject(ConnetionPtr.Pin()->GetManage()->NetworkObjectClass, USimpleController::StaticClass());
			}
			else
			{
				RegisterObject(SimpleControllerDelegate, USimpleController::StaticClass());
			}
		}
		else
		{
			RegisterObject(SimpleControllerDelegate, USimpleController::StaticClass());
		}
	};

	if(IsInGameThread())
	{
		SpawnMyObject();
	}
	else
	{
		FFunctionGraphTask::CreateAndDispatchWhenReady([SpawnMyObject]()
		{
			SpawnMyObject();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
	
	ID = FGuid::NewGuid();
}

void FSimpleChannel::SpawnPlayer()
{
	auto SpawnMyObject = [&]()
	{
		if (ConnetionPtr.IsValid())
		{
			if (ConnetionPtr.Pin()->GetManage()->SimplePlayerClass)
			{
				RegisterObject(ConnetionPtr.Pin()->GetManage()->SimplePlayerClass, USimplePlayer::StaticClass());
			}
			else
			{
				RegisterObject(SimplePlayerDelegate, USimplePlayer::StaticClass());
			}
		}
		else
		{
			RegisterObject(SimplePlayerDelegate, USimplePlayer::StaticClass());
		}
	};

	if (IsInGameThread())
	{
		SpawnMyObject();
	}
	else
	{
		FFunctionGraphTask::CreateAndDispatchWhenReady([SpawnMyObject]()
		{
			SpawnMyObject();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
	
	ID = FGuid::NewGuid();
}

bool FSimpleChannel::GetAddrInfo(FSimpleAddrInfo& InAddrInfo)
{
	if (ConnetionPtr.IsValid())
	{
		ConnetionPtr.Pin()->GetAddr()->GetIp(InAddrInfo.Addr.IP);
		InAddrInfo.Addr.Port = ConnetionPtr.Pin()->GetAddr()->GetPort();
		InAddrInfo.ChannelID = GetGuid();

		return true;
	}

	return false;
}

USimpleNetworkObject* FSimpleChannel::SpawnObject(UClass* InClass)
{
	if (USimpleNetworkObject *InObject = NewObject<USimpleNetworkObject>(InClass, InClass))
	{
		return InObject;
	}

	return NULL;
}

void FSimpleChannel::RegisterObject(UClass* InClass, UClass* InObjectClass)
{
	if (Object.IsValid())
	{
		Close();
	}

	if (InClass)
	{
		Object = TStrongObjectPtr<USimpleNetworkObject>(SpawnObject(InClass));
	}
	else
	{
		Object = TStrongObjectPtr<USimpleNetworkObject>(SpawnObject(InObjectClass));
	}

	if (Object.IsValid())
	{
		Object->LinkState = ConnetionPtr.Pin()->GetLinkState();
		Object->Channel = this;
	}
}

void FSimpleChannel::RegisterObject(FSimpleReturnDelegate InDelegate, UClass* InObjectClass)
{
	if (InDelegate.IsBound())
	{
		RegisterObject(InDelegate.Execute(), InObjectClass);
	}
	else
	{
		RegisterObject(NULL, InObjectClass);
	}
}

bool FSimpleChannel::IsValid() const
{
	return ID != FGuid();
}

void FSimpleChannel::AcceptSuccess(const FGuid& InDataID, uint32 InIndex, bool bAck, bool bAllSuccessful)
{
	if (FSimpleChannel::FBatch *InBatch = Batchs.Find(InDataID))
	{
		if (bAllSuccessful)
		{
			InBatch->bAck = true;
		}
		else
		{
			if (FSimpleChannel::FBatch::FElement *Element = InBatch->Sequence.Find(InIndex))
			{
				Element->bAck = bAck;
			}
		}
	}
}

void FSimpleChannel::SendBatch(const FGuid& InDataID, uint32 InIndex, uint32 InProtocol)
{
	if (ConnetionPtr.IsValid())
	{
		if (FSimpleChannel::FBatch* InBatch = Batchs.Find(InDataID))
		{
			if (FSimpleChannel::FBatch::FElement* Element = InBatch->Sequence.Find(InIndex))
			{
				FSimplePackageHead *PackageHead = (FSimplePackageHead*)Element->Package.GetData();
				PackageHead->Protocol = InProtocol;

				ConnetionPtr.Pin()->Send(Element->Package);

				//Enable patch detection
				Element->bStartUpRepackaging = true;
			}
		}
	}
}

void FSimpleChannel::Tick(float DeltaSeconds)
{
	if (Object.IsValid())
	{
		Object->Tick(DeltaSeconds);
	}

	if (ConnetionPtr.IsValid())
	{
		TArray<FGuid> RemoveBatchID;
		for (auto& Tmp : Batchs)
		{
			if (Tmp.Value.bAck)
			{
				RemoveBatchID.Add(Tmp.Key);
			}
			else
			{
				if (FSimpleNetGlobalInfo::Get()->GetInfo().bRepackaging)
				{
					for (auto& TmpSequence : Tmp.Value.Sequence)
					{
						//Do you want to start the packet replenishment test
						if (TmpSequence.Value.bStartUpRepackaging)
						{
							//Do you complete the repackaging
							if (!TmpSequence.Value.bAck)
							{
								TmpSequence.Value.CurrentTime += DeltaSeconds;
								if (TmpSequence.Value.CurrentTime >= FSimpleNetGlobalInfo::Get()->GetInfo().RepackagingTime)
								{
									TmpSequence.Value.CurrentTime = 0.f;
									TmpSequence.Value.RepeatCount++;

									//Repackaging
									ConnetionPtr.Pin()->Send(TmpSequence.Value.Package);

									if (TmpSequence.Value.RepeatCount >= FSimpleNetGlobalInfo::Get()->GetInfo().RepackagingFrequency)
									{
										//Start cleaning to prevent protocol attacks
										Tmp.Value.bAck = true;

										UE_LOG(LogSimpleNetChannel, Error, TEXT("Unable to get client acceptance confirmation,Please check whether the local sending buffer is too large, or the opposite receiving buffer is too small, or whether the other party is offline."));
										break;
									}
								}
							}
						}
					}
				}
			}
		}

		for (auto& Tmp : RemoveBatchID)
		{
			Batchs.Remove(Tmp);
		}
	}
}

void FSimpleChannel::RecvProtocol(uint32 InProtocolsNumber)
{
	if (Object.IsValid())
	{
		Object->RecvProtocol(InProtocolsNumber);
	}
}

const FGuid& FSimpleChannel::GetGuid() const
{
	return ID;
}

void FSimpleChannel::DestroySelf()
{
	ConnetionPtr.Pin()->Close();
}

void FSimpleChannel::SetGuid(const FGuid& NewGuid)
{
	ID = NewGuid;
}

void FSimpleChannel::SetConnetion(TWeakPtr<FSimpleConnetion> InConnetion)
{
	ConnetionPtr = InConnetion;
}

TSharedPtr<FSimpleConnetion > FSimpleChannel::GetConnetion()
{
	return ConnetionPtr.Pin();
}