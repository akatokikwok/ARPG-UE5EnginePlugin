//Copyright (C) grb.2022.All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

// The file used to reflect gameplay tags is used. 此页文件是被反射生成而非人为编写
// 此文件禁止被多个文件include,否则会造成链接错误.

/* Enumerations can be generated dynamically from gameplaytag. */
enum class EGamePlayTags0 : uint64
{
    Player_Attack_ComboLinkage = 1,
    Player_Skill_Dodge = 2,
    Player_Skill_Sprint = 4,
    Player_State_Die = 8,
    Player_State_Hit = 16,
    Player_Buff_Damage = 32,
    Player_Skill_WhirlwindBlow = 64,
};

/* Convert GamplayTag tag to enumeration */
EGamePlayTags0  NameToEGamePlayTags0(const FName &InName)
{
	if (InName == TEXT("Player.Attack.ComboLinkage")) {
		 return EGamePlayTags0::Player_Attack_ComboLinkage;
	} else
	if (InName == TEXT("Player.Skill.Dodge")) {
		 return EGamePlayTags0::Player_Skill_Dodge;
	} else
	if (InName == TEXT("Player.Skill.Sprint")) {
		 return EGamePlayTags0::Player_Skill_Sprint;
	} else
	if (InName == TEXT("Player.State.Die")) {
		 return EGamePlayTags0::Player_State_Die;
	} else
	if (InName == TEXT("Player.State.Hit")) {
		 return EGamePlayTags0::Player_State_Hit;
	} else
	if (InName == TEXT("Player.Buff.Damage")) {
		 return EGamePlayTags0::Player_Buff_Damage;
	} else
	if (InName == TEXT("Player.Skill.WhirlwindBlow")) {
		 return EGamePlayTags0::Player_Skill_WhirlwindBlow;
	} 

    return (EGamePlayTags0)0llu;
}

/* Convert enumeration to gameplaytag. */
FString  EGamePlayTags0ToName(const EGamePlayTags0& InTag)
{
	switch(InTag)
	{
		case EGamePlayTags0::Player_Attack_ComboLinkage:
		 return TEXT("Player.Attack.ComboLinkage");

		case EGamePlayTags0::Player_Skill_Dodge:
		 return TEXT("Player.Skill.Dodge");

		case EGamePlayTags0::Player_Skill_Sprint:
		 return TEXT("Player.Skill.Sprint");

		case EGamePlayTags0::Player_State_Die:
		 return TEXT("Player.State.Die");

		case EGamePlayTags0::Player_State_Hit:
		 return TEXT("Player.State.Hit");

		case EGamePlayTags0::Player_Buff_Damage:
		 return TEXT("Player.Buff.Damage");

		case EGamePlayTags0::Player_Skill_WhirlwindBlow:
		 return TEXT("Player.Skill.WhirlwindBlow");

	}

	return FString();
}

/* Convert a composite enumeration to the corresponding label. */
void  NameToEGamePlayTags0s(const FName &InName,TArray<FName> &OutName)
{
	uint64 Tags = FCString::Strtoui64(*InName.ToString(),NULL,10);
	if(Tags & (int32)EGamePlayTags0::Player_Attack_ComboLinkage)
	{
		OutName.AddUnique(TEXT("Player.Attack.ComboLinkage"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_Dodge)
	{
		OutName.AddUnique(TEXT("Player.Skill.Dodge"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_Sprint)
	{
		OutName.AddUnique(TEXT("Player.Skill.Sprint"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_State_Die)
	{
		OutName.AddUnique(TEXT("Player.State.Die"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_State_Hit)
	{
		OutName.AddUnique(TEXT("Player.State.Hit"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Buff_Damage)
	{
		OutName.AddUnique(TEXT("Player.Buff.Damage"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_WhirlwindBlow)
	{
		OutName.AddUnique(TEXT("Player.Skill.WhirlwindBlow"));
	}
}

/* Convert multiple enumerations into one label for storage. */
FName  EGamePlayTags0sToName(const TArray<FName> &InName)
{
    uint64 Tags = 0llu;
	for(auto &Tmp : InName)
	{
		if(Tmp == FName(TEXT("Player.Attack.ComboLinkage")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Attack_ComboLinkage;
		} else
		if(Tmp == FName(TEXT("Player.Skill.Dodge")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_Dodge;
		} else
		if(Tmp == FName(TEXT("Player.Skill.Sprint")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_Sprint;
		} else
		if(Tmp == FName(TEXT("Player.State.Die")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_State_Die;
		} else
		if(Tmp == FName(TEXT("Player.State.Hit")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_State_Hit;
		} else
		if(Tmp == FName(TEXT("Player.Buff.Damage")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Buff_Damage;
		} else
		if(Tmp == FName(TEXT("Player.Skill.WhirlwindBlow")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_WhirlwindBlow;
		} 
	}

	return *FString::Printf(TEXT("%llu"), Tags);
}

//////////////////////////Main/////////////////////////////
/* Convert the server's sequence to a label. */
void  AnalysisArrayNameToGamePlayTags(const TArray<FName> &InNames,TArray<FName> &OutNames)
{
	for(auto &Tmp : InNames)
	{
		NameToEGamePlayTags0s(Tmp,OutNames);
	}
}

/* Convert multiple tags to a sequence stored by the server. */
void  AnalysisGamePlayTagsToArrayName(const TArray<FName> &InNames,TArray<FName> &OutNames)
{
	FName TagName = EGamePlayTags0sToName(InNames);
	if(TagName != NAME_None)
	{
		OutNames.AddUnique(TagName);
	}
}
