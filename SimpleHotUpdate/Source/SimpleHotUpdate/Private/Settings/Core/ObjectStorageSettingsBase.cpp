// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "Settings/Core/ObjectStorageSettingsBase.h"

void UObjectStorageSettingsBase::SaveObjectStorageConfig()
{
	FString SettingsEditorConfigPath = FPaths::ProjectConfigDir() / TEXT("Default") + GetClass()->ClassConfigName.ToString() + TEXT(".ini");
	SaveConfig(CPF_Config, *SettingsEditorConfigPath);
}

void UObjectStorageSettingsBase::LoadObjectStorageConfig()
{
	FString SettingsEditorConfigPath = FPaths::ProjectConfigDir() / TEXT("Default") + GetClass()->ClassConfigName.ToString() + TEXT(".ini");
	LoadConfig(GetClass(), *SettingsEditorConfigPath);
}
