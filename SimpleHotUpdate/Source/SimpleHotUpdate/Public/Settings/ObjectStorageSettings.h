// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/ObjectStorageSettingsBase.h"
#include "SimpleHotUpdateType.h"
#include "ObjectStorageSettings.generated.h"

UCLASS(config = ObjectStorageSettings)
class SIMPLEHOTUPDATE_API UObjectStorageSettings : public UObjectStorageSettingsBase
{
	GENERATED_BODY()

public:
	UObjectStorageSettings();

	UPROPERTY(config, EditAnywhere, Category = GeneralSettings, meta = (ToolTip = "Specify the address to upload."))
	FName PatchVersionLogName;

	UPROPERTY(config, EditAnywhere, Category = GeneralSettings, meta = (ToolTip = "Specify the address to upload."))
	FName ServerVersionName;

	UPROPERTY(config, EditAnywhere, Category = GeneralSettings, meta = (ToolTip = "Specify the address to upload."))
	FName VersionLock;

	UPROPERTY(config,VisibleDefaultsOnly, Category = GeneralSettings, meta = (ToolTip = "Specify the address to upload."))
	FName LinkURL;

	UPROPERTY(config, EditAnywhere, Category = PakSettings, meta = (ToolTip = "Specify the address to upload."))
	bool bInstallContentPak;

	/*项目路径 + 自定义的路径*/
	UPROPERTY(config, EditAnywhere, Category = PakSettings, meta = (ToolTip = "Specify the address to upload.", EditCondition = "!bInstallContentPak"))
	FString RelativeInstallationLocation;

	UPROPERTY(config, EditAnywhere, Category = CustomPakSettings, meta = (ToolTip = "Specify the address to upload."))
	TArray<FCustomVersionInstallation> MainVersionInstallationInfo;

	UPROPERTY(config, EditAnywhere, Category = CustomPakSettings, meta = (ToolTip = "Specify the address to upload."))
	TArray<FCustomVersionInstallation> CustomExtraInstallationInfo;
protected:
	void UpdateStorageSettingsInstance(const FString &InValue);
};