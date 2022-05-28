// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "SimpleUDPConnetion.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "../Log/SimpleNetChannelLog.h"
#include "Global/SimpleNetGlobalInfo.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

void FSimpleUDPConnetion::ConnectVerification()
{
	Super::ConnectVerification();
}

void FSimpleUDPConnetion::Analysis(uint8* InData, int32 BytesNumber)
{
	Super::Analysis(InData, BytesNumber);
	//char* String = (char*)InData;
	
//	UE_LOG(LogSimpleNetChannel, Display, TEXT("Analysis Data = [%s],Number = [%i]"), UTF8_TO_TCHAR(String),BytesNumber);
}

void FSimpleUDPConnetion::Send(TArray<uint8>& InData)
{
	FSimpleConnetion::Send(InData);

	if (Socket)
	{
		int32 BytesSend = 0;
		if (Socket->SendTo(InData.GetData(), InData.Num(), BytesSend,*GetAddr()))
		{
			if (FSimpleNetGlobalInfo::Get()->GetInfo().bShowSendDebug)
			{
				UE_LOG(LogSimpleNetChannel, Display, TEXT("SendTo %i Bytes"), BytesSend);
			}	
		}
		else
		{
			UE_LOG(LogSimpleNetChannel, Error, TEXT(" Send Error!,Please check whether the peer address is correct"));
		}
	}
}

void FSimpleUDPConnetion::Receive(const FGuid& InChannelID, TArray<uint8>& InData)
{
	FSimpleConnetion::Receive(InChannelID, InData);
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif