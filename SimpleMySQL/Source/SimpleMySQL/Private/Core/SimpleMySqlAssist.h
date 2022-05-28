// Copyright (C) RenZhai.2020.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FSimpleMysqlComparisonOperator;
enum class EMysqlComparisonOperator : uint8;
struct FMysqlFieldType;

namespace SimpleMysqlAssist
{
	FString ComparisonOperatorToString(EMysqlComparisonOperator CO);

	void ConditionToString(FString &SQL,const TArray<FSimpleMysqlComparisonOperator> &Condition);

	void FieldTypeArrayToString(FString &SQL, const TMap<FString, FMysqlFieldType> &InNewFields);

	void FieldTypeToString(FString &SQL, const FString &Fieldname,const FMysqlFieldType &InType);

	void SpawnKeyAndValue(FString &SQL, const TMap<FString, FString> &InsetKeyValueDatas);

	void SpawnValue(FString &SQL, const TMap<FString, FString> &InsetKeyValueDatas);

	void SpawnValue(FString &SQL, const TArray<FString> &InsetValueDatas);

	bool IsMysqlFunction(const FString &Element);

	bool IsTypesOfAutomaticRecognition(EMysqlVariableType VariableType);

	bool UTF8ToGBK(const FString &FormData, FString &ToData);

	bool GBKToUTF8(const FString &FormData, FString &ToData);

	TCHAR *EncodingMysqlToUE(const char *NewEncoding);
	FString &EncodingUEToMysql(FString &NewEncoding);
}