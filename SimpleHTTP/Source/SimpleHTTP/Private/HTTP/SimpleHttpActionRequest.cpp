// Copyright (C) RenZhai.2020.All Rights Reserved.

#include "HTTP/SimpleHttpActionRequest.h"
#include "Client/HTTPClient.h"
#include "Core/SimpleHttpMacro.h"
#include "HAL/FileManager.h"
#include "SimpleHTTPLog.h"
#include "Misc/Paths.h"
#include "Math/UnrealMathUtility.h"
//#include "GenericPlatform/GenericPlatformHttp.h"

FSimpleHttpActionRequest::FSimpleHttpActionRequest()
	:bRequestComplete(false)
	,bSaveDisk(true)
{
}

FSimpleHttpActionRequest::~FSimpleHttpActionRequest()
{

}

void FSimpleHttpActionRequest::GetObjects(const TArray<FString> &URL, const FString &SavePaths)
{

}

void FSimpleHttpActionRequest::GetObjects(const TArray<FString> &URL)
{

}

void FSimpleHttpActionRequest::DeleteObjects(const TArray<FString> &URL)
{

}

void FSimpleHttpActionRequest::HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	auto Call = [&]()
	{
		FSimpleHttpRequest SimpleHttpRequest;
		FSimpleHttpResponse SimpleHttpResponse;
		ResponsePtrToSimpleResponse(Response, SimpleHttpResponse);
		RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

		SimpleHttpRequestCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);
		SimpleCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);

		UE_LOG(LogSimpleHTTP, Warning, TEXT("Request completion failed."));
	};

	//404 405 100 -199 200 -299
	if (!Request.IsValid())
	{
		//switch (Request.GetStatus())
		//{
		//case EHttpRequestStatus::NotStarted:
		//case EHttpRequestStatus::Processing:
		//case EHttpRequestStatus::Failed:
		//case EHttpRequestStatus::Failed_ConnectionError:
		//case EHttpRequestStatus::Succeeded:
		//}

		UE_LOG(LogSimpleHTTP, Warning, TEXT("Server request failed."));

		Call();
	}
	else if(!Response.IsValid())
	{
		UE_LOG(LogSimpleHTTP, Warning, TEXT(" Response failed."));

		Call();
	}
	else if (!bConnectedSuccessfully)
	{
		UE_LOG(LogSimpleHTTP, Warning, TEXT("HTTP connected failed."));

		Call();
	}
	else if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG(LogSimpleHTTP, Warning, TEXT("HTTP error code [%d]."), Response->GetResponseCode());

		Call();
	}
	else
	{
		if (Request->GetVerb() == "GET")
		{
			if (bSaveDisk)
			{
				FString Filename = FPaths::GetCleanFilename(Request->GetURL());
				FFileHelper::SaveArrayToFile(Response->GetContent(), *(TmpSavePaths / Filename));

				UE_LOG(LogSimpleHTTP, Log, TEXT("Store the obtained http file locally."));
				UE_LOG(LogSimpleHTTP, Log, TEXT("%s."), *Filename);
			}
			else
			{
				UE_LOG(LogSimpleHTTP, Log, TEXT("This is a get request that is not stored locally."));
			}
		}

		FSimpleHttpRequest SimpleHttpRequest;
		FSimpleHttpResponse SimpleHttpResponse;
		ResponsePtrToSimpleResponse(Response, SimpleHttpResponse);
		RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

		SimpleHttpRequestCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);
		SimpleCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);

		UE_LOG(LogSimpleHTTP, Log, TEXT("Request to complete execution of binding agent."));
	}

	AllRequestCompleteDelegate.ExecuteIfBound();
	AllTasksCompletedDelegate.ExecuteIfBound();

	bRequestComplete = true;
}

void FSimpleHttpActionRequest::HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FSimpleHttpRequest SimpleHttpRequest;
	RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

	SimpleHttpRequestProgressDelegate.ExecuteIfBound(SimpleHttpRequest, BytesSent, BytesReceived);
	SimpleSingleRequestProgressDelegate.ExecuteIfBound(SimpleHttpRequest, BytesSent, BytesReceived);

//	UE_LOG(LogSimpleHTTP, Log, TEXT("Http request progress."));
}

void FSimpleHttpActionRequest::HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
	FSimpleHttpRequest SimpleHttpRequest;
	RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

	SimpleHttpRequestHeaderReceivedDelegate.ExecuteIfBound(SimpleHttpRequest, HeaderName, NewHeaderValue);
	SimpleSingleRequestHeaderReceivedDelegate.ExecuteIfBound(SimpleHttpRequest, HeaderName, NewHeaderValue);

//	UE_LOG(LogSimpleHTTP, Log, TEXT("Http request header received."));
}

void FSimpleHttpActionRequest::Print(const FString &Msg, float Time /*= 10.f*/, FColor Color /*= FColor::Red*/)
{
#ifdef PLATFORM_PROJECT
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg);
	}
#endif
}

bool FSimpleHttpActionRequest::GetObject(const FString &URL, const FString &SavePaths)
{
	TmpSavePaths = SavePaths;

	FHTTPClient Client;

	FGetObjectRequest Request(URL);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.GetObject(Request);
}

bool FSimpleHttpActionRequest::GetObject(const FString &URL)
{
	bSaveDisk = false;

	FHTTPClient Client;

	FGetObjectRequest Request(URL);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.GetObject(Request);
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL,const TArray<uint8> &Data)
{
	FHTTPClient Client;

	FPutObjectRequest Request(URL, Data);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.PutObject(Request);
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL, const FString &LocalPaths)
{
	TArray<uint8> Data;
	if (FFileHelper::LoadFileToArray(Data, *LocalPaths))
	{
		FHTTPClient Client;
		FPutObjectRequest Request(URL, Data);

		REQUEST_BIND_FUN(FSimpleHttpActionRequest)

		return Client.PutObject(Request);
	}

	return false;
}

bool FSimpleHttpActionRequest::PutObjectByString(const FString& URL, const FString& InBuff)
{
	FHTTPClient Client;
	FPutObjectRequest Request(URL, InBuff);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.PutObject(Request);
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	FHTTPClient Client;

	FPutObjectRequest Request(URL, Stream);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.PutObject(Request);
}

bool FSimpleHttpActionRequest::DeleteObject(const FString &URL)
{
	FHTTPClient Client;

	FDeleteObjectsRequest Request(URL);

	return Client.DeleteObject(Request);
}

bool FSimpleHttpActionRequest::PostObject(const FString &URL)
{
	//FString NewURL = FGenericPlatformHttp::UrlEncode(URL);

	bSaveDisk = false;

	FHTTPClient Client;

	FPostObjectsRequest Request(URL);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.PostObject(Request);
}