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

namespace NetDataAnalysis
{
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
		}
	}

	/** 从Json里解析出人物GAS属性集 */
	bool StringToMMOARPGCharacterAttribute(const FString& InJson, FMMOARPGCharacterAttribute& Out_CA)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InJson);
		TSharedPtr<FJsonObject> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			JsonObjectToAttributeData(TEXT("Health"), ReadRoot, Out_CA.Health);
			JsonObjectToAttributeData(TEXT("MaxHealth"), ReadRoot, Out_CA.MaxHealth);
			JsonObjectToAttributeData(TEXT("Mana"), ReadRoot, Out_CA.Mana);
			JsonObjectToAttributeData(TEXT("MaxMana"), ReadRoot, Out_CA.MaxMana);

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
			AttributeDataToJsonObject(TEXT("Health"), JsonWriter, InCA.Health);
			AttributeDataToJsonObject(TEXT("MaxHealth"), JsonWriter, InCA.MaxHealth);
			AttributeDataToJsonObject(TEXT("Mana"), JsonWriter, InCA.Mana);
			AttributeDataToJsonObject(TEXT("MaxMana"), JsonWriter, InCA.MaxMana);
		}

		JsonWriter->WriteObjectEnd();
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
