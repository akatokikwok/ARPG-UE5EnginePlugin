// Copyright (C) RenZhai.2020.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#ifdef SIMPLEHOTUPDATE_OPEN_ENGINE_MODULE
#include "Engine/EngineTypes.h"
#endif
#include "SimpleHotUpdateType.generated.h"

#ifndef SIMPLEHOTUPDATE_OPEN_ENGINE_MODULE
struct FFilePath;
#endif


UENUM(BlueprintType)
enum class EObjectStorageServerType :uint8
{
	HTTP_SERVER,
	OSS_SERVER,
	LOCAL_POS,
};

UENUM(BlueprintType)
enum class EServerVersionResponseType :uint8
{
	VERSION_DIFF			UMETA(DisplayName = "Version Diff"),//更新补丁版本
	CRC_DIFF				UMETA(DisplayName = "Crc Diff"),//热更新
	EQUAL					UMETA(DisplayName = "Equal"),
	CONNECTION_ERROR		UMETA(DisplayName = "Connection Error"),
	MAX						UMETA(DisplayName = "Max"),
};

USTRUCT(BlueprintType)
struct SIMPLEHOTUPDATE_API FCustomVersionInstallation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = CustomInstallation)
	FFilePath FilePath;

	UPROPERTY(EditAnywhere, Category = CustomInstallation, meta = (ToolTip = "Specify the address to upload."))
	bool bInstallContentPak;

	UPROPERTY(EditAnywhere, Category = CustomInstallation, meta = (ToolTip = "Specify the address to upload.", EditCondition = "!bInstallContentPak"))
	FString RelativeInstallationLocation;
};