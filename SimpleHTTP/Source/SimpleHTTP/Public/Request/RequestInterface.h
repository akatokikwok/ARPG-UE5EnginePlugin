// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

namespace SimpleHTTP
{
	namespace HTTP
	{
		struct IHTTPClientRequest
		{
			friend struct FHTTPClient;

			IHTTPClientRequest();

			IHTTPClientRequest &operator<<(const FHttpRequestCompleteDelegate& SimpleDelegateInstance)
			{
				HttpReuest->OnProcessRequestComplete() = SimpleDelegateInstance;
				return *this;
			}

			IHTTPClientRequest &operator<<(const FHttpRequestProgressDelegate& SimpleDelegateInstance)
			{			
				HttpReuest->OnRequestProgress() = SimpleDelegateInstance;
				return *this;
			}

			IHTTPClientRequest &operator<<(const FHttpRequestHeaderReceivedDelegate& SimpleDelegateInstance)
			{
				HttpReuest->OnHeaderReceived() = SimpleDelegateInstance;
				return *this;
			}

		protected:
			bool ProcessRequest();
			void CancelRequest();

		protected:
			TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> HttpReuest; //4.26
			//TSharedPtr<class IHttpRequest> HttpReuest;
		};
	}
}