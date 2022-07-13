// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "UObject/Core/SimpleNetObject.h"

void USimpleNetworkObject::Init()
{

}

void USimpleNetworkObject::Tick(float DeltaTime)
{

}

TSharedPtr<FSimpleConnetion > USimpleNetworkObject::GetConnetion()
{
	if (Channel)
	{
		return Channel->GetConnetion();
	}

	return NULL;
}

FSimpleNetManage* USimpleNetworkObject::GetManage()
{
	if (TSharedPtr<FSimpleConnetion >ConnetionPtr = GetConnetion())
	{
		return ConnetionPtr->GetManage();
	}

	return NULL;
}

void USimpleNetworkObject::DestroySelf()
{
	if (Channel)
	{
		Channel->DestroySelf();
	}
}

bool USimpleNetworkObject::IsMainConnetion()
{
	check(GetConnetion());

	return GetConnetion()->GetConnetionType()== ESimpleConnetionType::CONNETION_MAIN_LISTEN;
}

void USimpleNetworkObject::Close()
{
	RecvDelegate.Clear();
}

bool USimpleNetworkObject::GetServerAddrInfo(FSimpleAddrInfo& InAddrInfo)
{
	return GetLocalAddrInfo(InAddrInfo);
}

bool USimpleNetworkObject::GetAddrInfo(FSimpleAddrInfo& InAddrInfo)
{
	return GetRemoteAddrInfo(InAddrInfo);
}

void USimpleNetworkObject::RecvProtocol(uint32 InProtocol)
{
	if (RecvDelegate.IsBound())
	{
		RecvDelegate.Broadcast(InProtocol, Channel);
	}
}

FSimpleChannel* USimpleNetworkObject::GetChannel()
{
	return Channel;
}

bool USimpleNetworkObject::GetRemoteAddrInfo(FSimpleAddrInfo& InAddrInfo)
{
	if (Channel)
	{
		return Channel->GetAddrInfo(InAddrInfo);
	}

	return false;
}

bool USimpleNetworkObject::GetLocalAddrInfo(FSimpleAddrInfo& InAddrInfo)
{
	if (GetManage() && GetManage()->GetChannel())
	{
		return GetManage()->GetChannel()->GetAddrInfo(InAddrInfo);
	}

	return false;
}