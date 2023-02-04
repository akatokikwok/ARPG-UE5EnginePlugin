//Copyright (C) grb.2022.All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

// The file used to reflect gameplay tags is used. 此页文件是被反射生成而非人为编写
// 此文件禁止被多个文件include,否则会造成链接错误.


/*A static function that contains a list of types order*/
int32 GetGamePlayTagOrderIndex(const FName &InTag)
{
	static TArray<FString> GameplayTagsOrder = 
	{
		"Player.Skill.Dodge",
		"Player.Skill.Sprint",
		"Player.State.Die",
		"Player.State.Hit",
		"Player.Buff.Damage",
		"Player.Skill.WhirlwindBlow",
		"Player.Skill.SprintWhirlwindBlow",
		"Player.Skill.EscapeToKill",
		"Player.Skill.StreamBlade",
		"Player.Cooldown.Skill",
		"Player.Skill.PickFly",
		"Player.Skill.Spike",
		"Player.Skill.SprintPickFly",
		"Player.Attack.ComboLinkage.Air",
		"Player.Attack.ComboLinkage.Ground",
	};

	return GameplayTagsOrder.Find(InTag.ToString());
}

/*Get the number of enumerations of the current order*/
int32 GetOrderMatchingEnumIndex(int32 InGameplayTagsOrderIndex)
{
	return FMath::CeilToInt((float)InGameplayTagsOrderIndex / 63.f);
}

/* Enumerations can be generated dynamically from gameplaytag. */
enum class EGamePlayTags0 : uint64
{
    Player_Skill_Dodge = 1,
    Player_Skill_Sprint = 2,
    Player_State_Die = 4,
    Player_State_Hit = 8,
    Player_Buff_Damage = 16,
    Player_Skill_WhirlwindBlow = 32,
    Player_Skill_SprintWhirlwindBlow = 64,
    Player_Skill_EscapeToKill = 128,
    Player_Skill_StreamBlade = 256,
    Player_Cooldown_Skill = 512,
    Player_Skill_PickFly = 1024,
    Player_Skill_Spike = 2048,
    Player_Skill_SprintPickFly = 4096,
    Player_Attack_ComboLinkage_Air = 8192,
    Player_Attack_ComboLinkage_Ground = 16384,
};

/* Convert GamplayTag tag to enumeration */
EGamePlayTags0  NameToEGamePlayTags0(const FName &InName)
{
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
	} else
	if (InName == TEXT("Player.Skill.SprintWhirlwindBlow")) {
		 return EGamePlayTags0::Player_Skill_SprintWhirlwindBlow;
	} else
	if (InName == TEXT("Player.Skill.EscapeToKill")) {
		 return EGamePlayTags0::Player_Skill_EscapeToKill;
	} else
	if (InName == TEXT("Player.Skill.StreamBlade")) {
		 return EGamePlayTags0::Player_Skill_StreamBlade;
	} else
	if (InName == TEXT("Player.Cooldown.Skill")) {
		 return EGamePlayTags0::Player_Cooldown_Skill;
	} else
	if (InName == TEXT("Player.Skill.PickFly")) {
		 return EGamePlayTags0::Player_Skill_PickFly;
	} else
	if (InName == TEXT("Player.Skill.Spike")) {
		 return EGamePlayTags0::Player_Skill_Spike;
	} else
	if (InName == TEXT("Player.Skill.SprintPickFly")) {
		 return EGamePlayTags0::Player_Skill_SprintPickFly;
	} else
	if (InName == TEXT("Player.Attack.ComboLinkage.Air")) {
		 return EGamePlayTags0::Player_Attack_ComboLinkage_Air;
	} else
	if (InName == TEXT("Player.Attack.ComboLinkage.Ground")) {
		 return EGamePlayTags0::Player_Attack_ComboLinkage_Ground;
	} 

    return (EGamePlayTags0)0llu;
}

/* Convert enumeration to gameplaytag. */
FString  EGamePlayTags0ToName(const EGamePlayTags0& InTag)
{
	switch(InTag)
	{
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

		case EGamePlayTags0::Player_Skill_SprintWhirlwindBlow:
		 return TEXT("Player.Skill.SprintWhirlwindBlow");

		case EGamePlayTags0::Player_Skill_EscapeToKill:
		 return TEXT("Player.Skill.EscapeToKill");

		case EGamePlayTags0::Player_Skill_StreamBlade:
		 return TEXT("Player.Skill.StreamBlade");

		case EGamePlayTags0::Player_Cooldown_Skill:
		 return TEXT("Player.Cooldown.Skill");

		case EGamePlayTags0::Player_Skill_PickFly:
		 return TEXT("Player.Skill.PickFly");

		case EGamePlayTags0::Player_Skill_Spike:
		 return TEXT("Player.Skill.Spike");

		case EGamePlayTags0::Player_Skill_SprintPickFly:
		 return TEXT("Player.Skill.SprintPickFly");

		case EGamePlayTags0::Player_Attack_ComboLinkage_Air:
		 return TEXT("Player.Attack.ComboLinkage.Air");

		case EGamePlayTags0::Player_Attack_ComboLinkage_Ground:
		 return TEXT("Player.Attack.ComboLinkage.Ground");

	}

	return FString();
}

/* Convert a composite enumeration to the corresponding label. */
void  NameToEGamePlayTags0s(const FName &InName,TArray<FName> &OutName)
{
	uint64 Tags = FCString::Strtoui64(*InName.ToString(),NULL,10);
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
	if(Tags & (int32)EGamePlayTags0::Player_Skill_SprintWhirlwindBlow)
	{
		OutName.AddUnique(TEXT("Player.Skill.SprintWhirlwindBlow"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_EscapeToKill)
	{
		OutName.AddUnique(TEXT("Player.Skill.EscapeToKill"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_StreamBlade)
	{
		OutName.AddUnique(TEXT("Player.Skill.StreamBlade"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Cooldown_Skill)
	{
		OutName.AddUnique(TEXT("Player.Cooldown.Skill"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_PickFly)
	{
		OutName.AddUnique(TEXT("Player.Skill.PickFly"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_Spike)
	{
		OutName.AddUnique(TEXT("Player.Skill.Spike"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_SprintPickFly)
	{
		OutName.AddUnique(TEXT("Player.Skill.SprintPickFly"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Attack_ComboLinkage_Air)
	{
		OutName.AddUnique(TEXT("Player.Attack.ComboLinkage.Air"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Attack_ComboLinkage_Ground)
	{
		OutName.AddUnique(TEXT("Player.Attack.ComboLinkage.Ground"));
	}
}

/* Convert multiple enumerations into one label for storage. */
FName  EGamePlayTags0sToName(const TArray<FName> &InName)
{
    uint64 Tags = 0llu;
	for(auto &Tmp : InName)
	{
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
		} else
		if(Tmp == FName(TEXT("Player.Skill.SprintWhirlwindBlow")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_SprintWhirlwindBlow;
		} else
		if(Tmp == FName(TEXT("Player.Skill.EscapeToKill")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_EscapeToKill;
		} else
		if(Tmp == FName(TEXT("Player.Skill.StreamBlade")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_StreamBlade;
		} else
		if(Tmp == FName(TEXT("Player.Cooldown.Skill")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Cooldown_Skill;
		} else
		if(Tmp == FName(TEXT("Player.Skill.PickFly")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_PickFly;
		} else
		if(Tmp == FName(TEXT("Player.Skill.Spike")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_Spike;
		} else
		if(Tmp == FName(TEXT("Player.Skill.SprintPickFly")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Skill_SprintPickFly;
		} else
		if(Tmp == FName(TEXT("Player.Attack.ComboLinkage.Air")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Attack_ComboLinkage_Air;
		} else
		if(Tmp == FName(TEXT("Player.Attack.ComboLinkage.Ground")))
		{
			Tags |= (uint64)EGamePlayTags0::Player_Attack_ComboLinkage_Ground;
		} 
	}

	return *FString::Printf(TEXT("%llu"), Tags);
}

//////////////////////////Main/////////////////////////////
/*Convert BitAndOrder to bytes and TagName*/
bool SingleBitAndOrderToTagName(const int32 InEnumIndex, const uint64 EnumValue,FName &TagName)
{
	switch (InEnumIndex)
	{
	case 0:
		TagName = *EGamePlayTags0ToName((EGamePlayTags0)EnumValue);
		return true;
	}

	return false;
}

/*Convert FName to bytes and order*/
bool SingleTagNameToBitAndOrder(const FName& TageName, int32& InEnumIndex, uint64& EnumValue)
{
	int32 InOrder = GetGamePlayTagOrderIndex(TageName);
	if (InOrder != INDEX_NONE)
	{
		InEnumIndex = GetOrderMatchingEnumIndex(InOrder) - 1;
		switch (InEnumIndex)
		{
		case 0:
			EnumValue = (uint64)NameToEGamePlayTags0(TageName);
			return true;
		}
	}

	return true;
}

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
