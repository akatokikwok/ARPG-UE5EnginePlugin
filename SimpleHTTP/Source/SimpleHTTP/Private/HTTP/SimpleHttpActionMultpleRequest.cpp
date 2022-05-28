// Copyright (C) RenZhai.2020.All Rights Reserved.

#include "HTTP/SimpleHttpActionMultpleRequest.h"
#include "Client/HTTPClient.h"
#include "Core/SimpleHttpMacro.h"
#include "HAL/FileManager.h"
#include "SimpleHTTPLog.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Math/UnrealMathUtility.h"

FSimpleHttpActionMultpleRequest::FSimpleHttpActionMultpleRequest()
	:FSimpleHttpActionRequest()
	,RequestNumber(0)
{

}

void FSimpleHttpActionMultpleRequest::GetObjects(const TArray<FString> &URL, const FString &SavePaths)
{
	SetPaths(SavePaths);

	for (const auto &Tmp : URL)
	{
		FHTTPClient Client;

		FGetObjectRequest Request(Tmp);

		REQUEST_BIND_FUN(FSimpleHttpActionMultpleRequest)

		if (Client.GetObject(Request))
		{
			RequestNumber++;

			UE_LOG(LogSimpleHTTP, Log, TEXT("Multple get objects request number %d by multple request."), RequestNumber);
		}
		else
		{
			UE_LOG(LogSimpleHTTP, Error, TEXT("Multple get objects request execution failed."));
		}
	}
}

void FSimpleHttpActionMultpleRequest::GetObjects(const TArray<FString> &URL)
{
	bSaveDisk = false;

	for (const auto &Tmp : URL)
	{
		FHTTPClient Client;

		FGetObjectRequest Request(Tmp);

		REQUEST_BIND_FUN(FSimpleHttpActionMultpleRequest)

		if (Client.GetObject(Request))
		{
			RequestNumber++;
			UE_LOG(LogSimpleHTTP, Log, TEXT("Multple get objects request number %d by multple request."), RequestNumber);
		}
		else
		{
			UE_LOG(LogSimpleHTTP, Error, TEXT("Multple get objects request execution failed."));
		}
	}
}

void FSimpleHttpActionMultpleRequest::DeleteObjects(const TArray<FString> &URL)
{
	for (const auto &Tmp : URL)
	{
		FHTTPClient Client;

		FDeleteObjectsRequest Request(Tmp);

		REQUEST_BIND_FUN(FSimpleHttpActionMultpleRequest)

		if (Client.DeleteObject(Request))
		{
			RequestNumber++;

			UE_LOG(LogSimpleHTTP, Log, TEXT("Multple delete request number %d by multple request."), RequestNumber);
		}
		else
		{
			UE_LOG(LogSimpleHTTP, Error, TEXT("Fail to delete object."));
		}
	}
}

bool FSimpleHttpActionMultpleRequest::PutObject(const FString &URL, const FString &LocalPaths)
{
	if (LocalPaths.Len())
	{
		UE_LOG(LogSimpleHTTP, Log, TEXT("Set path %s."), *LocalPaths);
	}
	
	SetPaths(LocalPaths);

	//Start filtering path
	TArray<FString> AllPaths;
	IFileManager::Get().FindFilesRecursive(AllPaths, *LocalPaths, TEXT("*"), true, true);

	for (const auto &Tmp: AllPaths)
	{
		TArray<uint8> ByteData;
		FFileHelper::LoadFileToArray(ByteData, *Tmp);

		FString ObjectName = FPaths::GetCleanFilename(Tmp);

		FHTTPClient Client;

		FPutObjectRequest Request(URL / ObjectName, ByteData);

		REQUEST_BIND_FUN(FSimpleHttpActionMultpleRequest)

		if (Client.PutObject(Request))
		{
			RequestNumber++;

			UE_LOG(LogSimpleHTTP, Log, TEXT("Multple put object request number %d by multple request."), RequestNumber);
		}
		else
		{
			UE_LOG(LogSimpleHTTP, Error, TEXT("Fail to put object."));
		}
	}

	return RequestNumber > 0;
}

void FSimpleHttpActionMultpleRequest::HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
	else if (!Response.IsValid())
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
				FFileHelper::SaveArrayToFile(Response->GetContent(), *(GetPaths() / Filename));

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

	if (RequestNumber > 0)
	{
		RequestNumber--;
		if (RequestNumber <= 0)
		{
			AllRequestCompleteDelegate.ExecuteIfBound();
			AllTasksCompletedDelegate.ExecuteIfBound();

			bRequestComplete = true;

			UE_LOG(LogSimpleHTTP, Log, TEXT("The task has been completed."));
		}
	}
	else
	{
		UE_LOG(LogSimpleHTTP, Error, TEXT("Request quantity error."));
		//check(0);
	}
}

void FSimpleHttpActionMultpleRequest::HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FSimpleHttpActionRequest::HttpRequestProgress(Request, BytesSent, BytesReceived);
}

void FSimpleHttpActionMultpleRequest::HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
	FSimpleHttpActionRequest::HttpRequestHeaderReceived(Request, HeaderName, NewHeaderValue);
}

