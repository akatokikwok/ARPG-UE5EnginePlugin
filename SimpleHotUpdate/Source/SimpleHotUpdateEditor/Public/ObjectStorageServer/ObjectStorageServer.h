// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Protocol/SimpleHotUpdateProtocol.h"

namespace SimpleUnrealPakEditor
{
	struct FObjectStorageClient
	{
		//上传数据和版本
		static void HTTPPut(const FString& InPakInstallationPath = TEXT(""), bool bMainVersion = false, const FString& InFilePath = TEXT(""));
		static void OSSPut(const FString &InPakInstallationPath = TEXT(""), bool bMainVersion = false, const FString& InFilePath = TEXT(""));
		static void LocalPut(const FString& InPakInstallationPath = TEXT(""), bool bMainVersion = false, const FString& InFilePath = TEXT(""));

		//上传版本
		static void HTTPPutVersion(const FString& InVersion);
		static void OSSPutVersion(const FString& InVersion);
		static void LocalPutVersion(const FString& InVersion);

		//解锁服务器锁
		static void HTTPUnlock();
		static void OSSUnlock();
		static void LocalUnlock();

		static void MsgPrint(ESimpleHotUpdateProtocol InProtocol);
	};
}