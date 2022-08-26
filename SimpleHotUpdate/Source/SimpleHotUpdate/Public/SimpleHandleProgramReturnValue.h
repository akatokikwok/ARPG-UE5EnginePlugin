// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

enum class ESimpleHandleProgramReturnValue :uint8
{
	SIMPLEHANDLEPROGRAMRETURNVALUE_OK = 0,
	SIMPLEHANDLEPROGRAMRETURNVALUE_SERVER_LOCK =1, //The server is occupied by other users. Or deadlock, please find out the cause
};