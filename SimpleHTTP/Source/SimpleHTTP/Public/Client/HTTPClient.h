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
#include "Request/HTTPClientRequest.h"

using namespace SimpleHTTP::HTTP;

namespace SimpleHTTP
{
	namespace HTTP
	{
		struct FHTTPClient
		{
			FHTTPClient();

			bool GetObject(FGetObjectRequest &Request) const;
			bool DeleteObject(FDeleteObjectsRequest &Request)const;
			bool PutObject(FPutObjectRequest &Request) const;
			bool PostObject(FPostObjectsRequest &Request) const;
			void AbortRequest(IHTTPClientRequest &Request);
		};
	}
}