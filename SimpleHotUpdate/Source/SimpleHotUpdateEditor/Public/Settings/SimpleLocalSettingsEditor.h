// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/ObjectStorageSettings.h"
#include "SimpleLocalSettingsEditor.generated.h"

UCLASS(config = SimpleLocalSettingsEditor)	
class USimpleLocalSettingsEditor : public UObjectStorageSettings
{
	GENERATED_BODY()

public:
	USimpleLocalSettingsEditor();

	UPROPERTY(config, EditAnywhere, Category = LocalVersionSettings, meta = (ToolTip = "You need to specify the previous version on which this version needs to be built."))
	FFilePath BasedOnwhichVersion;

	UPROPERTY(config, EditAnywhere, Category = LocalVersionSettings, meta = (ToolTip = "Version generation path. In this mode, the next version will be generated to the specified file path."))
	FDirectoryPath LocalVersionPath;

	UPROPERTY(config, EditAnywhere, Category = LocalVersionSettings, meta = (ToolTip = "Specify a name."))
	FName BucketName;

	//UPROPERTY(config, EditAnywhere, Category = LocalVersionSettings, meta = (ToolTip = "Should we abandon the HTTP or HTTPS protocol."))
	//bool bHttps;

	UPROPERTY(config, EditAnywhere, Category = LocalVersionSettings, meta = (ToolTip = "Specify the target link, such as where you want to store it renzhai.net Or an IP (example:192.168.32.1) address"))
	FString IPOrDomain;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif

	virtual void ResetLinkURL();

	FString GetVersionPath() const;
};