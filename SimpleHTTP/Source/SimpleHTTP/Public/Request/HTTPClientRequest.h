// Copyright (C) RenZhai.2020.All Rights Reserved.
/*
�������ַ���������ʹ�ñ��ײ��
 DocURL��			https://zhuanlan.zhihu.com/p/82195344

 �������ַ������ο�����ǰ�Ĳ��
 MarketplaceURL :   https://www.aboutcg.org/courseDetails/682/introduce

 ������˽�������UE4�̳���ο���
 URL :				https://zhuanlan.zhihu.com/p/60117613

 �����ϵͳ�˽���լϵ�н̳��Լ���ز���������� ���Թ�ע�ҵĲ���
 URL :				http://renzhai.net/

 ������˽�������һ�ڵĿγ̰��ſ��� ������΢�Ź��ں����� ��լ ��ע���� ���ǻ����ͺܶ฻�м���������

 ���˲���            https://weibo.com/BZRZ/profile?s=6cm7D0  //������ʹ�16�굽���� ��������û��ô�ù� �Ժ�˵���������������� �ȷ�������
 */
#pragma once

#include "CoreMinimal.h"
#include "Request/RequestInterface.h"

namespace SimpleHTTP
{
	namespace HTTP
	{
		struct FPutObjectRequest :IHTTPClientRequest
		{
			FPutObjectRequest(const FString &URL, const FString& ContentString);
			FPutObjectRequest(const FString &URL, const TArray<uint8>& ContentPayload);
			FPutObjectRequest(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
		};

		struct FGetObjectRequest :IHTTPClientRequest
		{
			FGetObjectRequest(const FString &URL);
		};

		struct FDeleteObjectsRequest :IHTTPClientRequest
		{
			FDeleteObjectsRequest(const FString &URL);
		};

		struct FPostObjectsRequest :IHTTPClientRequest
		{
			FPostObjectsRequest(const FString &URL);
		};
	}
}