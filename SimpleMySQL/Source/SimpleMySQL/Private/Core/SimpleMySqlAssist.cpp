// Copyright (C) RenZhai.2020.All Rights Reserved.
#include "SimpleMySqlAssist.h"
#include "Core/SimpleMysqlLinkType.h"
//#include <Windows.h>
//#include <xstring>

FString SimpleMysqlAssist::ComparisonOperatorToString(EMysqlComparisonOperator CO)
{
	switch (CO)
	{
	case EMysqlComparisonOperator::EQUAL:
		return TEXT("=");
	case EMysqlComparisonOperator::GREATER_THAN:
		return TEXT(">");
	case EMysqlComparisonOperator::LESS_THAN:
		return TEXT("<");
	case EMysqlComparisonOperator::GREATER_THAN_OR_EQUAL:
		return TEXT(">=");
	case EMysqlComparisonOperator::LESS_THAN_OR_EQUAL:
		return TEXT("<=");
	case EMysqlComparisonOperator::NOT_EQUAL:
		return TEXT("!=");
	case EMysqlComparisonOperator::BETWEEN:
		return TEXT("BETWEEN");
	case EMysqlComparisonOperator::NOT_BETWEEN:
		return TEXT("NOT BETWEEN");
	case EMysqlComparisonOperator::SQL_IN:
		return TEXT("IN");
	case EMysqlComparisonOperator::NOT_IN:
		return TEXT("NOT IN");
	case EMysqlComparisonOperator::COMPARE_NULL_VALUES:
		return TEXT("<=>");
	case EMysqlComparisonOperator::LIKE:
		return TEXT("LIKE");
	case EMysqlComparisonOperator::REGEXP_OR_RLIKE:
		return TEXT("REGEXP");
	case EMysqlComparisonOperator::IS_NULL:
		return TEXT("IS NULL");
	case EMysqlComparisonOperator::IS_NOT_NULL:
		return TEXT("IS NOT NULL");
	}

	return TEXT("");
}

void SimpleMysqlAssist::ConditionToString(FString &SQL, const TArray<FSimpleMysqlComparisonOperator> &Condition)
{
	if (Condition.Num())
	{
		SQL += TEXT(" WHERE ");
		for (const auto &Tmp : Condition)
		{
			SQL += Tmp.ToString() + TEXT(" ");
		}
	}
}

void SimpleMysqlAssist::FieldTypeArrayToString(FString &SQL, const TMap<FString, FMysqlFieldType> &InNewFields)
{
	if (InNewFields.Num())
	{
		for (auto &Tmp : InNewFields)
		{
			FieldTypeToString(SQL,Tmp.Key, Tmp.Value);
			SQL += TEXT(",");
		}

		SQL.RemoveFromEnd(TEXT(","));
	}
}

void SimpleMysqlAssist::FieldTypeToString(FString &SQL, const FString &Fieldname, const FMysqlFieldType &InType)
{
	SQL += Fieldname + TEXT(" ") + InType.ToString();
}

void SimpleMysqlAssist::SpawnKeyAndValue(FString &SQL, const TMap<FString, FString> &InsetKeyValueDatas)
{
	auto IsAll = [&]()
	{
		for (const auto &Tmp : InsetKeyValueDatas)
		{
			if (Tmp.Key.Contains(TEXT("*")))
			{
				return true;
			}
		}

		return false;
	};

	if (IsAll())
	{
		SQL += TEXT(" ");
	}
	else
	{
		SQL += TEXT("(");
		for (const auto &Tmp : InsetKeyValueDatas)
		{
			SQL += Tmp.Key + TEXT(",");
		}
		SQL.RemoveFromEnd(TEXT(","));

		SQL += TEXT(")");
	}

	SpawnValue(SQL, InsetKeyValueDatas);
}

bool SimpleMysqlAssist::IsMysqlFunction(const FString &Element)
{
	return Element.Contains(TEXT("(")) || Element.Contains(TEXT(")")) || Element.Contains(TEXT("()")) || Element.Contains(TEXT(" DIV ")) || Element.Contains(TEXT("@"));
}


bool SimpleMysqlAssist::IsTypesOfAutomaticRecognition(EMysqlVariableType VariableType)
{
	uint8	Type = (uint8)VariableType;
	return	Type != (uint8)EMysqlVariableType::MYSQL_JSON &&
			Type >= (uint8)EMysqlVariableType::MYSQL_TINY_BLOB && !(
			Type < (uint8)EMysqlVariableType::MYSQL_GEOMETRY);
}

bool SimpleMysqlAssist::GBKToUTF8(const FString &FormData, FString &ToData)
{
	char *FormDataPtr = TCHAR_TO_UTF8(const_cast<TCHAR *>(FormData.GetCharArray().GetData()));

	////转为unicode win utf16
	////拿到字节
	//int32 Len = MultiByteToWideChar(
	//	CP_ACP,//转换的格式
	//	0,//默认转换方式
	//	FormDataPtr,//输入的字符大小 
	//	-1,
	//	0,//输出
	//	0);//输出的空间大小

	//FString UTF8Data;
	//UTF8Data.Reset(Len);

	//wchar_t *UTF8DataPtr = TCHAR_TO_WCHAR(const_cast<TCHAR *>(UTF8Data.GetCharArray().GetData()));
	//char	*UTF8DataPtrChar = TCHAR_TO_UTF8(const_cast<TCHAR *>(UTF8Data.GetCharArray().GetData()));
	//MultiByteToWideChar(
	//	CP_ACP,
	//	0,
	//	FormDataPtr,
	//	-1,
	//	UTF8DataPtr,
	//	Len);

	//Len = WideCharToMultiByte(
	//	CP_UTF8,
	//	0,
	//	UTF8DataPtr,
	//	-1,
	//	0,//输出
	//	0,
	//	0, 0);//输出的空间大小

	//FString GBK;
	//GBK.Reset(Len);
	//char *GBKPtr = TCHAR_TO_UTF8(const_cast<TCHAR *>(GBK.GetCharArray().GetData()));
	//WideCharToMultiByte(
	//	CP_UTF8,
	//	0,
	//	UTF8DataPtr,
	//	-1,
	//	GBKPtr,
	//	Len,
	//	0,//输出
	//	0);//输出的空间大小

	//ToData += GBKPtr;

	return 0;
}

TCHAR * SimpleMysqlAssist::EncodingMysqlToUE(const char *NewEncoding)
{
	return ANSI_TO_TCHAR(NewEncoding);
}

FString & SimpleMysqlAssist::EncodingUEToMysql(FString &NewEncoding)
{
	return NewEncoding;
}

bool SimpleMysqlAssist::UTF8ToGBK(const FString &FormData, FString &ToData)
{
	//char *FormDataPtr = TCHAR_TO_UTF8(const_cast<TCHAR *>(FormData.GetCharArray().GetData()));
	//
	////转为unicode win utf16
	////拿到字节
	//int32 Len = MultiByteToWideChar(
	//	CP_UTF8,//转换的格式
	//	0,//默认转换方式
	//	FormDataPtr,//输入的字符大小 
	//	-1,
	//	0,//输出
	//	0);//输出的空间大小

	//FString UTF8Data;
	//UTF8Data.Reset(Len);
	// 
	//wchar_t *UTF8DataPtr = TCHAR_TO_WCHAR(const_cast<TCHAR *>(UTF8Data.GetCharArray().GetData()));
	//char	*UTF8DataPtrChar = TCHAR_TO_UTF8(const_cast<TCHAR *>(UTF8Data.GetCharArray().GetData()));
	//MultiByteToWideChar(
	//	CP_UTF8, 
	//	0, 
	//	FormDataPtr,
	//	-1,
	//	UTF8DataPtr,
	//	Len);

	//Len = WideCharToMultiByte(
	//	CP_ACP, 
	//	0,
	//	UTF8DataPtr,
	//	-1,
	//	0,//输出
	//	0,
	//	0,0);//输出的空间大小

	//FString GBK;
	//GBK.Reset(Len);
	//char *GBKPtr = TCHAR_TO_UTF8(const_cast<TCHAR *>(GBK.GetCharArray().GetData()));
	//WideCharToMultiByte(
	//	CP_ACP,
	//	0,
	//	UTF8DataPtr,
	//	-1,
	//	GBKPtr,
	//	Len,
	//	0,//输出
	//	0);//输出的空间大小

	//ToData += GBKPtr;

	return true;
}

void SimpleMysqlAssist::SpawnValue(FString &SQL, const TMap<FString, FString> &InsetKeyValueDatas)
{
	TArray<FString> InsetValueDatas;
	for (const auto &Tmp : InsetKeyValueDatas)
	{
		InsetValueDatas.Add(Tmp.Value);
	}
	SpawnValue(SQL, InsetValueDatas);
}

void SimpleMysqlAssist::SpawnValue(FString &SQL, const TArray<FString> &InsetValueDatas)
{
	FString ValueString;
	for (const auto &Tmp : InsetValueDatas)
	{
		if (!Tmp.IsEmpty())
		{
			//判断是不是Mysql函数
			if (IsMysqlFunction(Tmp))
			{
				ValueString += (Tmp + TEXT(","));
			}
			else
			{
				ValueString += (TEXT("'") + Tmp + TEXT("',"));
			}
		}
	}

	ValueString.RemoveFromEnd(TEXT(","));
	if (!ValueString.IsEmpty())
	{
		SQL += TEXT(" VALUES(") + ValueString + TEXT(")");
	}
}
