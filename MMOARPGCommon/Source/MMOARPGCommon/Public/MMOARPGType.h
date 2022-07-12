#pragma once
#include "CoreMinimal.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.generated.h"

/**
 * 中心服务器链接者枚举.
 */
enum ECentralServerLinkType
{
	GAME_PLAYER_LINK,
	GAME_DEDICATED_SERVER_LINK
};

// 登录状态(如账户错误,密码错误,服务器错误,成功之类.)
enum ELoginType
{
	LOGIN_DB_SERVER_ERROR,
	LOGIN_SUCCESS,
	LOGIN_ACCOUNT_WRONG,
	LOGIN_PASSWORD_WRONG,
};

/**
 * 注册类型枚举.
 */
enum ERegistrationType
{
	ACCOUNT_AND_EMAIL_REPETITION_ERROR,// 名字重复
	PLAYER_REGISTRATION_SUCCESS,// 注册成功
	SERVER_BUG_WRONG,
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
		, HeadSize(0.f)
		, ChestSize(0.f)
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
	UPROPERTY()
		float HeadSize;
	UPROPERTY()
		float ChestSize;
};

FORCEINLINE bool operator==(const FMMOARPGCharacterAppearance& Lhs, const FMMOARPGCharacterAppearance& Rhs)
{
	return Lhs.SlotPosition == Rhs.SlotPosition;
}

typedef TArray<FMMOARPGCharacterAppearance> FCharacterAppearances;// 一个玩家可能会有多个角色.


/**
 * MMOARPG项目的 AttributeData (仿自虚幻的FGameplayAttributeData.)
 * 具备basevalue 和 currentvalue
 * 定义在MMOARPGType.h
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
		FMMOARPGAttributeData Level;

	UPROPERTY()
		FMMOARPGAttributeData Health;

	UPROPERTY()
		FMMOARPGAttributeData MaxHealth;

	UPROPERTY()
		FMMOARPGAttributeData Mana;

	UPROPERTY()
		FMMOARPGAttributeData MaxMana;

	UPROPERTY()
		FMMOARPGAttributeData PhysicsAttack;

	UPROPERTY()
		FMMOARPGAttributeData MagicAttack;

	UPROPERTY()
		FMMOARPGAttributeData PhysicsDefense;

	UPROPERTY()
		FMMOARPGAttributeData MagicDefense;

	UPROPERTY()
		FMMOARPGAttributeData AttackRange;

	// 一组连招GA名字
	UPROPERTY()
		TArray<FName> ComboAttack;// 一组连招GA名字.

	// 一组技能GA名字
	UPROPERTY()
		TArray<FName> Skill;// 一组技能GA名字.

	// 一组肢体行为GA名字
	UPROPERTY()
		TArray<FName> Limbs;// 一组肢体行为GA名字.
public:
	// 为ComboAttack字段拼接分隔符 |
	FString ComboAttackToString() const;
	// 为skill字段拼接分隔符 |
	FString SkillToString() const;
	// 为Limbs字段拼接分隔符 |
	FString LimbsToString() const;
private:
	// 把一个字符串叠上指定的分隔符
	FString	ArrayNameToString(const TArray<FName>& Names, const TCHAR* InSplitSymbol) const;
};

/**
 * DS信息数据结构(由1个CS所掌控)
 */
USTRUCT(BlueprintType)
struct MMOARPGCOMMON_API FMMOARPGDicatedServerInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		int32 ID;
};

/**
 * 玩家注册信息.
 */
struct MMOARPGCOMMON_API FMMOARPGPlayerRegistInfo
{
public:
	// 用户数据
	FMMOARPGUserData UserInfo;

	// 捏脸数据
	FMMOARPGCharacterAppearance CAInfo;

	// 角色属性集(GAS)
	TMap<int32, FMMOARPGCharacterAttribute> CharacterAttributes;
public:
	void Reset();
	bool IsVaild();

};

/**
 * Json-用户数据 解析的几个API.
 */
namespace NetDataAnalysis
{
	// 通用方法, 把类似1|2|3 拆解成123
	void MMOARPGCOMMON_API AnalysisToArrayName(const FString& InRowString, TArray<FName>& OutInfo);

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

	/** 把JSON语句 解析成<玩家, 属性集> */
	bool MMOARPGCOMMON_API StringToMMOARPGCharacterAttribute(const FString& OutString, TMap<int32, FMMOARPGCharacterAttribute>& InCA);
	/** 把<玩家, 属性集> 压缩成JSON语句 */
	void MMOARPGCOMMON_API MMOARPGCharacterAttributeToString(const TMap<int32, FMMOARPGCharacterAttribute>& InCA, FString& OutString);

}