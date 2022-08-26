// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectStorageSettingsBase.generated.h"

UCLASS(config = ObjectStorageSettingsBase)
class SIMPLEHOTUPDATE_API UObjectStorageSettingsBase : public UObject
{
	GENERATED_BODY()

public:
	UObjectStorageSettingsBase(){}

public:
	virtual void SaveObjectStorageConfig();
	virtual void LoadObjectStorageConfig();

	virtual void ResetLinkURL(){}
};