// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/ObjectStorageSettings.h"
#include "SimpleHTTPServerSettingsEditor.generated.h"

UCLASS(config = SimpleHTTPServerSettingsEditor)
class USimpleHTTPServerSettingsEditor : public UObjectStorageSettings
{
	GENERATED_BODY()

public:
	USimpleHTTPServerSettingsEditor();

	UPROPERTY(config, EditAnywhere, Category = HTTPServerSettings, meta = (ToolTip = "Hot update"))
	FString PublicIP;

	UPROPERTY(config, EditAnywhere, Category = HTTPServerSettings, meta = (ToolTip = "Hot update"))
	FString LANIP;

	UPROPERTY(config, EditAnywhere, Category = HTTPServerSettings, meta = (ToolTip = "Specify the address to upload."))
	FName ObjectStorageName;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif
	virtual void ResetLinkURL();
};