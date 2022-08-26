// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/ObjectStorageSettings.h"
#include "SimpleOSSServerSettingsEditor.generated.h"

UCLASS(config = SimpleOSSServerSettingsEditor)
class USimpleOSSServerSettingsEditor : public UObjectStorageSettings
{
	GENERATED_BODY()

public:
	USimpleOSSServerSettingsEditor();

	UPROPERTY(config, EditAnywhere, Category = OSSServerSettings, meta = (ToolTip = "Hot update"))
	FString AccessKeyId;

	UPROPERTY(config, EditAnywhere, Category = OSSServerSettings, meta = (ToolTip = "Hot update"))
	FString AccessKeySecret;

	UPROPERTY(config, EditAnywhere, Category = OSSServerSettings, meta = (ToolTip = "Hot update"))
	FString Endpoint;

	UPROPERTY(config, EditAnywhere, Category = OSSServerSettings, meta = (ToolTip = "Specify the address to upload."))
	FName BucketName;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif

	virtual void ResetLinkURL();
};