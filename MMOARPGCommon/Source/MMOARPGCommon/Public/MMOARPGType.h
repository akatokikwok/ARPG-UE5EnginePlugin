#pragma once
#include "CoreMinimal.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.generated.h"

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

/** 玩家用户信息. */
struct MMOARPGCOMMON_API FMMOARPGUserData
{
	FMMOARPGUserData()
		: ID(INDEX_NONE)
	{}
public:
	void Reset();

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
USTRUCT()
struct MMOARPGCOMMON_API FMMOARPGCharacterAppearance
{
	GENERATED_USTRUCT_BODY()
public:
	FMMOARPGCharacterAppearance()
		: Lv(INDEX_NONE)
		, SlotPosition(INDEX_NONE)
		, LegSize(0.f)
		, WaistSize(0.f)
		, ArmSize(0.f)
	{}
public:
	// 还原并格式化所有CA存档里的字段.
	void Reset();

public:
	UPROPERTY()
		FString Name;
	UPROPERTY()
		FString Date;
	UPROPERTY()
		int32 Lv;
	UPROPERTY()
		int32 SlotPosition;// 每份CA存档都有自己独立的槽号.
	UPROPERTY()
		float LegSize;
	UPROPERTY()
		float WaistSize;
	UPROPERTY()
		float ArmSize;
};

FORCEINLINE bool operator==(const FMMOARPGCharacterAppearance& Lhs, const FMMOARPGCharacterAppearance& Rhs)
{
	return Lhs.SlotPosition == Rhs.SlotPosition;
}

typedef TArray<FMMOARPGCharacterAppearance> FCharacterAppearances;// 一个玩家可能会有多个角色.

/** 玩家注册信息. */
struct MMOARPGCOMMON_API FMMOARPGPlayerRegistInfo
{
public:
	FMMOARPGUserData UserInfo;// 玩家信息.
	FMMOARPGCharacterAppearance CAInfo;// 玩家CA存档.
public:
	void Reset();
	bool IsVaild();

};

/**
 * 仿自虚幻的FGameplayAttributeData.
 * MMOARPG项目的 AttributeData
 */
USTRUCT(BlueprintType)
struct MMOARPGCOMMON_API FMMOARPGAttributeData
{
	GENERATED_USTRUCT_BODY()
public:
	FMMOARPGAttributeData()
		: BaseValue(0.f)
		, CurrentValue(0.f)
	{}

	UPROPERTY()
		float BaseValue;

	UPROPERTY()
		float CurrentValue;
};

/**
 * MMOARPG人物的GAS属性集
 */
USTRUCT(BlueprintType)
struct MMOARPGCOMMON_API FMMOARPGCharacterAttribute
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		FMMOARPGAttributeData Health;

	UPROPERTY()
		FMMOARPGAttributeData MaxHealth;

	UPROPERTY()
		FMMOARPGAttributeData Mana;

	UPROPERTY()
		FMMOARPGAttributeData MaxMana;
};

/**
 * Json-用户数据 解析的几个API.
 */
namespace NetDataAnalysis
{
	// 用户数据写入json.
	MMOARPGCOMMON_API void UserDataToString(const FMMOARPGUserData& InUserData, FString& OutString);// 需要导出宏.
	// Json转译成用户数据.
	MMOARPGCOMMON_API void StringToUserData(const FString& JsonString, FMMOARPGUserData& OutUserData);// 需要导出宏.

	/** 4个负责 解析玩家形象的json方法. */
	void MMOARPGCOMMON_API CharacterAppearancesToString(const FCharacterAppearances& InCAs, FString& OutString);
	void MMOARPGCOMMON_API CharacterAppearancesToString(const FMMOARPGCharacterAppearance& InCA, FString& OutString);
	void MMOARPGCOMMON_API StringToCharacterAppearances(const FString& InJsonString, FCharacterAppearances& OutCAs);
	void MMOARPGCOMMON_API StringToCharacterAppearances(const FString& InJsonString, FMMOARPGCharacterAppearance& OutCA);

	/** 从Json里解析出人物GAS属性集 */
	bool MMOARPGCOMMON_API StringToMMOARPGCharacterAttribute(const FString& InJson, FMMOARPGCharacterAttribute& Out_CA);
	/** 人物GAS属性集压入Json */
	void MMOARPGCOMMON_API MMOARPGCharacterAttributeToString(const FMMOARPGCharacterAttribute& InCA, FString& OutString);
}
