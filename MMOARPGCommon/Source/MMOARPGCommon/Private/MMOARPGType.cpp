#include "MMOARPGType.h"
#include "Json.h"
#include "Serialization/JsonSerializer.h"

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
// 			JsonWriter->WriteValue(TEXT("LegSize"), Tmp.LegSize);
// 			JsonWriter->WriteValue(TEXT("WaistSize"), Tmp.WaistSize);
// 			JsonWriter->WriteValue(TEXT("ArmSize"), Tmp.ArmSize);

			JsonWriter->WriteObjectEnd();
		}
		JsonWriter->WriteArrayEnd();
		JsonWriter->Close();
	}

	void CharacterAppearancesToString(const FMMOARPGCharacterAppearance& InCAs, FString& OutString)
	{
		OutString.Empty();

		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteObjectStart();
		{
			JsonWriter->WriteValue(TEXT("Name"), InCAs.Name);
			JsonWriter->WriteValue(TEXT("Date"), InCAs.Date);
			JsonWriter->WriteValue(TEXT("Lv"), InCAs.Lv);
 			JsonWriter->WriteValue(TEXT("SlotPosition"), InCAs.SlotPosition);
// 			JsonWriter->WriteValue(TEXT("LegSize"), InCAs.LegSize);
// 			JsonWriter->WriteValue(TEXT("WaistSize"), InCAs.WaistSize);
// 			JsonWriter->WriteValue(TEXT("ArmSize"), InCAs.ArmSize);
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void StringToCharacterAppearances(const FString& OutString, FCharacterAppearances& InCAs)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(OutString);
		TArray<TSharedPtr<FJsonValue>> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			for (auto& Tmp : ReadRoot) {
				InCAs.Add(FMMOARPGCharacterAppearance());
				FMMOARPGCharacterAppearance& InLast = InCAs.Last();
				if (TSharedPtr<FJsonObject> InJsonObject = Tmp->AsObject()) {
					InLast.Name = InJsonObject->GetStringField(TEXT("Name"));
					InLast.Date = InJsonObject->GetStringField(TEXT("Date"));
					InLast.Lv = InJsonObject->GetIntegerField(TEXT("Lv"));
 					InLast.SlotPosition = InJsonObject->GetIntegerField(TEXT("SlotPosition"));
// 					InLast.LegSize = InJsonObject->GetNumberField(TEXT("LegSize"));
// 					InLast.WaistSize = InJsonObject->GetNumberField(TEXT("WaistSize"));
// 					InLast.ArmSize = InJsonObject->GetNumberField(TEXT("ArmSize"));
				}
			}
		}
	}

	void StringToCharacterAppearances(const FString& OutString, FMMOARPGCharacterAppearance& InCAs)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(OutString);
		TSharedPtr<FJsonObject> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			InCAs.Name = ReadRoot->GetStringField(TEXT("Name"));
			InCAs.Date = ReadRoot->GetStringField(TEXT("Date"));
			InCAs.Lv = ReadRoot->GetIntegerField(TEXT("Lv"));
 			InCAs.SlotPosition = ReadRoot->GetIntegerField(TEXT("SlotPosition"));
// 			InCAs.LegSize = ReadRoot->GetNumberField(TEXT("LegSize"));
// 			InCAs.WaistSize = ReadRoot->GetNumberField(TEXT("WaistSize"));
// 			InCAs.ArmSize = ReadRoot->GetNumberField(TEXT("ArmSize"));
		}
	}

}

