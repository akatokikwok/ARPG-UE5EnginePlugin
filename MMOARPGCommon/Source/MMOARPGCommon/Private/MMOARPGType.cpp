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
 					InLast.LegSize = InJsonObject->GetNumberField(TEXT("LegSize"));
 					InLast.WaistSize = InJsonObject->GetNumberField(TEXT("WaistSize"));
 					InLast.ArmSize = InJsonObject->GetNumberField(TEXT("ArmSize"));
				}
			}
		}
	}

	void StringToCharacterAppearances(const FString& OutString, FMMOARPGCharacterAppearance& InCA)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(OutString);
		TSharedPtr<FJsonObject> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot)) {
			InCA.Name = ReadRoot->GetStringField(TEXT("Name"));
			InCA.Date = ReadRoot->GetStringField(TEXT("Date"));
			InCA.Lv = ReadRoot->GetIntegerField(TEXT("Lv"));
 			InCA.SlotPosition = ReadRoot->GetIntegerField(TEXT("SlotPosition"));
 			InCA.LegSize = ReadRoot->GetNumberField(TEXT("LegSize"));
 			InCA.WaistSize = ReadRoot->GetNumberField(TEXT("WaistSize"));
 			InCA.ArmSize = ReadRoot->GetNumberField(TEXT("ArmSize"));
		}
	}

}

