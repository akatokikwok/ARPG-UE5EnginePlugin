// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once
#include "../SimpleNetChannelType.h"
#include "Misc/Paths.h"

class SIMPLENETCHANNEL_API FSimpleNetGlobalInfo
{
public:
	static FSimpleNetGlobalInfo* Get();
	static void Destroy();

	void Init(const FString &InPath = FPaths::ProjectDir() / TEXT("SimpleNetConfig.ini"));
	const FSimpleConfigInfo& GetInfo() const;
	
	void SetSecretKey(const FString &InSecretKey);
	const TArray<uint8> &GetSecretKey() const;

private:
	void InitSecretKey();

private:
	static FSimpleNetGlobalInfo* Global;
	FSimpleConfigInfo ConfigInfo;

	TArray<uint8> SecretKey;
};