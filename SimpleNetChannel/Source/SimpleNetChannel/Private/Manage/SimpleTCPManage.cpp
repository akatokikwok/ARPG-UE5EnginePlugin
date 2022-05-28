// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "SimpleTCPManage.h"

FSimpleTCPManage::FSimpleTCPManage(ESimpleNetLinkState InType)
	:Super()
{

}

bool FSimpleTCPManage::Init(int32 InPort )
{
	Super::Init();

	return false;
}

void FSimpleTCPManage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bInit)
	{

	}
}

void FSimpleTCPManage::Close()
{
	Super::Close();
}