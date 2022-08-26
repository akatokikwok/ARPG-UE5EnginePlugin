// Copyright (C) RenZhai.2020.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Version/SimpleVersion.h"
#include "HTTP/SimpleHotUpdateHTTP.h"
#include "SimpleHotUpdateType.h"
#include "SimpleHotUpdateObject.generated.h"

DECLARE_DELEGATE_FourParams(FProgressDelegate, float /*,InPercentage*/, const FString&, int32 /*TotalBytes*/, int32 /*BytesReceived*/)
DECLARE_DELEGATE_OneParam(FVersionDelegate, EServerVersionResponseType /*,InPercentage*/)

UCLASS(BlueprintType, Blueprintable)
class SIMPLEHOTUPDATE_API UVersionControlObject :public UObject
{
	GENERATED_BODY()

public:
	FProgressDelegate ProgressDelegate;
	FVersionDelegate VersionDelegate;
public:
	UVersionControlObject();

	static UVersionControlObject* CreateObject(UClass* InClass = NULL, UObject* InParent = NULL);

	virtual bool InitVersion();
	
	void GetCurrentVersion(const FString &URL,TFunction<void(const FVersion &, bool)> InFunction);
	void GetCurrentVersion(TFunction<void(const FVersion&, bool)> InFunction);

	static void Tick(float DeltaTime);

	void SetServerVersion(const FVersion &InServerVersion);
protected:
	void IterativeUpdating();
	EServerVersionResponseType GetValidationVersion();
	void UpdateVersion();
	FString GetDownLoadPersistencePath();
	FString GetDownLoadPersistenceTmpPath();
protected:
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 TotalBytes, int32 BytesReceived);
	void OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);
	void OnRequestsComplete();
protected:
	bool CallInstallationProgress();
public:
	void InitVersionControlObject();
	void SerializeClientVersion();
	void DeserializationClientVersion();
	FString GetClientVersionPosition();

	FString GetClientVersionName();
	FString GetServerVersionName();
protected:
	FVersion ClientVersion;
	FVersion ServerVersion;

	bool bInitialization;

protected:
	TArray<FString> RelativePatchs;
};