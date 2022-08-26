// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "HTTP/SimpleHotUpdateHTTP.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

#ifndef ENGINE_MAJOR_VERSION
#define ENGINE_MAJOR_VERSION 5
#endif // !ENGINE_MAJOR_VERSION

#ifndef ENGINE_MINOR_VERSION
#define ENGINE_MINOR_VERSION 2
#endif // !ENGINE_MAJOR_VERSION

#ifndef ENGINE_PATCH_VERSION
#define ENGINE_PATCH_VERSION 1
#endif // !ENGINE_MAJOR_VERSION

namespace SimpleHotUpdateHTTP
{
	FHTTP::FHTTP(FHTTPDelegate InDelegate)
		:Delegate(InDelegate)
		, CompleteCount(0)
		, TaskCount(0)
	{
	}

	FHTTP* FHTTP::CreateHTTPObject(FHTTPDelegate InDelegate)
	{
		return new FHTTP(InDelegate);
	}

	void FHTTP::GetObjectToMemory(const FString& URL)
	{
#if (ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 2 && ENGINE_PATCH_VERSION > 5))
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpReuest = FHttpModule::Get().CreateRequest();
#else
		TSharedRef<IHttpRequest> HttpReuest = FHttpModule::Get().CreateRequest();
#endif
		HttpReuest->SetURL(URL);
		HttpReuest->SetVerb(TEXT("GET"));
		HttpReuest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
		HttpReuest->OnProcessRequestComplete().BindRaw(this, &FHTTP::OnRequestComplete);
		HttpReuest->OnRequestProgress().BindRaw(this, &FHTTP::OnRequestProgress);
		HttpReuest->OnHeaderReceived().BindRaw(this, &FHTTP::OnRequestHeaderReceived);
	
		HttpReuest->ProcessRequest();

		AddTaskCount();
	}

	void FHTTP::GetObjectsToMemory(const TArray<FString>& URL)
	{
		for (auto &Tmp : URL)
		{
			GetObjectToMemory(Tmp);
		}
	}

	void FHTTP::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		Delegate.SimpleCompleteDelegate.ExecuteIfBound(HttpRequest, HttpResponse, bSucceeded);

		if (CheckCompleteCount())
		{
			OnRequestsComplete();
		}
	}

	void FHTTP::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived)
	{
		int32 TotalByts = 0;
		if (int32 *TotalBytsPtr = DataSizeInfoMap.Find(*HttpRequest->GetURL()))
		{
			TotalByts = *TotalBytsPtr;
		}

		Delegate.SimpleSingleRequestProgressDelegate.ExecuteIfBound(HttpRequest, TotalByts, BytesReceived);
	}

	void FHTTP::OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
	{
		if (HeaderName == TEXT("Content-Length"))
		{
			DataSizeInfoMap.Add(*Request->GetURL(), FCString::Atoi(*NewHeaderValue));
		}

		Delegate.SimpleSingleRequestHeaderReceivedDelegate.ExecuteIfBound(Request, HeaderName, NewHeaderValue);
	}

	void FHTTP::OnRequestsComplete()
	{
		Delegate.AllTasksCompletedDelegate.ExecuteIfBound();
		delete this;
	}

	void FHTTP::AddTaskCount()
	{
		TaskCount++;
	}

	bool FHTTP::CheckCompleteCount()
	{
		CompleteCount++;
		return CompleteCount >= TaskCount;
	}
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif