// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "Stream/SimpleIOStream.h"
#include "Log/SimpleNetChannelLog.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

FSimpleIOStream::FSimpleIOStream(TArray<uint8>& InBuffer)
:Buffer(InBuffer)
, Ptr(InBuffer.GetData())
{}

void FSimpleIOStream::Wirte(void* InData, int64 InLength)
{
	int32 StartPos = Buffer.AddUninitialized(InLength);
	FMemory::Memcpy(&Buffer[StartPos], InData, InLength);
}

void FSimpleIOStream::Seek(int32 InPos)
{
	if (!Ptr)
	{
		Ptr = Buffer.GetData();
	}

	Ptr += InPos;
}

FSimpleIOStream& FSimpleIOStream::operator<<(FString& InValue)
{
	*this << InValue.GetCharArray();

	return *this;
}

FSimpleIOStream& FSimpleIOStream::operator>>(FString& InValue)
{
	*this >> InValue.GetCharArray();

	return *this;
}

uint8* FSimpleIOStream::Begin()
{
	Ptr = Buffer.GetData();
	return Ptr;
}

uint8* FSimpleIOStream::End()
{
	Begin();
	Ptr += Buffer.Num();
	return Ptr;
}

uint8* FSimpleIOStream::Tall()
{
	return Ptr;
}

void FSimpleIOStream::PrintMsg(const FString& InString)
{
	UE_LOG(LogSimpleNetChannel, Error, TEXT("%s"),*InString);
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif