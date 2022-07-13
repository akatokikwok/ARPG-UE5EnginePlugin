// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "UObject/SimpleController.h"
#include "Misc/FileHelper.h"

void USimpleController::Init()
{
	Super::Init();
}

void USimpleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void USimpleController::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);


}

bool USimpleController::DebugServerPrintString(const FString& InData)
{
	FString ProjectDir = FPaths::ProjectSavedDir() / FGuid::NewGuid().ToString() + TEXT(".txt");
	return FFileHelper::SaveStringToFile(InData, *ProjectDir);
}
