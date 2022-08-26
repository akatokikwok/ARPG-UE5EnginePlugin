// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "SimpleHotPakType.generated.h"

USTRUCT(BlueprintType)
struct SIMPLEHOTPAK_API FSimpleHotPakConfig
{
	GENERATED_USTRUCT_BODY()

	FSimpleHotPakConfig();

	/*32-bit AES key */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	FString AES;

	/*Is the key read from the local configuration */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	bool bConfiguration;
};