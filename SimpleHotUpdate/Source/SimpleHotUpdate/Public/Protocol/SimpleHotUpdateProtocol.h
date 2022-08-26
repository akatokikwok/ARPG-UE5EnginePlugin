// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

enum class ESimpleHotUpdateProtocol : uint8
{
	SP_NONE_REQUEST = 0,							//无协议请求
	SP_PUT_TO_SERVER_REQUEST,  						//上传数据和版本信息到服务器
	SP_UPLOAD_VERSION_INFORMATION_REQUEST,			//只上传版本信息到服务器
	SP_CLOSE_LOCK_REQUEST,							//关锁 (防止死锁准备)
	SP_INVALID_RESPONSE,							//无效协议 有可能用户传递了 SP_NONE_REQUEST
	SP_OK_RESPONSE,									//执行成功
	SP_FAIL_RESPONSE,								//执行失败
	SP_SERVER_LOCKED_RESPONSE,						//服务器被锁住,可能是其管理员操作或者死锁	
	SP_UPLOAD_VERSION_INFORMATION_FAIL_RESPONSE,	//版本信息上传失败
	SP_CHECK_VERSION_PATH_IS_EMPTY_RESPONSE,		//检测到版本路径为空
	SP_CHECK_VERSION_INFO_IS_EMPTY_RESPONSE,		//检测到版本信息为空
	SP_CHECK_PAK_LOCAL_PATH_IS_EMPTY_RESPONSE,		//检测PakLocalPaths路径为空
	SP_CLOSE_LOCK_FAIL_RESPONSE,					//关闭锁失败
};