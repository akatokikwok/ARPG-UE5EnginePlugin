#pragma once
#include "CoreMinimal.h"
#include "SimpleNetChannelType.h"

// 登录状态(如账户错误,密码错误,服务器错误,成功之类.)
enum ELoginType
{
	LOGIN_DB_SERVER_ERROR,
	LOGIN_SUCCESS,
	LOGIN_ACCOUNT_WRONG,
	LOGIN_PASSWORD_WRONG,
};

// 检查命名类型.
enum ECheckNameType
{
	UNKNOWN_ERROR,
	NAME_NOT_EXIST,
	SERVER_NOT_EXIST,
	NAME_EXIST,
};

struct MMOARPGCOMMON_API FMMOARPGUserData
{
	FMMOARPGUserData()
		: ID(INDEX_NONE)
	{}

	int32 ID;
	FString Account;
	FString EMail;
	FString Name;
	FString HeadportraitURL;

};

/** 网关状态.*/
struct MMOARPGCOMMON_API FMMOARPGGateStatus
{
	FMMOARPGGateStatus()
		: GateConnectionNum(INDEX_NONE)// 若人数是-1则表明链接已满.
	{}

	int32 GateConnectionNum;// 链接人数.
	FSimpleAddrInfo GateServerAddrInfo;// 网关服务器地址.
};

/** 玩家形象. */
struct MMOARPGCOMMON_API FMMOARPGCharacterAppearance
{
	FMMOARPGCharacterAppearance()
		:Lv(INDEX_NONE)
		,SlotPosition(INDEX_NONE)
	{}
public:
	FString Name;
	FString Date;
	int32 Lv;
	int32 SlotPosition;// 每份CA存档都有一个槽号.
	
};

typedef TArray<FMMOARPGCharacterAppearance> FCharacterAppearances;// 一个玩家可能会有多个角色.

/** Json-用户数据 解析的几个API. */
namespace NetDataAnalysis
{
	// 用户数据写入json.
	MMOARPGCOMMON_API void UserDataToString(const FMMOARPGUserData& InUserData, FString& OutString);// 需要导出宏.
	// Json转译成用户数据.
	MMOARPGCOMMON_API void StringToUserData(const FString& JsonString, FMMOARPGUserData& OutUserData);// 需要导出宏.

	/** 4个负责 解析玩家形象的json方法. */
	void MMOARPGCOMMON_API CharacterAppearancesToString(const FCharacterAppearances& InCAs, FString& OutString);
	void MMOARPGCOMMON_API CharacterAppearancesToString(const FMMOARPGCharacterAppearance& InCAs, FString& OutString);
	void MMOARPGCOMMON_API StringToCharacterAppearances(const FString& OutString, FCharacterAppearances& InCAs);
	void MMOARPGCOMMON_API StringToCharacterAppearances(const FString& OutString, FMMOARPGCharacterAppearance& InCAs);
}