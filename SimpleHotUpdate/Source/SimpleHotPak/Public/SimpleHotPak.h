// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FPakPlatformFile;

class FSimpleHotPakModule : public IModuleInterface
{
public:
	FSimpleHotPakModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FPakPlatformFile* GetPakPlatformFile();

private:
	FPakPlatformFile* PakPlatformFile;
};
