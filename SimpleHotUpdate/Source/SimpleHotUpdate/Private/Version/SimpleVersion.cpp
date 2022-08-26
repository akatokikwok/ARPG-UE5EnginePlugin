// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "Version/SimpleVersion.h"
#include "Json.h"
#include "..\..\Public\Version\SimpleVersion.h"

namespace SimpleVersionControl
{
	void Save(const FVersionList& Infor, FString& Json)
	{
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Json);
		JsonWriter->WriteObjectStart();//{
		JsonWriter->WriteValue(TEXT("Name"), Infor.Name);
		JsonWriter->WriteValue(TEXT("Crc"), Infor.Crc);
		JsonWriter->WriteArrayStart(TEXT("Content"));//Content:[
		for (const auto& Tmp : Infor.Content)
		{
			JsonWriter->WriteValue(Tmp);
		}
		JsonWriter->WriteArrayEnd();//]
		////}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	bool Read(const FString& Json, FVersionList& Infor)
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Json);
		TSharedPtr<FJsonValue> JsonParsed;
		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
		{
			const TSharedPtr<FJsonObject> JsonObject = JsonParsed->AsObject();

			if (!JsonObject->TryGetStringField(TEXT("Name"), Infor.Name))
			{
				return false;
			}

			if (JsonObject->TryGetStringField(TEXT("Crc"), Infor.Crc))
			{
				return false;
			}

			const TArray<TSharedPtr<FJsonValue>>* JsonArrays;
			if (!JsonObject->TryGetArrayField(TEXT("Content"), JsonArrays))
			{
				return false;
			}
			else if (JsonArrays)
			{
				for (const auto& Tmp : *JsonArrays)
				{
					Infor.Content.Add(Tmp->AsString());
				}
			}

			return true;
		}

		return false;
	}

	void Save(const FVersionInfor& Infor, FString& Json)
	{
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Json);

		//{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue(TEXT("Name"), Infor.Name);
		JsonWriter->WriteValue(TEXT("Crc"), Infor.Crc);
		JsonWriter->WriteArrayStart(TEXT("Content"));
		for (const auto& Tmp : Infor.Content)
		{
			JsonWriter->WriteObjectStart();
			JsonWriter->WriteValue(TEXT("Len"), Tmp.Len);
			JsonWriter->WriteValue(TEXT("Link"), Tmp.Link);
			JsonWriter->WriteValue(TEXT("Crc"), Tmp.Crc);
			JsonWriter->WriteValue(TEXT("bMainVersion"), Tmp.bMainVersion);
			JsonWriter->WriteValue(TEXT("InstallationPath"), Tmp.InstallationPath);
			JsonWriter->WriteValue(TEXT("bDiscard"), Tmp.bDiscard);
			JsonWriter->WriteObjectEnd();
		}
		JsonWriter->WriteArrayEnd();
		//}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	bool Read(const FString& Json, FVersionInfor& Infor)
	{
		TSharedPtr<FJsonValue> JsonParsed;

		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Json);
		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
		{
			bool bReadSuccess = true;

			const TSharedPtr<FJsonObject> JsonObject = JsonParsed->AsObject();

			if (!JsonObject->TryGetStringField(TEXT("Name"), Infor.Name))
			{
				bReadSuccess = false;
			}

			if (JsonObject->TryGetStringField(TEXT("Crc"), Infor.Crc))
			{
				bReadSuccess = false;
			}

			const TArray<TSharedPtr<FJsonValue>>* JsonArrays;
			if (!JsonObject->TryGetArrayField(TEXT("Content"), JsonArrays))
			{
				bReadSuccess = false;
			}
			else if (JsonArrays)
			{		
				for (const auto& Tmp : *JsonArrays)
				{
					const TSharedPtr<FJsonObject> TmpJsonObject = Tmp->AsObject();

					FRemoteDataDescribe RemoteDataDescribe;
					if (!TmpJsonObject->TryGetStringField(TEXT("Link"), RemoteDataDescribe.Link))
					{
						bReadSuccess = false;
					}

					if (!TmpJsonObject->TryGetNumberField(TEXT("Len"), RemoteDataDescribe.Len))
					{
						bReadSuccess = false;
					}

					if (!TmpJsonObject->TryGetStringField(TEXT("Crc"), RemoteDataDescribe.Crc))
					{
						bReadSuccess = false;
					}

					if (!TmpJsonObject->TryGetBoolField(TEXT("bMainVersion"), RemoteDataDescribe.bMainVersion))
					{
						bReadSuccess = false;
					}

					if (!TmpJsonObject->TryGetStringField(TEXT("InstallationPath"), RemoteDataDescribe.InstallationPath))
					{
						bReadSuccess = false;
					}

					{
						uint32 Type = 0;
						if (!TmpJsonObject->TryGetNumberField(TEXT("bDiscard"), Type))
						{
							bReadSuccess = false;
						}
						else
						{
							RemoteDataDescribe.bDiscard = (bool)Type;
						}
					}

					Infor.Content.Add(RemoteDataDescribe);
				}				
			}

			return true;
		}

		return false;
	}

	void Save(const FRemoteDataDescribe& Infor, FString& Json)
	{
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Json);

		JsonWriter->WriteObjectStart();
		{
			JsonWriter->WriteValue(TEXT("Len"), Infor.Len);
			JsonWriter->WriteValue(TEXT("Link"), Infor.Link);
			JsonWriter->WriteValue(TEXT("Crc"), Infor.Crc);
			JsonWriter->WriteValue(TEXT("bDiscard"), Infor.bDiscard);
			JsonWriter->WriteValue(TEXT("bMainVersion"), Infor.bMainVersion);
			JsonWriter->WriteValue(TEXT("InstallationPath"), Infor.InstallationPath);
		}
		JsonWriter->WriteObjectEnd();

		JsonWriter->Close();
	}

	bool Read(const FString& Json, FRemoteDataDescribe& Infor)
	{
		TSharedPtr<FJsonValue> JsonParsed;

		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Json);
		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
		{
			if (const TSharedPtr<FJsonObject> TmpJsonObject = JsonParsed->AsObject())
			{
				Infor.Len = TmpJsonObject->GetIntegerField(TEXT("Len"));
				Infor.bDiscard = TmpJsonObject->GetBoolField(TEXT("bDiscard"));
				Infor.Link = TmpJsonObject->GetStringField(TEXT("Link"));
				Infor.Crc = TmpJsonObject->GetStringField(TEXT("Crc"));
				Infor.bMainVersion = TmpJsonObject->GetBoolField(TEXT("bMainVersion"));
				Infor.InstallationPath = TmpJsonObject->GetStringField(TEXT("InstallationPath"));
			
				return true;
			}
		}

		return false;
	}
}

bool FSimpleVersionBase::IsVaild() const
{
	return !Crc.IsEmpty();
}

FVersionInfor::FVersionInfor()
{
	Name = TEXT("1.0.0");
}

void FVersionInfor::Empty()
{
	Content.Empty();
}

FRemoteDataDescribe::FRemoteDataDescribe()
	:Len(0)
	, Crc(FGuid::NewGuid().ToString())
	, bDiscard(false)
	, bMainVersion(false)
{

}

FRemoteDataDescribe::FRemoteDataDescribe(int64 InLen, const FString& InLink, const FString& InCrc)
	:Len(InLen)
	, Link(InLink)
	, Crc(InCrc)
	, bDiscard(false)
{
}
