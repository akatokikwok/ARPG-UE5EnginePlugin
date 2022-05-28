// Copyright (C) RenZhai.2020.All Rights Reserved.
#include "Client/HTTPClient.h"

SimpleHTTP::HTTP::FHTTPClient::FHTTPClient()
{

}

bool SimpleHTTP::HTTP::FHTTPClient::GetObject(FGetObjectRequest &Request) const
{
	return Request.ProcessRequest();
}

bool SimpleHTTP::HTTP::FHTTPClient::DeleteObject(FDeleteObjectsRequest &Request) const
{
	return Request.ProcessRequest();
}

bool SimpleHTTP::HTTP::FHTTPClient::PutObject(FPutObjectRequest &Request) const
{
	return Request.ProcessRequest();
}

void SimpleHTTP::HTTP::FHTTPClient::AbortRequest(IHTTPClientRequest &Request)
{
	Request.CancelRequest();
}

bool SimpleHTTP::HTTP::FHTTPClient::PostObject(FPostObjectsRequest &Request) const
{
	return Request.ProcessRequest();
}