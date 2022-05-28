// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "SimpleNetManage.h"

class FSocket;
class FSimpleTCPManage :public FSimpleNetManage
{
	typedef FSimpleNetManage Super;
public:
	FSimpleTCPManage(ESimpleNetLinkState InType);

	virtual bool Init(int32 InPort = 0);
	virtual void Tick(float DeltaTime);
	virtual void Close();
};