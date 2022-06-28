//Copyright (C) grb.2022.All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

// The file used to reflect gameplay tags is used. 此页文件是被反射生成而非人为编写
// 此文件禁止被多个文件include,否则会造成链接错误.

/*Enumerations can be generated dynamically from gameplaytag*/
enum class EGamePlayTags0 :uint32
{
    Player_Attack_ComboLinkage = 1,
    Player_Skill_Dodge = 2,
    Player_Skill_Sprint = 4,
    Player_Skill_Sprint2 = 8,
    Player_State_Die = 10,
    Player_State_Hit = 20,
};

/*Convert GamplayTag tag to enumeration*/
EGamePlayTags0  NameToEGamePlayTags0(const FName &InName)
{
	if (InName == TEXT("Player_Attack_ComboLinkage")) {
		 return EGamePlayTags0::Player_Attack_ComboLinkage;
	} else
	if (InName == TEXT("Player_Skill_Dodge")) {
		 return EGamePlayTags0::Player_Skill_Dodge;
	} else
	if (InName == TEXT("Player_Skill_Sprint")) {
		 return EGamePlayTags0::Player_Skill_Sprint;
	} else
	if (InName == TEXT("Player_Skill_Sprint2")) {
		 return EGamePlayTags0::Player_Skill_Sprint2;
	} else
	if (InName == TEXT("Player_State_Die")) {
		 return EGamePlayTags0::Player_State_Die;
	} else
	if (InName == TEXT("Player_State_Hit")) {
		 return EGamePlayTags0::Player_State_Hit;
	} 

  return (EGamePlayTags0)0;
}

/*Convert enumeration to gameplaytag.*/
FString  EGamePlayTags0ToName(const EGamePlayTags0& InTag)
{
	switch(InTag)
	{
		case EGamePlayTags0::Player_Attack_ComboLinkage:
		 return TEXT("Player_Attack_ComboLinkage");

		case EGamePlayTags0::Player_Skill_Dodge:
		 return TEXT("Player_Skill_Dodge");

		case EGamePlayTags0::Player_Skill_Sprint:
		 return TEXT("Player_Skill_Sprint");

		case EGamePlayTags0::Player_Skill_Sprint2:
		 return TEXT("Player_Skill_Sprint2");

		case EGamePlayTags0::Player_State_Die:
		 return TEXT("Player_State_Die");

		case EGamePlayTags0::Player_State_Hit:
		 return TEXT("Player_State_Hit");

	}

	return FString();
}

/*Convert a composite enumeration to the corresponding label.*/
void  NameToEGamePlayTags0s(const FName &InName,TArray<FName> &OutName)
{
	int32 Tags = FCString::Atoi(*InName.ToString());
	if(Tags & (int32)EGamePlayTags0::Player_Attack_ComboLinkage)
	{
		OutName.AddUnique(TEXT("Player_Attack_ComboLinkage"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_Dodge)
	{
		OutName.AddUnique(TEXT("Player_Skill_Dodge"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_Sprint)
	{
		OutName.AddUnique(TEXT("Player_Skill_Sprint"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_Skill_Sprint2)
	{
		OutName.AddUnique(TEXT("Player_Skill_Sprint2"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_State_Die)
	{
		OutName.AddUnique(TEXT("Player_State_Die"));
	}
	if(Tags & (int32)EGamePlayTags0::Player_State_Hit)
	{
		OutName.AddUnique(TEXT("Player_State_Hit"));
	}
}

/*Convert multiple enumerations into one label for storage.*/
FName  EGamePlayTags0sToName(const TArray<FName> &InName)
{
 int32 Tags = 0;
	for(auto &Tmp : InName)
	{
		if(Tmp == FName(TEXT("Player_Attack_ComboLinkage")))
		{
			Tags |= (int32)EGamePlayTags0::Player_Attack_ComboLinkage;
		} else
		if(Tmp == FName(TEXT("Player_Skill_Dodge")))
		{
			Tags |= (int32)EGamePlayTags0::Player_Skill_Dodge;
		} else
		if(Tmp == FName(TEXT("Player_Skill_Sprint")))
		{
			Tags |= (int32)EGamePlayTags0::Player_Skill_Sprint;
		} else
		if(Tmp == FName(TEXT("Player_Skill_Sprint2")))
		{
			Tags |= (int32)EGamePlayTags0::Player_Skill_Sprint2;
		} else
		if(Tmp == FName(TEXT("Player_State_Die")))
		{
			Tags |= (int32)EGamePlayTags0::Player_State_Die;
		} else
		if(Tmp == FName(TEXT("Player_State_Hit")))
		{
			Tags |= (int32)EGamePlayTags0::Player_State_Hit;
		} 
	}

	return *FString::Printf(TEXT("%llu"),Tags);
}

//////////////////////////Main/////////////////////////////
/*Convert the server's sequence to a label.*/
void  AnalysisArrayNameToGamePlayTags(const TArray<FName> &InNames,TArray<FName> &OutNames)
{
	for(auto &Tmp : InNames)
	{
		NameToEGamePlayTags0s(Tmp,OutNames);
	}
}

/*Convert multiple tags to a sequence stored by the server.*/
void  AnalysisGamePlayTagsToArrayName(const TArray<FName> &InNames,TArray<FName> &OutNames)
{
	FName TagName = EGamePlayTags0sToName(InNames);
	if(TagName != NAME_None)
	{
		OutNames.AddUnique(TagName);
	}
}
