// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/ObjectStorageServerSettingsEditor.h"
#include "Settings/SimpleHTTPServerSettingsEditor.h"
#include "Settings/SimpleOSSServerSettingsEditor.h"
#include "Settings/SimpleLocalSettingsEditor.h"
#include "SimpleHotUpdateType.h"


enum class ESimpleSubMenuType :uint8
{
	HU_CONTENT,
	HU_ASSET
};

const UObjectStorageSettings* GetStorageSettings();
