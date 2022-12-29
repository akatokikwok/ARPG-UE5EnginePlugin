#include "MMOARPGType.h"
#include "Json.h"
#include "Serialization/JsonSerializer.h"

// 私有工具方法: Json提取出人物属性集的各个字段.
void JsonObjectToAttributeData(
	const FString& InJsonObjectName,
	TSharedPtr<FJsonObject> ReadRoot,
	FMMOARPGAttributeData& InAttributeData)
{
	if (TSharedPtr<FJsonObject> SubJsonObject = ReadRoot->GetObjectField(InJsonObjectName)) {
		InAttributeData.BaseValue = SubJsonObject->GetNumberField(TEXT("BaseValue"));
		InAttributeData.CurrentValue = SubJsonObject->GetNumberField(TEXT("CurrentValue"));
	}
}

// 同名重载. 私有工具方法: Json提取出人物属性集的各个字段.
void JsonObjectToAttributeData(
	const FString& InJsonObjectName,
	TSharedPtr<FJsonObject> InObjectJson,
	TArray<FName>& Res_AttributeData)
{
	// 1|2|4|5 拆解成12345
	NetDataAnalysis::AnalysisToArrayName(InObjectJson->GetStringField(InJsonObjectName), Res_AttributeData);
}

// 私有工具方法: 人物属性集的各个字段压入JSON
void AttributeDataToJsonObject(
	const FString& InJsonObjectName,
	TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter,
	const FMMOARPGAttributeData& InAttributeData)
{
	JsonWriter->WriteObjectStart(InJsonObjectName);
	{
		JsonWriter->WriteValue(TEXT("BaseValue"), InAttributeData.BaseValue);
		JsonWriter->WriteValue(TEXT("CurrentValue"), InAttributeData.CurrentValue);
	}
	JsonWriter->WriteObjectEnd();
}

// 同名重载. 私有工具方法: 人物属性集的各个字段压入JSON
void AttributeDataToJsonObject(
	const FString& InJsonObjectName,
	TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter,
	const TArray<FName>& InAttributeData)
{
	FString ValueString;
	for (auto& Tmp : InAttributeData) {
		ValueString += Tmp.ToString() + TEXT("|");
	}
	ValueString.RemoveFromEnd(TEXT("|"));

	JsonWriter->WriteValue(InJsonObjectName, ValueString);
}

namespace NetDataAnalysis
{
	// 通用方法, 把类似1|2|3 拆解成123
	void AnalysisToArrayName(const FString& InRowString, TArray<FName>& OutInfo)
	{
		TArray<FString> InfoArray;
		InRowString.ParseIntoArray(InfoArray, TEXT("|"));

		for (auto& Tmp : InfoArray) {
			OutInfo.Add(*Tmp);
		}
	}

	/** 将有值的用户数据 存进 Json String. */
	void UserDataToString(const FMMOARPGUserData& InUserData, FString& OutString)
	{
		OutString.Empty();// 先清一下.

		// 构建一个写入器.
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = 
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		Writer->WriteObjectStart();
		{
			Writer->WriteValue(TEXT("ID"), InUserData.ID);
			Writer->WriteValue(TEXT("Account"), *InUserData.Account);
			Writer->WriteValue(TEXT("EMail"), *InUserData.EMail);
			Writer->WriteValue(TEXT("Name"), *InUserData.Name);
			Writer->WriteValue(TEXT("HeadportraitURL"), *InUserData.HeadportraitURL);
		}
		Writer->WriteObjectEnd();
		Writer->Close();
	}

	/** 从有值的Json String 取出 用户数据.*/
	void StringToUserData(const FString& JsonString, FMMOARPGUserData& OutUserData)
	{
		/**
		 * 所谓json就是类似于这种
		 * "ID":1,
		 * "Accout":"grb",
		 * "Name":"grb",
		 */

		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		TSharedPtr<FJsonValue> ReadRoot;
		// 反序列化.
		if (FJsonSerializer::Deserialize(Reader, ReadRoot)) {
			// 
			if (TSharedPtr<FJsonObject> InJsonObj = ReadRoot->AsObject()) {
				OutUserData.ID = InJsonObj->GetIntegerField(TEXT("ID"));
				OutUserData.Account = InJsonObj->GetStringField(TEXT("Account"));
				OutUserData.EMail = InJsonObj->GetStringField(TEXT("EMail"));
				OutUserData.Name = InJsonObj->GetStringField(TEXT("Name"));
				OutUserData.HeadportraitURL = InJsonObj->GetStringField(TEXT("HeadportraitURL"));
			}
		}

	}

	/************************************************************************/

	void CharacterAppearancesToString(const FCharacterAppearances& InCAs, FString& OutString)
	{
		OutString.Empty();

		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteArrayStart();
		for (auto& Tmp : InCAs) {
			JsonWriter->WriteObjectStart();

			JsonWriter->WriteValue(TEXT("Name"), Tmp.Name);
			JsonWriter->WriteValue(TEXT("Date"), Tmp.Date);
			JsonWriter->WriteValue(TEXT("Lv"), Tmp.Lv);
 			JsonWriter->WriteValue(TEXT("SlotPosition"), Tmp.SlotPosition);
			JsonWriter->WriteValue(TEXT("LegSize"), Tmp.LegSize);
			JsonWriter->WriteValue(TEXT("WaistSize"), Tmp.WaistSize);
			JsonWriter->WriteValue(TEXT("ArmSize"), Tmp.ArmSize);
			JsonWriter->WriteValue(TEXT("HeadSize"), Tmp.HeadSize);
			JsonWriter->WriteValue(TEXT("ChestSize"), Tmp.ChestSize);

			JsonWriter->WriteObjectEnd();
		}
		JsonWriter->WriteArrayEnd();
		JsonWriter->Close();
	}

	void CharacterAppearancesToString(const FMMOARPGCharacterAppearance& InCA, FString& OutString)
	{
		OutString.Empty();

		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteObjectStart();
		{
			JsonWriter->WriteValue(TEXT("Name"), InCA.Name);
			JsonWriter->WriteValue(TEXT("Date"), InCA.Date);
			JsonWriter->WriteValue(TEXT("Lv"), InCA.Lv);
 			JsonWriter->WriteValue(TEXT("SlotPosition"), InCA.SlotPosition);
 			JsonWriter->WriteValue(TEXT("LegSize"), InCA.LegSize);
 			JsonWriter->WriteValue(TEXT("WaistSize"), InCA.WaistSize);
 			JsonWriter->WriteValue(TEXT("ArmSize"), InCA.ArmSize);
			JsonWriter->WriteValue(TEXT("HeadSize"), InCA.HeadSize);
			JsonWriter->WriteValue(TEXT("ChestSize"), InCA.ChestSize);

		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void StringToCharacterAppearances(const FString& InJsonString, FCharacterAppearances& OutCAs)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InJsonString);
		TArray<TSharedPtr<FJsonValue>> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			for (auto& Tmp : ReadRoot) {
				OutCAs.Add(FMMOARPGCharacterAppearance());
				FMMOARPGCharacterAppearance& InLast = OutCAs.Last();
				if (TSharedPtr<FJsonObject> InJsonObject = Tmp->AsObject()) {
					InLast.Name = InJsonObject->GetStringField(TEXT("Name"));
					InLast.Date = InJsonObject->GetStringField(TEXT("Date"));
					InLast.Lv = InJsonObject->GetIntegerField(TEXT("Lv"));
 					InLast.SlotPosition = InJsonObject->GetIntegerField(TEXT("SlotPosition"));
 					InLast.LegSize = InJsonObject->GetNumberField(TEXT("LegSize"));
 					InLast.WaistSize = InJsonObject->GetNumberField(TEXT("WaistSize"));
 					InLast.ArmSize = InJsonObject->GetNumberField(TEXT("ArmSize"));
					InLast.HeadSize = InJsonObject->GetNumberField(TEXT("HeadSize"));
					InLast.ChestSize = InJsonObject->GetNumberField(TEXT("ChestSize"));
				}
			}
		}
	}

	void StringToCharacterAppearances(const FString& InJsonString, FMMOARPGCharacterAppearance& OutCA)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InJsonString);
		TSharedPtr<FJsonObject> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			OutCA.Name = ReadRoot->GetStringField(TEXT("Name"));
			OutCA.Date = ReadRoot->GetStringField(TEXT("Date"));
			OutCA.Lv = ReadRoot->GetIntegerField(TEXT("Lv"));
 			OutCA.SlotPosition = ReadRoot->GetIntegerField(TEXT("SlotPosition"));
 			OutCA.LegSize = ReadRoot->GetNumberField(TEXT("LegSize"));
 			OutCA.WaistSize = ReadRoot->GetNumberField(TEXT("WaistSize"));
 			OutCA.ArmSize = ReadRoot->GetNumberField(TEXT("ArmSize"));
			OutCA.HeadSize = ReadRoot->GetNumberField(TEXT("HeadSize"));
			OutCA.ChestSize = ReadRoot->GetNumberField(TEXT("ChestSize"));
		}
	}

	/** 从Json里解析出人物GAS属性集 */
	bool StringToMMOARPGCharacterAttribute(const FString& InJson, FMMOARPGCharacterAttribute& Out_CA)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InJson);
		TSharedPtr<FJsonObject> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			JsonObjectToAttributeData(TEXT("Level"), ReadRoot, Out_CA.Level);
			JsonObjectToAttributeData(TEXT("Health"), ReadRoot, Out_CA.Health);
			JsonObjectToAttributeData(TEXT("MaxHealth"), ReadRoot, Out_CA.MaxHealth);
			JsonObjectToAttributeData(TEXT("Mana"), ReadRoot, Out_CA.Mana);
			JsonObjectToAttributeData(TEXT("MaxMana"), ReadRoot, Out_CA.MaxMana);
			JsonObjectToAttributeData(TEXT("PhysicsAttack"), ReadRoot, Out_CA.PhysicsAttack);
			JsonObjectToAttributeData(TEXT("MagicAttack"), ReadRoot, Out_CA.MagicAttack);
			JsonObjectToAttributeData(TEXT("PhysicsDefense"), ReadRoot, Out_CA.PhysicsDefense);
			JsonObjectToAttributeData(TEXT("MagicDefense"), ReadRoot, Out_CA.MagicDefense);
			JsonObjectToAttributeData(TEXT("AttackRange"), ReadRoot, Out_CA.AttackRange);
			JsonObjectToAttributeData(TEXT("EmpiricalValue"), ReadRoot, Out_CA.EmpiricalValue);
			JsonObjectToAttributeData(TEXT("MaxEmpiricalValue"), ReadRoot, Out_CA.MaxEmpiricalValue);

			JsonObjectToAttributeData(TEXT("ComboAttack"), ReadRoot, Out_CA.ComboAttack.Slots);
			JsonObjectToAttributeData(TEXT("Skill"), ReadRoot, Out_CA.Skill.Slots);
			JsonObjectToAttributeData(TEXT("Limbs"), ReadRoot, Out_CA.Limbs.Slots);

			// 读取出装配好的技能
			Out_CA.SkillAssemblyString = ReadRoot->GetStringField(TEXT("SkillAssemblyString"));
			return true;
		}

		return false;
	}

	/** 人物GAS属性集压入Json */
	void MMOARPGCharacterAttributeToString(const FMMOARPGCharacterAttribute& InCA, FString& OutString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteObjectStart();
		{
			AttributeDataToJsonObject(TEXT("Level"), JsonWriter, InCA.Level);
			AttributeDataToJsonObject(TEXT("Health"), JsonWriter, InCA.Health);
			AttributeDataToJsonObject(TEXT("MaxHealth"), JsonWriter, InCA.MaxHealth);
			AttributeDataToJsonObject(TEXT("Mana"), JsonWriter, InCA.Mana);
			AttributeDataToJsonObject(TEXT("MaxMana"), JsonWriter, InCA.MaxMana);
			AttributeDataToJsonObject(TEXT("PhysicsAttack"), JsonWriter, InCA.PhysicsAttack);
			AttributeDataToJsonObject(TEXT("MagicAttack"), JsonWriter, InCA.MagicAttack);
			AttributeDataToJsonObject(TEXT("PhysicsDefense"), JsonWriter, InCA.PhysicsDefense);
			AttributeDataToJsonObject(TEXT("MagicDefense"), JsonWriter, InCA.MagicDefense);
			AttributeDataToJsonObject(TEXT("AttackRange"), JsonWriter, InCA.AttackRange);
			AttributeDataToJsonObject(TEXT("EmpiricalValue"), JsonWriter, InCA.EmpiricalValue);
			AttributeDataToJsonObject(TEXT("MaxEmpiricalValue"), JsonWriter, InCA.MaxEmpiricalValue);

			AttributeDataToJsonObject(TEXT("ComboAttack"), JsonWriter, InCA.ComboAttack.Slots);
			AttributeDataToJsonObject(TEXT("Skill"), JsonWriter, InCA.Skill.Slots);
			AttributeDataToJsonObject(TEXT("Limbs"), JsonWriter, InCA.Limbs.Slots);

			// 写入装配好的技能.
			JsonWriter->WriteValue(TEXT("SkillAssemblyString"), InCA.SkillAssemblyString);
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	/** 把JSON语句 解析成<玩家, 属性集> */
	bool StringToMMOARPGCharacterAttribute(const FString& InJson_GASAT, TMap<int32, FMMOARPGCharacterAttribute>& InATMap)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InJson_GASAT);
		TArray<TSharedPtr<FJsonValue>> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			for (auto& Tmp : ReadRoot) {
				if (TSharedPtr<FJsonObject> InJsonObject = Tmp->AsObject()) {
					int32 ID = InJsonObject->GetIntegerField(TEXT("ID"));
					FString CharacterAttributeJson = InJsonObject->GetStringField(TEXT("CA"));

					InATMap.Add(ID, FMMOARPGCharacterAttribute());
					StringToMMOARPGCharacterAttribute(CharacterAttributeJson, InATMap[ID]);
				}
			}
			return true;
		}

		return false;
	}

	/** 把<玩家, 属性集> 压缩成JSON语句 */
	void MMOARPGCharacterAttributeToString(const TMap<int32, FMMOARPGCharacterAttribute>& InATMap, FString& OutJson_AT)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJson_AT);

		JsonWriter->WriteArrayStart();
		for (auto& Tmp : InATMap) {/* 对于属性集map的每一组pair. */
			JsonWriter->WriteObjectStart();
			{
				JsonWriter->WriteValue(TEXT("ID"), Tmp.Key);

				FString CharacterAttributeString;
				MMOARPGCharacterAttributeToString(Tmp.Value, CharacterAttributeString);

				JsonWriter->WriteValue(TEXT("CA"), CharacterAttributeString);

			}
			JsonWriter->WriteObjectEnd();
		}
		JsonWriter->WriteArrayEnd();
		JsonWriter->Close();
	}

	/** 把JSON语句 解析成 技能槽 */
	bool StringToMMOARPGAttributeSlot(const FString& IntString, FMMOARPGAttributeSlot& OutAS)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(IntString);
		TArray<TSharedPtr<FJsonValue>> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			for (auto& Tmp : ReadRoot) {
				OutAS.Slots.Add(*Tmp->AsString());				
			}
			return true;
		}
		return false;
	}

	/** 把技能槽 压缩成JSON语句 */
	void MMOARPGAttributeSlotToString(const FMMOARPGAttributeSlot& InAS, FString& OutString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteArrayStart();
		for (auto& Tmp : InAS.Slots) {
			JsonWriter->WriteValue(Tmp.ToString());
		}
		JsonWriter->WriteArrayEnd();
		JsonWriter->Close();
	}
}


void FMMOARPGCharacterAppearance::Reset()
{
	Lv = INDEX_NONE;
	SlotPosition = INDEX_NONE;
	LegSize = 0.0f;
	WaistSize = 0.0f;
	ArmSize = 0.0f;

	Name.Empty();
	Date.Empty();
}

void FMMOARPGUserData::Reset()
{
	ID = INDEX_NONE;
	Account.Empty();
	EMail.Empty();
	Name.Empty();
	HeadportraitURL.Empty();
}

void FMMOARPGPlayerRegistInfo::Reset()
{
	UserInfo.Reset();
	CAInfo.Reset();
}

bool FMMOARPGPlayerRegistInfo::IsVaild()
{
	return UserInfo.ID != INDEX_NONE;
}
// 集体清除所有GA技能槽名字
void FMMOARPGCharacterAttribute::Clear()
{
	Limbs.Slots.Empty();
	Skill.Slots.Empty();
	ComboAttack.Slots.Empty();
}

// 把一个字符串叠上指定的分隔符
FString FMMOARPGAttributeSlot::ToString() const
{
	TCHAR* InSplitSymbol = TEXT("|");// 暂定1个分隔符"|"

	FString String;
	for (auto& Tmp : Slots) {
		String += Tmp.ToString() + InSplitSymbol;
	}
	String.RemoveFromEnd(InSplitSymbol);

	// 意外保护,防止空字符串
	if (String.IsEmpty()) {
		String = TEXT("0");
	}

	return String;
}


#pragma region 弃用接口
// 为ComboAttack字段拼接分隔符 |
//FString FMMOARPGCharacterAttribute::ComboAttackToString() const
//{
//	return ArrayNameToString(ComboAttack, TEXT("|"));
//}

// 为skill字段拼接分隔符 |
//FString FMMOARPGCharacterAttribute::SkillToString() const
//{
//	return ArrayNameToString(Skill, TEXT("|"));
//}

// 为Limbs字段拼接分隔符 |
//FString FMMOARPGCharacterAttribute::LimbsToString() const
//{
//	return ArrayNameToString(Limbs, TEXT("|"));
//}

// 把一个字符串叠上指定的分隔符
//FString FMMOARPGCharacterAttribute::ArrayNameToString(const TArray<FName>& Names, const TCHAR* InSplitSymbol) const
//{
//	FString String;
//	for (auto& Tmp : Names) {
//		String += Tmp.ToString() + InSplitSymbol;
//	}
//	String.RemoveFromEnd(InSplitSymbol);
//
//	// 意外保护,防止空字符串
//	if (String.IsEmpty()) {
//		String = TEXT("0");
//	}
//
//	return String;
//}
#pragma endregion 弃用接口