// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/SimpleMysqlLink.h"
#include "SimpleMysqlObject.generated.h"


/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class SIMPLEMYSQL_API USimpleMysqlObject : public UObject
{
	GENERATED_BODY()

public:
	USimpleMysqlObject();

	void InitMysql(const FString &InUser,
		const FString &InHost,
		const FString &InPawd,
		const FString &InDB,
		const uint32  InPort,
		const FString &Unix_Socket = TEXT("\0"),
		const TArray<ESimpleClientFlags> &InClientFlag = TArray<ESimpleClientFlags>());

	TSharedRef<FSimpleMysqlLink> GetLink();
private:
	TSharedPtr<FSimpleMysqlLink> Link;
};
