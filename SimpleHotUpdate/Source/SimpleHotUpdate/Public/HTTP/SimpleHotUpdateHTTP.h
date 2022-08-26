// Copyright (C) RenZhai.2020.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Http.h"

DECLARE_DELEGATE_ThreeParams(FHttpUHRequestProgressDelegate, FHttpRequestPtr /*Request*/, int32 /*TotalBytes*/, int32 /*BytesReceived*/)

namespace SimpleHotUpdateHTTP
{
	struct FHTTPDelegate
	{
		FHttpRequestCompleteDelegate						SimpleCompleteDelegate;
		FHttpUHRequestProgressDelegate						SimpleSingleRequestProgressDelegate;
		FHttpRequestHeaderReceivedDelegate					SimpleSingleRequestHeaderReceivedDelegate;
		FSimpleDelegate										AllTasksCompletedDelegate;
	};

	struct FHTTP
	{
		FHTTP(FHTTPDelegate InDelegate);
		static FHTTP* CreateHTTPObject(FHTTPDelegate InDelegate);
		void GetObjectToMemory(const FString& URL);
		void GetObjectsToMemory(const TArray<FString>& URL);

	private:
		void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
		void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived);
		void OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);
		void OnRequestsComplete();
	private:
		void AddTaskCount();
		bool CheckCompleteCount();
	private:
		FHTTPDelegate Delegate;
		int32 CompleteCount;
		int32 TaskCount;

		TMap<FName, int32> DataSizeInfoMap;
	};
}