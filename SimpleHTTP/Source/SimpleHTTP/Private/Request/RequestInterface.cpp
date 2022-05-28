// Copyright (C) RenZhai.2020.All Rights Reserved.

#include "Request/RequestInterface.h"
#include "HttpModule.h"
#include "SimpleHTTPLog.h"

SimpleHTTP::HTTP::IHTTPClientRequest::IHTTPClientRequest()
	:HttpReuest(FHttpModule::Get().CreateRequest())
{

}

bool SimpleHTTP::HTTP::IHTTPClientRequest::ProcessRequest()
{
	UE_LOG(LogSimpleHTTP, Log, TEXT("Process Request."));

	return HttpReuest->ProcessRequest();
}

void SimpleHTTP::HTTP::IHTTPClientRequest::CancelRequest()
{
	UE_LOG(LogSimpleHTTP, Log, TEXT("Cancel Request."));

	HttpReuest->CancelRequest();
}

