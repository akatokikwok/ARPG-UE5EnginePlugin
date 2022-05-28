// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "Global/SimpleNetGlobalInfo.h"
#include "Misc/FileHelper.h"
#include "Log/SimpleNetChannelLog.h"
#include "Stream/SimpleIOStream.h"

#if PLATFORM_MAC
#include "Mac/MacPlatformProcess.h"
#define INSERT_TEXT(x) *FString(x)
#else
#define INSERT_TEXT(x) TEXT(x)
#endif
enum EParamType
{
	Param_Int,
	Param_Float,
	Param_String,
};

void AutoInsertString(TMap<FString,FString> &InConfigInfo,void* InData,const TCHAR* InFieldName, EParamType InType)
{
	if (FString* InValue = InConfigInfo.Find(InFieldName))
	{
		switch (InType)
		{
		case EParamType::Param_Int:
		{
			*(int32*)InData = FCString::Atoi(**InValue);
			break;
		}
		case EParamType::Param_Float:
		{
			*(float*)InData = FCString::Atof(**InValue);
			break;
		}
		case EParamType::Param_String:
		{
			*(FString*)InData = *InValue;
			break;
		}
		}
	}
	else
	{
		UE_LOG(LogSimpleNetChannel, Error, TEXT("The following fields [%s] could not be found."), InFieldName);
	}
}

FSimpleNetGlobalInfo* FSimpleNetGlobalInfo::Global = NULL;

FSimpleNetGlobalInfo* FSimpleNetGlobalInfo::Get()
{
	if (!Global)
	{
		Global = new FSimpleNetGlobalInfo();
		//Global->Init();//Read configuration table
	}

	return Global;
}

void FSimpleNetGlobalInfo::Destroy()
{
	if (Global)
	{
		delete Global;
		Global = NULL;
	}
}

void FSimpleNetGlobalInfo::SetSecretKey(const FString &InSecretKey)
{
	ConfigInfo.SecretKey = InSecretKey;
}

void FSimpleNetGlobalInfo::Init(const FString& InPath)
{
#if PLATFORM_MAC
#if !WITH_EDITOR
	const FString& InSimpleNetConfigPath = FPaths::ProjectDir() / TEXT("SimpleNetConfig.ini");
	FString InFullPath = FPaths::ConvertRelativePathToFull(InSimpleNetConfigPath);
	FString APPName = FPlatformProcess::ExecutableName();
	FString R, L;
	InFullPath.Split(*(TEXT("/") + APPName + TEXT(".app/")), &L, &R);
	InFullPath = L / TEXT("SimpleNetConfig.ini");
#endif // 0
	const FString InFullPath = FPaths::ConvertRelativePathToFull(InPath);
#else
	const FString InFullPath = FPaths::ConvertRelativePathToFull(InPath);
#endif

	TArray<FString> Content;
	FFileHelper::LoadFileToStringArray(Content, *InFullPath);
	if (Content.Num())
	{
		auto Lamabda = [&](TMap<FString, FString>& OutContent)
		{
			for (auto &Tmp : Content)
			{
				if (Tmp.Contains("[") && Tmp.Contains("]"))//Analytic head
				{
					Tmp.RemoveFromEnd("]");
					Tmp.RemoveFromStart("[");

					OutContent.Add("ConfigHead", Tmp);
				}
				else //Analysis of the body
				{
					FString R, L;
					Tmp.Split(TEXT("="),&L,&R);

					OutContent.Add(L, R);
				}
			}
		};

		TMap<FString, FString> InConfigInfo;
		Lamabda(InConfigInfo);

		AutoInsertString(InConfigInfo,&ConfigInfo.Version, INSERT_TEXT("Version"), EParamType::Param_String);
		AutoInsertString(InConfigInfo,&ConfigInfo.URL, INSERT_TEXT("URL"), EParamType::Param_String);
		AutoInsertString(InConfigInfo,&ConfigInfo.PublicIP, INSERT_TEXT("PublicIP"), EParamType::Param_String);
		AutoInsertString(InConfigInfo,&ConfigInfo.SecretKey, INSERT_TEXT("SecretKey"), EParamType::Param_String);
		AutoInsertString(InConfigInfo,&ConfigInfo.Port, INSERT_TEXT("Port"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.RecvDataNumber, INSERT_TEXT("RecvDataNumber"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.SendDataNumber, INSERT_TEXT("SendDataNumber"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.MaxConnections, INSERT_TEXT("MaxConnections"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.MaxChannels, INSERT_TEXT("MaxChannels"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.bAsynchronous, INSERT_TEXT("bAsynchronous"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.bPrintHeartBeat, INSERT_TEXT("bPrintHeartBeat"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.bSlidingWindow, INSERT_TEXT("bSlidingWindow"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.bRepackaging, INSERT_TEXT("bRepackaging"), EParamType::Param_Int);
		AutoInsertString(InConfigInfo,&ConfigInfo.OutTimeLink, INSERT_TEXT("RepackagingTime"), EParamType::Param_Float);
		AutoInsertString(InConfigInfo,&ConfigInfo.OutTimeLink, INSERT_TEXT("OutTimeLink"), EParamType::Param_Float);
		AutoInsertString(InConfigInfo,&ConfigInfo.HeartBeatTimeTnterval, INSERT_TEXT("HeartBeatTimeTnterval"), EParamType::Param_Float);
	}
	else
	{
		Content.Add(TEXT("[SimpleNetConfig]"));
		Content.Add(FString::Printf(TEXT("Version=%s"),*ConfigInfo.Version));
		Content.Add(FString::Printf(TEXT("URL=%s"), *ConfigInfo.URL));
		Content.Add(FString::Printf(TEXT("Port=%i"),ConfigInfo.Port));
		Content.Add(FString::Printf(TEXT("RecvDataNumber=%i"), ConfigInfo.RecvDataNumber));
		Content.Add(FString::Printf(TEXT("SendDataNumber=%i"), ConfigInfo.SendDataNumber));
		Content.Add(FString::Printf(TEXT("MaxConnections=%i"), ConfigInfo.MaxConnections));
		Content.Add(FString::Printf(TEXT("MaxChannels=%i"), ConfigInfo.MaxChannels));
		Content.Add(FString::Printf(TEXT("bAsynchronous=%i"), ConfigInfo.bAsynchronous));
		Content.Add(FString::Printf(TEXT("bPrintHeartBeat=%i"), ConfigInfo.bPrintHeartBeat));
		Content.Add(FString::Printf(TEXT("bSlidingWindow=%i"), ConfigInfo.bSlidingWindow));
		Content.Add(FString::Printf(TEXT("bRepackaging=%i"), ConfigInfo.bRepackaging));
		Content.Add(FString::Printf(TEXT("OutTimeLink=%f"), ConfigInfo.OutTimeLink));
		Content.Add(FString::Printf(TEXT("RepackagingTime=%f"), ConfigInfo.RepackagingTime));
		Content.Add(FString::Printf(TEXT("HeartBeatTimeTnterval=%f"), ConfigInfo.HeartBeatTimeTnterval));
		Content.Add(FString::Printf(TEXT("PublicIP=%s"),*ConfigInfo.PublicIP));
		Content.Add(FString::Printf(TEXT("SecretKey=%s"), *ConfigInfo.SecretKey));
		FFileHelper::SaveStringArrayToFile(Content, *InFullPath);
	}

	InitSecretKey();
}

void FSimpleNetGlobalInfo::InitSecretKey()
{
	//Key registration
	if (!ConfigInfo.SecretKey.IsEmpty())
	{
		FSimpleIOStream SecretIOStream(SecretKey);
		SecretIOStream << ConfigInfo.SecretKey;
	}
}

const TArray<uint8> &FSimpleNetGlobalInfo::GetSecretKey() const
{
	return SecretKey;
}

const FSimpleConfigInfo& FSimpleNetGlobalInfo::GetInfo() const
{
	// TODO: insert return statement here
	return ConfigInfo;
}
