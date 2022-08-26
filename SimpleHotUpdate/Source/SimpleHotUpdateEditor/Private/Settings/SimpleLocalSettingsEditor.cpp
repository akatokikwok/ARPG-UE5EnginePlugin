// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "Settings/SimpleLocalSettingsEditor.h"
#include "Settings/SimpleUnrealPakSettingsEditor.h"

#define LOCTEXT_NAMESPACE "USimpleLocalSettingsEditor"

USimpleLocalSettingsEditor::USimpleLocalSettingsEditor()
	//:bHttps(false)
{

}

#if WITH_EDITOR
void USimpleLocalSettingsEditor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName().ToString() == TEXT("IPOrDomain") ||
		PropertyChangedEvent.GetPropertyName().ToString() == TEXT("BucketName") ||
		PropertyChangedEvent.GetPropertyName().ToString() == TEXT("LocalVersionPath") ||
		PropertyChangedEvent.GetPropertyName().ToString() == TEXT("ServerVersionName"))

	{
		BasedOnwhichVersion.FilePath = LocalVersionPath.Path / BucketName.ToString();

		ResetLinkURL();
	}
}

#endif
void USimpleLocalSettingsEditor::ResetLinkURL()
{
	IPOrDomain.RemoveFromEnd(TEXT("/"));
	IPOrDomain.RemoveFromEnd(TEXT("//"));
	IPOrDomain.RemoveFromEnd(TEXT("\\"));

	LinkURL = *(IPOrDomain / BucketName.ToString());

	UpdateStorageSettingsInstance(LinkURL.ToString());
}

FString USimpleLocalSettingsEditor::GetVersionPath() const
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FString Platform = GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform();

	FString RootPath = LocalVersionPath.Path / BucketName.ToString() / Platform;
	FString VersionPath = RootPath / ServerVersionName.ToString();

	FString NewPath;
	if (BasedOnwhichVersion.FilePath.IsEmpty())
	{
		if (PlatformFile.FileExists(*VersionPath))
		{
			NewPath = VersionPath;
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::YesNo,
				LOCTEXT("VersionDiscardSettings_locationVersion",
					"There is no matching version in this location. Please regenerate a version."));
		}
	}
	else
	{
		if (BasedOnwhichVersion.FilePath != TEXT("NONE"))
		{
			//优先级最高
			NewPath = BasedOnwhichVersion.FilePath;
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::YesNo,
				LOCTEXT("VersionDiscardSettings_locationVersion2",
					"BasedOnwhichVersion != TEXT(\"NONE\"."));
		}
	}

	return NewPath;
}

#undef LOCTEXT_NAMESPACE