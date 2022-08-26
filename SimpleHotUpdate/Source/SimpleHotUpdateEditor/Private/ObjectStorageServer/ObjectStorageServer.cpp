// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "ObjectStorageServer/ObjectStorageServer.h"
#include "Interfaces/IPluginManager.h"
#include "Settings/SimpleHTTPServerSettingsEditor.h"
#include "Settings/SimpleOSSServerSettingsEditor.h"
#include "Settings/SimpleUnrealPakSettingsEditor.h"
#include "Settings/SimpleLocalSettingsEditor.h"
#include "SimpleHandleProgramReturnValue.h"
#include "Version/SimpleVersion.h"
#include "Misc/FileHelper.h"
#include "Protocol/SimpleHotUpdateProtocol.h"

#define LOCTEXT_NAMESPACE "SimpleUnrealPakEditor"

namespace SimpleUnrealPakEditor
{
	FString GetOSSExePath()
	{
		return IPluginManager::Get().FindPlugin("SimpleHotUpdate")->GetBaseDir() / TEXT("Engine/Binaries/Win64/OSSObjectStorageClient-Win64-DebugGame");
	}

	FString GetHTTPExePath()
	{
		return IPluginManager::Get().FindPlugin("SimpleHotUpdate")->GetBaseDir() / TEXT("Engine/Binaries/Win64/HTTPObjectStorageClient-Win64-DebugGame.exe");
	}

	FString GetLoclExePath()
	{
		return IPluginManager::Get().FindPlugin("SimpleHotUpdate")->GetBaseDir() / TEXT("Engine/Binaries/Win64/LocalObjectStorageClient-Win64-DebugGame.exe");
	}

	FString GetCheckValue(const FString &InString)
	{
		if (InString.IsEmpty())
		{
			return TEXT("NONE");
		}

		return InString;
	}

	void CheckLocalPutVersion(TFunction<void()> InFun)
	{
		if (GetDefault<USimpleLocalSettingsEditor>()->BucketName != NAME_None)
		{
			if (!GetDefault<USimpleLocalSettingsEditor>()->LocalVersionPath.Path.IsEmpty())
			{
				InFun();
			}
			else
			{
				FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("LocalPut_LocalVersionPath",
					"You need to specify a location to store the version locally."));
			}
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("LocalPut_BucketName",
				"You need to specify a bucket name. The bucket name cannot be empty."));
		}
	}

	void CreateProc(const TCHAR* InEXEPath,const TCHAR* InParam)
	{
		void* ReadPipe = nullptr;
		void* WritePipe = nullptr;
		FPlatformProcess::CreatePipe(ReadPipe, WritePipe);
		FProcHandle Handle = FPlatformProcess::CreateProc(InEXEPath, InParam, false, false, false, nullptr, 0, NULL, WritePipe, ReadPipe);
		FPlatformProcess::WaitForProc(Handle);

		FString OutStd = FPlatformProcess::ReadPipe(ReadPipe);

		FMessageDialog::Open(EAppMsgType::YesNo, FText::Format(LOCTEXT("ServerPut",
			"The upload has been completed. Please check the server. {0}"),
			FText::FromString(OutStd)));

		int32 ReturnCode = (int32)ESimpleHotUpdateProtocol::SP_OK_RESPONSE;
		FPlatformProcess::GetProcReturnCode(Handle, &ReturnCode);

		//捕获返回类型
		SimpleUnrealPakEditor::FObjectStorageClient::MsgPrint((ESimpleHotUpdateProtocol)ReturnCode);
	}

	void FObjectStorageClient::MsgPrint(ESimpleHotUpdateProtocol InProtocol)
	{
		switch (InProtocol)
		{
		case ESimpleHotUpdateProtocol::SP_INVALID_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_INVALID_RESPONSE",
				"无效协议 有可能用户传递了 SP_NONE_REQUEST."));
			break;
		case ESimpleHotUpdateProtocol::SP_OK_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_OK_RESPONSE",
				"执行成功."));
			break;
		case ESimpleHotUpdateProtocol::SP_FAIL_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_FAIL_RESPONSE",
				"执行失败."));
			break;
		case ESimpleHotUpdateProtocol::SP_SERVER_LOCKED_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_SERVER_LOCKED_RESPONSE",
				"服务器被锁住,可能是其管理员操作或者死锁."));
			break;
		case ESimpleHotUpdateProtocol::SP_UPLOAD_VERSION_INFORMATION_FAIL_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_UPLOAD_VERSION_INFORMATION_FAIL_RESPONSE",
				"版本信息上传失败."));
			break;
		case ESimpleHotUpdateProtocol::SP_CHECK_VERSION_PATH_IS_EMPTY_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_CHECK_VERSION_PATH_IS_EMPTY_RESPONSE",
				"检测到版本路径为空."));
			break;
		case ESimpleHotUpdateProtocol::SP_CHECK_PAK_LOCAL_PATH_IS_EMPTY_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_CHECK_PAK_LOCAL_PATH_IS_EMPTY_RESPONSE",
				"检测PakLocalPaths路径为空."));
			break;
		case ESimpleHotUpdateProtocol::SP_CLOSE_LOCK_FAIL_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_CLOSE_LOCK_FAIL_RESPONSE",
				"关闭锁失败."));
			break;
		case ESimpleHotUpdateProtocol::SP_CHECK_VERSION_INFO_IS_EMPTY_RESPONSE:
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("MsgPrint_SP_CHECK_VERSION_INFO_IS_EMPTY_RESPONSE",
				"版本信息为空."));
			break;
		}
	}

	void FObjectStorageClient::HTTPPut(const FString& InPakInstallationPath, bool bMainVersion, const FString& InFilePath)
	{
		FString CreateParm = FString::Printf(
		TEXT("-Protocol=%i -VersionName=%s -Account=%s -Password=%s -Bucket=%s -PatchVersionLogName=%s -ServerVersionName=%s\
 -VersionLock=%s -platform=%s -PakLocalPaths=%s -Endpoint=%s -InstallationPath=%s -bMainVersion=%s -FilePath=%s"),
			(int32)ESimpleHotUpdateProtocol::SP_PUT_TO_SERVER_REQUEST,//对应协议
			*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->VersionName.ToString()),
			TEXT(""),
			TEXT(""),
			*GetCheckValue(GetDefault<USimpleHTTPServerSettingsEditor>()->ObjectStorageName.ToString()),
			*GetCheckValue(GetDefault<USimpleHTTPServerSettingsEditor>()->PatchVersionLogName.ToString()),
			*GetCheckValue(GetDefault<USimpleHTTPServerSettingsEditor>()->ServerVersionName.ToString()),
			*GetCheckValue(GetDefault<USimpleHTTPServerSettingsEditor>()->VersionLock.ToString()),
			*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
			*GetCheckValue(InFilePath.IsEmpty() ? GetDefault<USimpleUnrealPakSettingsEditor>()->PakSavePaths.Path:TEXT("")),
			*GetCheckValue(GetDefault<USimpleHTTPServerSettingsEditor>()->LANIP),
			*GetCheckValue(InPakInstallationPath),
			*GetCheckValue(bMainVersion ? TEXT("true") : TEXT("false")),
			*GetCheckValue(InFilePath));

		CreateProc(*GetHTTPExePath(),*CreateParm);
	}

	void FObjectStorageClient::OSSPut(const FString& InPakInstallationPath, bool bMainVersion,const FString &InFilePath)
	{
		FString CreateParm = FString::Printf(
			TEXT("-Protocol=%i -VersionName=%s -AccessKeyId=%s -AccessKeySecret=%s -Bucket=%s -PatchVersionLogName=%s -ServerVersionName=%s\
 -VersionLock=%s -platform=%s -PakLocalPaths=%s -Endpoint=%s -InstallationPath=%s -bMainVersion=%s -FilePath=%s"),
			(int32)ESimpleHotUpdateProtocol::SP_PUT_TO_SERVER_REQUEST,//对应协议
			*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->VersionName.ToString()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->AccessKeyId),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->AccessKeySecret),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->BucketName.ToString()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->PatchVersionLogName.ToString()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->ServerVersionName.ToString()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->VersionLock.ToString()),
			*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
			*GetCheckValue(InFilePath.IsEmpty() ? GetDefault<USimpleUnrealPakSettingsEditor>()->PakSavePaths.Path :TEXT("")),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->Endpoint),
			*GetCheckValue(InPakInstallationPath),
			*GetCheckValue(bMainVersion ? TEXT("true"):TEXT("false")),
			*GetCheckValue(InFilePath));

		CreateProc(*GetOSSExePath(), *CreateParm);
	}

	void FObjectStorageClient::HTTPPutVersion(const FString& InVersion)
	{
		if (InVersion.IsEmpty())
		{
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PutVersion_Version",
				"This interface cannot be called directly when the version information is empty."));

			return;
		}

		FString CreateParm = FString::Printf(
			TEXT("-Protocol=%i -Bucket=%s -ServerVersionName=%s -platform=%s -Endpoint=%s \
-VersionJson=%s"),
			(int32)ESimpleHotUpdateProtocol::SP_UPLOAD_VERSION_INFORMATION_REQUEST,//对应协议
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->BucketName.ToString()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->ServerVersionName.ToString()),
			*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->Endpoint),
			*InVersion);
		
		CreateProc(*GetHTTPExePath(), *CreateParm);
	}

	void FObjectStorageClient::OSSPutVersion(const FString& InVersion)
	{
		if (InVersion.IsEmpty())
		{
			FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PutVersion_Version",
				"This interface cannot be called directly when the version information is empty."));

			return;
		}
		FString CreateParm = FString::Printf(
			TEXT("-Protocol=%i -AccessKeyId=%s -AccessKeySecret=%s -Bucket=%s -ServerVersionName=%s -platform=%s -Endpoint=%s \
-VersionJson=%s"),
			(int32)ESimpleHotUpdateProtocol::SP_UPLOAD_VERSION_INFORMATION_REQUEST,//对应协议
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->AccessKeyId),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->AccessKeySecret),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->BucketName.ToString()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->ServerVersionName.ToString()),
			*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->Endpoint),
			*InVersion);

		CreateProc(*GetOSSExePath(), *CreateParm);
	}

	void FObjectStorageClient::LocalPutVersion(const FString& InVersion)
	{
		CheckLocalPutVersion(
		[&]()
		{
			FString CreateParm = FString::Printf(
				TEXT("-Protocol=%i -VersionName=%s -Bucket=%s -PatchVersionLogName=%s -ServerVersionName=%s\
-VersionLock=%s -platform=%s -BasedOnwhichVersionString=%s \
-LinkURL=%s -VersionJson=%s"),
				(int32)ESimpleHotUpdateProtocol::SP_PUT_TO_SERVER_REQUEST,//对应协议
				*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->VersionName.ToString()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->BucketName.ToString()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->PatchVersionLogName.ToString()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->ServerVersionName.ToString()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->VersionLock.ToString()),
				*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->BasedOnwhichVersion.FilePath),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->LinkURL.ToString()),
				*InVersion);

			CreateProc(*GetLoclExePath(), *CreateParm);
		});
	}

	void FObjectStorageClient::LocalPut(const FString& InPakInstallationPath, bool bMainVersion, const FString& InFilePath)
	{
		CheckLocalPutVersion(
			[&]()
			{
				FString CreateParm = FString::Printf(
					TEXT("-Protocol=%i -VersionName=%s -Bucket=%s -PatchVersionLogName=%s -ServerVersionName=%s\
 -VersionLock=%s -platform=%s -PakLocalPaths=%s -InstallationPath=%s -bMainVersion=%s -FilePath=%s -BasedOnwhichVersionString=%s \
-LinkURL=%s -LocalVersionPath=%s"),
					(int32)ESimpleHotUpdateProtocol::SP_PUT_TO_SERVER_REQUEST,//对应协议
					*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->VersionName.ToString()),
					*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->BucketName.ToString()),
					*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->PatchVersionLogName.ToString()),
					*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->ServerVersionName.ToString()),
					*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->VersionLock.ToString()),
					*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
					*GetCheckValue(FPaths::ConvertRelativePathToFull(InFilePath.IsEmpty() ? GetDefault<USimpleUnrealPakSettingsEditor>()->PakSavePaths.Path : TEXT(""))),
					*GetCheckValue(InPakInstallationPath),
					*GetCheckValue(bMainVersion ? TEXT("true") : TEXT("false")),
					*GetCheckValue(bMainVersion ? FPaths::ConvertRelativePathToFull(InFilePath): InFilePath),
					*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->BasedOnwhichVersion.FilePath.Contains(TEXT("../../../")) ? FPaths::ConvertRelativePathToFull(GetDefault<USimpleLocalSettingsEditor>()->BasedOnwhichVersion.FilePath) : GetDefault<USimpleLocalSettingsEditor>()->BasedOnwhichVersion.FilePath),
					*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->LinkURL.ToString()),
					*GetCheckValue(FPaths::ConvertRelativePathToFull(GetDefault<USimpleLocalSettingsEditor>()->LocalVersionPath.Path)));

				CreateProc(*GetLoclExePath(), *CreateParm);

				//IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
				//FString ServerVersionName = GetDefault<USimpleOSSServerSettingsEditor>()->ServerVersionName.ToString();
				//FString PatchVersionLogName = GetDefault<USimpleOSSServerSettingsEditor>()->PatchVersionLogName.ToString();
				//FString VersionLock = GetDefault<USimpleOSSServerSettingsEditor>()->VersionLock.ToString();
				//FString Platform = GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform();
				//FString BucketName = GetDefault<USimpleLocalSettingsEditor>()->BucketName.ToString();
				//FString PakLocalPaths = GetCheckValue(InFilePath.IsEmpty() ? GetDefault<USimpleUnrealPakSettingsEditor>()->PakSavePaths.Path : TEXT(""));
				//FString FilePath = GetCheckValue(InFilePath);

				//FString RootPath = GetDefault<USimpleLocalSettingsEditor>()->LocalVersionPath.Path / BucketName / Platform;
				//FString VersionPath = RootPath / ServerVersionName;
				//FString VersionLockPath = RootPath / VersionLock;

				//FString BasedOnwhichVersion;
				//if (GetDefault<USimpleLocalSettingsEditor>()->BasedOnwhichVersion.FilePath.IsEmpty())
				//{
				//	if (!PlatformFile.FileExists(*VersionPath))
				//	{
				//		if (EAppReturnType::No == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("LocalPut_VersionPath_Check",
				//			"We didn't find the new version generated based on any version. If your version is the first version, click continue. If your version has a new version, this is only the next version. Click Cancel. The hot update will be considered as the first version.")))
				//		{
				//			return;
				//		}
				//	}

				//	BasedOnwhichVersion = VersionPath;
				//}
				//else
				//{
				//	//优先级最高
				//	BasedOnwhichVersion = GetDefault<USimpleLocalSettingsEditor>()->BasedOnwhichVersion.FilePath;
				//}
				////I 生成基本文件
				//auto CreateDirectory = [](const FString& InPath)
				//{
				//	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
				//	if (!PlatformFile.DirectoryExists(*InPath))
				//	{
				//		PlatformFile.CreateDirectory(*InPath);
				//	}
				//};

				//CreateDirectory(RootPath);
				//CreateDirectory(RootPath/TEXT("Patch"));
				//FFileHelper::SaveStringToFile(TEXT("false"), *VersionLockPath);

				////II 拿到当前最新版本
				//FVersion Version;
				//FString VersionJsonString;
				//FFileHelper::LoadFileToString(VersionJsonString ,*BasedOnwhichVersion);
				//if (!VersionJsonString.IsEmpty())
				//{
				//	SimpleVersionControl::Read(VersionJsonString, Version);
				//}

				////III 判断版本是否一样 如果一样代表热更新 不一样代表有新版本上传
				//Version.Crc = FGuid::NewGuid().ToString();
				//Version.Name = GetDefault<USimpleUnrealPakSettingsEditor>()->VersionName.ToString();

				//FString URL = GetDefault<USimpleLocalSettingsEditor>()->LinkURL.ToString();

				//auto UpdateNewData = [&](const FString& NewPath)
				//{
				//	if (!NewPath.IsEmpty())
				//	{
				//		bool bExists = false;

				//		FString PatchName = FPaths::GetCleanFilename(NewPath);

				//		FRemoteDataDescribe Describe;
				//		FRemoteDataDescribe* RemoteDataDescribe = &Describe;
				//		for (auto& TmpVersion : Version.Content)
				//		{
				//			FString LinkName = FPaths::GetCleanFilename(TmpVersion.Link);
				//			if (LinkName == PatchName)
				//			{
				//				RemoteDataDescribe = &TmpVersion;
				//				bExists = true;
				//				break;
				//			}
				//		}

				//		//安装和主版本
				//		{
				//			RemoteDataDescribe->InstallationPath = InPakInstallationPath;
				//			RemoteDataDescribe->bMainVersion = bMainVersion;
				//		}

				//		//大小
				//		{
				//			TArray<uint8> DataInstance;
				//			FFileHelper::LoadFileToArray(DataInstance, *NewPath);
				//			RemoteDataDescribe->Len = DataInstance.Num();
				//		}

				//		RemoteDataDescribe->Crc = FGuid::NewGuid().ToString();

				//		FString ObjectName = TEXT("Patch") / PatchName;

				//		FString Target = RootPath / ObjectName;
				//		if (SimpleUnrealPakEditor::LocalPut(NewPath, Target))
				//		{
				//			RemoteDataDescribe->Link = URL / Platform / ObjectName;
				//			if (!bExists)
				//			{
				//				Version.Content.Add(*RemoteDataDescribe);
				//			}

				//			//UE_LOG(LogOSSObjectStorageClient, Display, TEXT("Upload %s succeeded."), *FPaths::ConvertRelativePathToFull(NewPath));
				//		}
				//		else
				//		{
				//			//UE_LOG(LogOSSObjectStorageClient, Error, TEXT("Upload %s fail."), *FPaths::ConvertRelativePathToFull(NewPath));
				//		}
				//	}
				//};

				//TArray<FString> PakPaths;
				//if (FilePath != TEXT("NONE"))
				//{
				//	PakPaths.Add(FilePath);
				//}
				//else if (PakLocalPaths != TEXT("NONE"))
				//{
				//	IFileManager::Get().FindFilesRecursive(PakPaths, *PakLocalPaths, TEXT("*"), true, false);
				//}
				//else
				//{
				//	FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("LocalPut_FilePak_Check",
				//		"Both local path and file path are empty. At least one must be set."));
				//	
				//	return;
				//}
				//
				////V上传本地版本
				//for (const auto& Tmp : PakPaths)
				//{
				//	UpdateNewData(Tmp);
				//}

				////VI上传服务器版本
				//VersionJsonString.Empty();
				//SimpleVersionControl::Save(Version, VersionJsonString);

				//FFileHelper::SaveStringToFile(VersionJsonString,*VersionPath);

				//必须要清除，如果想生成下一个版本，它还是会按照这个版本生成
				if (USimpleLocalSettingsEditor *InLocalSettingsEditor = const_cast<USimpleLocalSettingsEditor*>(GetDefault<USimpleLocalSettingsEditor>()))
				{
					InLocalSettingsEditor->BasedOnwhichVersion.FilePath.Empty();
				}
			});
	}

	void FObjectStorageClient::HTTPUnlock()
	{
		FString CreateParm = FString::Printf(
			TEXT("-Protocol=%i -Bucket=%s -ServerVersionName=%s -platform=%s -Endpoint=%s"),
			(int32)ESimpleHotUpdateProtocol::SP_CLOSE_LOCK_REQUEST,//对应协议
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->BucketName.ToString()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->ServerVersionName.ToString()),
			*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->Endpoint));

		CreateProc(*GetHTTPExePath(), *CreateParm);
	}

	void FObjectStorageClient::OSSUnlock()
	{
		FString CreateParm = FString::Printf(
			TEXT("-Protocol=%i -AccessKeyId=%s -AccessKeySecret=%s -Bucket=%s -ServerVersionName=%s -platform=%s -Endpoint=%s"),
			(int32)ESimpleHotUpdateProtocol::SP_CLOSE_LOCK_REQUEST,//对应协议
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->AccessKeyId),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->AccessKeySecret),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->BucketName.ToString()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->ServerVersionName.ToString()),
			*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
			*GetCheckValue(GetDefault<USimpleOSSServerSettingsEditor>()->Endpoint));

		CreateProc(*GetOSSExePath(), *CreateParm);
	}

	void FObjectStorageClient::LocalUnlock()
	{
		CheckLocalPutVersion(
			[&]()
			{
				FString CreateParm = FString::Printf(
					TEXT("-Protocol=%i -VersionName=%s -Bucket=%s -PatchVersionLogName=%s -ServerVersionName=%s \
-VersionLock=%s -platform=%s -LocalVersionPath=%s"),
				(int32)ESimpleHotUpdateProtocol::SP_CLOSE_LOCK_REQUEST,//对应协议
				*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->VersionName.ToString()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->BucketName.ToString()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->PatchVersionLogName.ToString()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->ServerVersionName.ToString()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->VersionLock.ToString()),
				*GetCheckValue(GetDefault<USimpleUnrealPakSettingsEditor>()->GetPlatform()),
				*GetCheckValue(GetDefault<USimpleLocalSettingsEditor>()->LocalVersionPath.Path));

				CreateProc(*GetLoclExePath(), *CreateParm);
			});
	}
}

#undef LOCTEXT_NAMESPACE