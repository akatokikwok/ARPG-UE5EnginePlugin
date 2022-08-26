// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "Settings/ObjectStorageSettings.h"

UObjectStorageSettings::UObjectStorageSettings()
	:PatchVersionLogName("Version/VersionLog.con")
	,ServerVersionName("Version/ServerVersion.con")
	,VersionLock("Version/MutexVersion.con")
{

}

void UObjectStorageSettings::UpdateStorageSettingsInstance(const FString& InValue)
{
	if (UObjectStorageSettings * InObjectStorageSettings = const_cast<UObjectStorageSettings*>(GetDefault<UObjectStorageSettings>()))
	{
		InObjectStorageSettings->LinkURL = *InValue;
		InObjectStorageSettings->VersionLock = VersionLock;
		InObjectStorageSettings->ServerVersionName = ServerVersionName;
		InObjectStorageSettings->PatchVersionLogName = PatchVersionLogName;

		InObjectStorageSettings->SaveObjectStorageConfig();
	}
}
