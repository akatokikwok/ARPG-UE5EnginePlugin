// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleEncryptionAndDecryption.h"
#include "Global/SimpleNetGlobalInfo.h"

namespace SimpleEncryptionAndDecryption
{
	void Encryption(TArray<uint8>& InData)
	{
		const FString& SecretKey = FSimpleNetGlobalInfo::Get()->GetInfo().SecretKey;
		if (!SecretKey.IsEmpty())
		{
			const TArray<uint8>& SecretKeyArray = FSimpleNetGlobalInfo::Get()->GetSecretKey();
			for (auto& Tmp : InData)
			{
				for (auto& TmpSecretKey : SecretKeyArray)
				{
					Tmp ^= TmpSecretKey;
				}
			}
		}
	}

	void Decryption(uint8* InData, int32 InLen)
	{
		const FString& SecretKey = FSimpleNetGlobalInfo::Get()->GetInfo().SecretKey;
		if (!SecretKey.IsEmpty())
		{
			const TArray<uint8>& SecretKeyArray = FSimpleNetGlobalInfo::Get()->GetSecretKey();
			for (int32 i = 0; i < InLen; i++)
			{
				for (int32 j = (SecretKeyArray.Num() - 1); j > -1; j--)
				{
					InData[i] ^= SecretKeyArray[j];
				}
			}
		}
	}

	void Decryption(TArray<uint8>& InData)
	{
		Decryption(InData.GetData(), InData.Num());
	}
}
