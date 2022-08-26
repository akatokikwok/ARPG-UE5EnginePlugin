// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "Object/SimpleHotUpdateObject.h"
#include "Log/SimpleHotUpdateLog.h"
#include "Misc/FileHelper.h"
#include "Settings/ObjectStorageSettings.h"
#include "HttpManager.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

UVersionControlObject::UVersionControlObject()
{
	bInitialization = false;
}

UVersionControlObject* UVersionControlObject::CreateObject(UClass* InClass, UObject* InParent)
{
	if (InParent)
	{
		if (UVersionControlObject* Obj = NewObject<UVersionControlObject>(InParent, InClass))
		{
			UE_LOG(LogSimpleVersionControl, Log, TEXT("Create an object with a parent."));
			Obj->InitVersionControlObject();
			return Obj;
		}
	}
	else
	{
		if (UVersionControlObject* Obj = NewObject<UVersionControlObject>(NULL, InClass))
		{
			Obj->AddToRoot();
			Obj->InitVersionControlObject();

			UE_LOG(LogSimpleVersionControl, Log, TEXT("Create an object with a parent."));
			return Obj;
		}
	}

	UE_LOG(LogSimpleVersionControl, Error, TEXT("Creation failed for unknown reason."));

	return nullptr;
}

bool UVersionControlObject::InitVersion()
{
	if (!bInitialization)
	{
		GetCurrentVersion([&](const FVersion&, bool bConConnect)
		{
			if (bConConnect)
			{
				IterativeUpdating();
			}	
		});

		bInitialization = true;
		return true;
	}

	return false;
}

void UVersionControlObject::GetCurrentVersion(const FString& URL, TFunction<void(const FVersion&, bool)> InFunction)
{
	SimpleHotUpdateHTTP::FHTTPDelegate Delegate;
	Delegate.SimpleCompleteDelegate.BindLambda([&, URL, InFunction](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConConnect)
	{
		ServerVersion.Empty();
		if (bConConnect)
		{
			if (!SimpleVersionControl::Read(Response->GetContentAsString(), ServerVersion))
			{
				UE_LOG(LogSimpleVersionControl, Error, TEXT("The request for object server version information failed. Please check whether the permissions of the following OSS servers are public read."));
			}
		}

		InFunction(ServerVersion, bConConnect);
	});

	SimpleHotUpdateHTTP::FHTTP::CreateHTTPObject(Delegate)->GetObjectToMemory(URL);
}

void UVersionControlObject::GetCurrentVersion(TFunction<void(const FVersion&,bool)> InFunction)
{
	FString ServerVersionURL = TEXT("http://") + GetDefault<UObjectStorageSettings>()->LinkURL.ToString() / FPlatformProperties::IniPlatformName() / GetDefault<UObjectStorageSettings>()->ServerVersionName.ToString();
	
	GetCurrentVersion(ServerVersionURL, InFunction);
}

void UVersionControlObject::Tick(float DeltaTime)
{
	FHttpModule::Get().GetHttpManager().Tick(DeltaTime);
}

void UVersionControlObject::SetServerVersion(const FVersion& InServerVersion)
{
	ServerVersion = InServerVersion;
}

void UVersionControlObject::IterativeUpdating()
{
	EServerVersionResponseType InResponseType= GetValidationVersion();

	if (InResponseType == EServerVersionResponseType::CRC_DIFF ||
		InResponseType == EServerVersionResponseType::VERSION_DIFF)
	{
		UpdateVersion();
	}

	VersionDelegate.ExecuteIfBound(InResponseType);
}

EServerVersionResponseType UVersionControlObject::GetValidationVersion()
{
	if (ClientVersion.Name != ServerVersion.Name)
	{
		return EServerVersionResponseType::VERSION_DIFF;
	}
	else if (ClientVersion.Crc != ServerVersion.Crc)
	{
		return EServerVersionResponseType::CRC_DIFF;
	}

	return EServerVersionResponseType::EQUAL;
}

void UVersionControlObject::UpdateVersion()
{
	//I 检测要更新的pak
	TArray<FRemoteDataDescribe> LinkArray;
	for (auto& ServerTmp : ServerVersion.Content)
	{
		if (!ServerTmp.bDiscard)
		{
			for (auto& ClientTmp : ClientVersion.Content)
			{
				if (ServerTmp.Link == ClientTmp.Link)
				{
					if (ServerTmp.Crc != ClientTmp.Crc)
					{
						LinkArray.AddUnique(ServerTmp);
					}

					break;
				}
			}
		}
	}

	//II 检测要添加的pak
	for (auto& ServerTmp : ServerVersion.Content)
	{
		if (ServerTmp.bDiscard)
		{
			continue;
		}

		bool bExitPak = false;
		for (auto& ClientTmp : ClientVersion.Content)
		{
			if (ServerTmp.Link == ClientTmp.Link)
			{
				bExitPak = true;
				break;
			}
		}

		if (!bExitPak)
		{
			LinkArray.AddUnique(ServerTmp);
		}
	}

	//IV 下载
	if (LinkArray.Num() > 0)
	{
		//组装Link
		TArray<FString> Links;
		for (auto &Tmp : LinkArray)
		{
			Links.Add(Tmp.Link);
		}

		if (Links.Num() > 0)
		{
			SimpleHotUpdateHTTP::FHTTPDelegate HTTPDelegate;
			HTTPDelegate.SimpleCompleteDelegate.BindLambda([&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
			{
				OnRequestComplete(HttpRequest, HttpResponse, bSucceeded);
			});
			HTTPDelegate.SimpleSingleRequestHeaderReceivedDelegate.BindLambda([&](FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
			{
				OnRequestHeaderReceived(Request, HeaderName, NewHeaderValue);
			});
			HTTPDelegate.SimpleSingleRequestProgressDelegate.BindLambda([&](FHttpRequestPtr HttpRequest, int32 TotalBytes, int32 BytesReceived)
			{
				OnRequestProgress(HttpRequest, TotalBytes, BytesReceived);
			});
			HTTPDelegate.AllTasksCompletedDelegate.BindLambda([&]()
			{
				OnRequestsComplete();
			});

			SimpleHotUpdateHTTP::FHTTP* InHttp = SimpleHotUpdateHTTP::FHTTP::CreateHTTPObject(HTTPDelegate);

			InHttp->GetObjectsToMemory(Links);
		}
	}
}

void ExistsAndCreate(const FString &InNewPath)
{
	if (!IFileManager::Get().DirectoryExists(*InNewPath))
	{
		IFileManager::Get().MakeDirectory(*InNewPath, true);
	}
}

FString UVersionControlObject::GetDownLoadPersistencePath()
{
	FString DownloadPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("Patch"));
	ExistsAndCreate(DownloadPath);

	return DownloadPath;
}

FString UVersionControlObject::GetDownLoadPersistenceTmpPath()
{
	FString DownloadPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("RelativePatch"));
	ExistsAndCreate(DownloadPath);
	return DownloadPath;
}

void UVersionControlObject::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	FString HeadName = FPaths::GetCleanFilename(HttpRequest->GetURL());

	//保存本地
	if (bSucceeded)
	{
		if (FRemoteDataDescribe* InDescribe = ServerVersion.Content.FindByPredicate([&](const FRemoteDataDescribe& InDescr)
			{
				return InDescr.Link == HttpRequest->GetURL();
			}))
		{
			FString DataFilename;
			if (InDescribe->InstallationPath == TEXT("NONE") || InDescribe->bMainVersion)
			{
				DataFilename = GetDownLoadPersistencePath() / HeadName;
			}
			else
			{
				DataFilename = GetDownLoadPersistenceTmpPath() / HeadName;

				//为独立程序搜集相对非Content的路径
				{
					FString InstallationPath = InDescribe->InstallationPath;
					FPaths::NormalizeFilename(InstallationPath);

					InstallationPath.RemoveFromEnd(TEXT("/"));
					InstallationPath.RemoveFromStart(TEXT("/"));

					FString NewPath = FPaths::ProjectDir() / InstallationPath;

					//检查路径是否存在
					ExistsAndCreate(NewPath);

					NewPath /= HeadName;
					RelativePatchs.Add(NewPath);
				}
			}

			FFileHelper::SaveArrayToFile(HttpResponse->GetContent(), *DataFilename);
			UE_LOG(LogSimpleVersionControl, Log, TEXT("The [%s] was stored successfully."), *HeadName);
		}
		else
		{
			UE_LOG(LogSimpleVersionControl, Error, TEXT("The [%s] No corresponding data found."), *HeadName);
		}
	}
	else
	{
		UE_LOG(LogSimpleVersionControl, Error, TEXT("Failed to store the [%s] =>[%s] "),*HeadName,*HttpResponse->GetContentAsString());
	}
}

void UVersionControlObject::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 TotalBytes, int32 BytesReceived)
{
	FString Head = FPaths::GetCleanFilename(HttpRequest->GetURL());

	float InValue = ((float)BytesReceived / (float)TotalBytes);
	UE_LOG(LogSimpleVersionControl, Log, TEXT("[%s] : %.2lf %%"),*Head, InValue * 100.f);

	ProgressDelegate.ExecuteIfBound(InValue, Head, TotalBytes, BytesReceived);
}

void UVersionControlObject::OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{

}

void UVersionControlObject::OnRequestsComplete()
{
	if (CallInstallationProgress())
	{
		UE_LOG(LogSimpleVersionControl, Log, TEXT("Start up project success!"));

		ClientVersion = ServerVersion;
		SerializeClientVersion();

		//GIsRequestingExit = true;
		//GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
	else
	{
		UE_LOG(LogSimpleVersionControl, Error, TEXT("Start up project failure!"));
		
		check(0);
	}

	//强制直接退出
	FPlatformMisc::RequestExit(true);
}

bool UVersionControlObject::CallInstallationProgress()
{
	auto GetPlatformName = []()->FString
	{
		FString PlatformName = FPlatformProperties::PlatformName();
		if (PlatformName.Contains(TEXT("Windows")))
		{
			return TEXT("Win64");
		}

		return PlatformName;
	};

	//寻找被丢弃的pak
	FString DiscardInfo;
	for (auto& ServerTmp : ServerVersion.Content)
	{
		if (ServerTmp.bDiscard)
		{
			FString Json;
			SimpleVersionControl::Save(ServerTmp, Json);

			if (!Json.IsEmpty())
			{
				DiscardInfo += (Json + TEXT("|"));
			}		
		}
	}
	DiscardInfo.RemoveFromEnd(TEXT("|"));

	//检测
	if (DiscardInfo.IsEmpty())
	{
		DiscardInfo = TEXT("NONE");
	}

	UE_LOG(LogSimpleVersionControl, Log, TEXT("Download task completed."));

	FString ExePath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir() / TEXT("Binaries") / GetPlatformName() / TEXT("SimpleInstallationProgress-Win64-DebugGame.exe"));

	FString Param;
	//-ResourcesToCopied=P:\ReHotUpdate\Save\Pak\ 
	//-ProjectToContentPaks=P:\ReHotUpdate\Content\Paks 
	//-ProjectStartupPath=I:\newue5\Engine\Binaries\Win64\SlateViewer-Win64-DebugGame.exe 
	//-ProjectProcessID=4325
	// 
	//-ResourcesToCopiedCustom=
	Param += TEXT("-ResourcesToCopiedCustom=") + GetDownLoadPersistenceTmpPath();
	//-ResourcesToCopied=
	Param += TEXT(" -ResourcesToCopied=") + GetDownLoadPersistencePath();
	//-ProjectToContentPaks=
	Param += TEXT(" -ProjectToContentPaks=") + FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("Paks"));
	//-ProjectStartupPath=
	Param += TEXT(" -ProjectStartupPath=") + FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Binaries") / GetPlatformName() / FApp::GetProjectName() + TEXT(".exe"));
	//-ProjectProcessID=
	Param += TEXT(" -ProjectProcessID=") + FString::FromInt(FPlatformProcess::GetCurrentProcessId());
	//-Discards=
	Param += TEXT(" -Discards=") + DiscardInfo;
	//-ProjectRootPath=
	Param += TEXT(" -ProjectRootPath=") + FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());;
	//-LinkURL=
	Param += TEXT(" -LinkURL=") + GetDefault<UObjectStorageSettings>()->LinkURL.ToString();
	if (RelativePatchs.Num())
	{
		FString RelativePatch;
		for (auto &Tmp : RelativePatchs)
		{
			RelativePatch += (Tmp + TEXT("+"));
		}
		RelativePatch.RemoveFromEnd(TEXT("+"));

		//-RelativePatchs=
		Param += (TEXT(" -RelativePatchs=") + RelativePatch);
	}
	
	FProcHandle Handle = FPlatformProcess::CreateProc(*ExePath, *Param, false, false, false, nullptr, 0, nullptr, nullptr);
	UE_LOG(LogSimpleVersionControl, Log, TEXT("[Param] : %s"), *Param);
	return Handle.IsValid();
}

void UVersionControlObject::InitVersionControlObject()
{
	FString ClientVersionsPaths = GetClientVersionPosition();
	if (!IFileManager::Get().FileExists(*ClientVersionsPaths))
	{
		SerializeClientVersion();
	}
	else
	{
		DeserializationClientVersion();
	}
}

void UVersionControlObject::SerializeClientVersion()
{
	FString CVJosn;
	SimpleVersionControl::Save(ClientVersion,CVJosn);
	FFileHelper::SaveStringToFile(CVJosn ,*GetClientVersionPosition());
}

void UVersionControlObject::DeserializationClientVersion()
{
	FString CVJosn;
	FFileHelper::LoadFileToString(CVJosn,*GetClientVersionPosition());
	if (!CVJosn.IsEmpty())
	{
		SimpleVersionControl::Read(CVJosn,ClientVersion);
	}
}

FString UVersionControlObject::GetClientVersionPosition()
{
	return FPaths::ProjectSavedDir() / TEXT(".Simple/ClientVersion.con");;
}

FString UVersionControlObject::GetClientVersionName()
{
	return ClientVersion.Name;
}

FString UVersionControlObject::GetServerVersionName()
{
	return ServerVersion.Name;
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif