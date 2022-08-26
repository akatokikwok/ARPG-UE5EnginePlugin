// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleHotUpdateEditorType.h"
#include "Settings/Core/ObjectStorageSettingsBase.h"
#include "ObjectStorageServerSettingsEditor.generated.h"

UCLASS(config = ObjectStorageServerSettingsEditor)
class UObjectStorageServerSettingsEditor : public UObjectStorageSettingsBase
{
	GENERATED_BODY()

public:
	UObjectStorageServerSettingsEditor();

	UPROPERTY(config, EditAnywhere, Category = ObjectStorageServerSettings, meta = (ToolTip = "Hot update"))
	EObjectStorageServerType ObjectStorageServerType;
	
	UPROPERTY(config, EditAnywhere, Category = ObjectStorageServerSettings, meta = (ToolTip = "Hot update"))
	bool bClearPak;
};