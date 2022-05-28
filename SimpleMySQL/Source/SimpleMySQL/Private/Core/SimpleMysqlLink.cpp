// Copyright (C) RenZhai.2020.All Rights Reserved.

#include "Core/SimpleMysqlLink.h"
#include "SimpleMySqlAssist.h"
#include "MySQL/mysql.h"
#include <string>
#include "Async/TaskGraphInterfaces.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif
 
FSimpleMysqlLink::FSimpleMysqlLink(const FString &InUser, const FString &InHost, const FString &InPawd, const FString &InDB, const uint32 InPort, const FString &InUnix_Socket /*= TEXT("\0")*/, const TArray<ESimpleClientFlags> &InClientFlag/*= 0*/)
	:User(InUser),
	Host(InHost),
	Pawd(InPawd),
	DB(InDB),
	Unix_Socket(InUnix_Socket),
	Port(InPort),
	ClientFlag(0),
	Mysql(new st_mysql()),
	MysqlStmt(NULL)
{
	for (auto &Tmp : InClientFlag)
	{
		ClientFlag |= ToMySqlClientFlag(Tmp);
	}

	//Ensure thread safety
	mysql_library_init(NULL, NULL, NULL);
	mysql_init(Mysql);

	MysqlStmt = mysql_stmt_init(Mysql);
}


FSimpleMysqlLink::~FSimpleMysqlLink()
{
	mysql_stmt_close(MysqlStmt);

	mysql_close(Mysql);
	mysql_library_end();

	delete Mysql;
	Mysql = NULL;
}

bool FSimpleMysqlLink::GetSelectTableData(const FSimpleMysqlQueryParameters &QueryParam, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	FString SQL;
	if (GetSelectTableDataSQL(SQL, QueryParam))
	{
		return QueryLinkResult(SQL, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool FSimpleMysqlLink::IsFieldExistInTable(const FString &InTableName, const FString &InFieldName, FString &ErrMesg)
{
	FString SQL = FString::Printf(TEXT("select count(*) as N from information_schema.columns where table_name = '%s' and column_name = '%s';"), *InTableName, *InFieldName);

	TArray<FSimpleMysqlResult> Results;
	if (QueryLinkResult(SQL, Results, ErrMesg))
	{
		int32 Number = FCString::Atoi(*Results[0].Rows["N"]);
		return Number > 0;
	}

	return false;
}

bool FSimpleMysqlLink::QueryLinkStmt(const FString& SQL, const TArray<FSimpleMysqlQueryStmt> &InQueryStmtParam, FString& ErrMesg)
{
	bool bQuerySuccess = false;

	FString PrepareSQLError;
	FString PrepareParametersError;
	FString ExecuteStmtError;
	if (PrepareStmtSQL(SQL,PrepareSQLError))
	{
		if (PrepareStmtParameters(InQueryStmtParam, PrepareParametersError))
		{	
			bQuerySuccess = ExecuteStmt(ExecuteStmtError);
		}
	}

	ErrMesg += (PrepareSQLError + PrepareParametersError + ExecuteStmtError);
	return bQuerySuccess;
}

bool FSimpleMysqlLink::PrepareStmtSQL(const FString &SQL, FString& ErrMesg)
{
	int32 Ret = mysql_ping(Mysql);
	if (Ret == 0)
	{
		if ((Ret = mysql_stmt_prepare(MysqlStmt, TCHAR_TO_UTF8(*SQL), SQL.Len())) != 0)
		{
			ErrMesg = UTF8_TO_TCHAR(mysql_stmt_error(MysqlStmt));
			return false;
		}
	}
	else
	{
		if (mysql_real_connect(
			Mysql,
			TCHAR_TO_UTF8(*Host),
			TCHAR_TO_UTF8(*User),
			TCHAR_TO_UTF8(*Pawd),
			TCHAR_TO_UTF8(*DB),
			Port,
			Unix_Socket == TEXT("") ? 0 : TCHAR_TO_UTF8(*Unix_Socket),
			ClientFlag))//
		{
			if ((Ret = mysql_stmt_prepare(MysqlStmt, TCHAR_TO_UTF8(*SQL), SQL.Len())) != 0)
			{
				ErrMesg = UTF8_TO_TCHAR(mysql_stmt_error(MysqlStmt));
				return false;
			}
		}
		else
		{
			ErrMesg = UTF8_TO_TCHAR(mysql_stmt_error(MysqlStmt));
			return false;
		}
	}

	return true;
}

bool FSimpleMysqlLink::PrepareStmtParameters(const TArray<FSimpleMysqlQueryStmt>& InQueryStmtParam, FString& ErrMesg)
{
	if (InQueryStmtParam.Num() != 0)
	{
		BindAllocation.Free();

		int32 MemorySize = sizeof(MYSQL_BIND) * InQueryStmtParam.Num();
		BindAllocation.Size = InQueryStmtParam.Num();
		BindAllocation.BindParam = (MYSQL_BIND*)FMemory::Malloc(MemorySize);
		FMemory::Memzero(BindAllocation.BindParam, MemorySize);

		uint8 *Byteode = (uint8*)BindAllocation.BindParam;
		for (const auto &Tmp : InQueryStmtParam)
		{
			MYSQL_BIND * TmpBind = (MYSQL_BIND*)Byteode;

			TmpBind->buffer_type = (enum_field_types)Tmp.VariableType;			
			Tmp.GetValue(&TmpBind->buffer);
			TmpBind->length = Tmp.GetValueTypeLenMemory();
			TmpBind->buffer_length = Tmp.GetValueTypeLen();

			Byteode += sizeof(MYSQL_BIND);
		}

		if (mysql_stmt_bind_param(MysqlStmt, BindAllocation.BindParam) == 0)
		{
			return true;
		}

		ErrMesg = UTF8_TO_TCHAR(mysql_stmt_error(MysqlStmt));
	}

	return false;
}

bool FSimpleMysqlLink::ExecuteStmt(FString& ErrMesg)
{
	if (mysql_stmt_execute(MysqlStmt) == 0)
	{
		return true;
	}

	ErrMesg = UTF8_TO_TCHAR(mysql_stmt_error(MysqlStmt));

	BindAllocation.Free();

	return false;
}

bool FSimpleMysqlLink::QueryLinkResult(const FString &SQL, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	FString ErrMsgQuery;
	if (QueryLink(SQL, ErrMsgQuery))
	{
		switch (SaveType)
		{
		case EMysqlQuerySaveType::STORE_RESULT:
			GetStoreResult(Results, ErrMesg, Debug);
			break;
		case EMysqlQuerySaveType::USE_RESULT:
			GetUseResult(Results, ErrMesg, Debug);
			break;
		}
	}

	ErrMesg += ErrMsgQuery;

	return ErrMesg.IsEmpty();
}

bool FSimpleMysqlLink::UpdateTableData(const FString &TableName, const TArray<FSimpleMysqlAssignment> &InFields, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg)
{
	FString SQL = TEXT("UPDATE ") + TableName + TEXT(" SET ");
	for (auto &Tmp : InFields)
	{
		SQL += (Tmp.ToString() + TEXT(","));
	}
	SQL.RemoveFromEnd(TEXT(","));

	SimpleMysqlAssist::ConditionToString(SQL, Condition);

	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::ReplaceTableData(const FString &TableName, const TArray<FSimpleMysqlReplaceParameters> &InReplaces, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg)
{
	FString SQL = TEXT("UPDATE ") + TableName + TEXT(" SET ");
	for (auto &Tmp : InReplaces)
	{
		SQL += (Tmp.ToString() + TEXT(","));
	}
	SQL.RemoveFromEnd(TEXT(","));

	SimpleMysqlAssist::ConditionToString(SQL, Condition);

	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::InsertTableData(const FString& TableName, const TMap<FString, FString> &InsetKeyValueDatas, const FSimpleMysqlQueryParameters &QueryParam, FString& ErrorMsg, bool bIgnore)
{
	FString SQL = TEXT("INSERT ");// ;
	
	if (bIgnore)
	{
		SQL += TEXT("IGNORE ");
	}

	SQL+= TEXT("INTO ") + TableName; 

	SimpleMysqlAssist::SpawnKeyAndValue(SQL, InsetKeyValueDatas);

	if (QueryParam.IsValid())
	{
		QueryParam.ToString(SQL);
	}

	SQL += TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::ReplaceIfExistence(const FString& TableName, const TMap<FString, FString> &InsetKeyValueDatas, FString& ErrorMsg)
{
	FString SQL = TEXT("REPLACE INTO ") + TableName;// ;

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

	SimpleMysqlAssist::SpawnKeyAndValue(SQL, InsetKeyValueDatas);

	SQL += TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::ChangeEngine(const FString &InTableName, EMysqlSaveEngine InEngine, FString& ErrorMsg)
{
	UEnum *EngineEnum = StaticEnum<EMysqlSaveEngine>();
	FString SQL = TEXT("ALTER TABLE ") + InTableName + TEXT(" ENGINE=") 
		+ EngineEnum->GetNameStringByIndex((int32)InEngine) + TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::DropTableField(const FString &InTableName,const TArray<FString> &InFieldsName, FString& ErrorMsg)
{
	FString SQL = TEXT("ALTER TABLE ") + InTableName + TEXT(" DROP ");
	for (auto &Tmp : InFieldsName)
	{
		SQL += TEXT(" COLUMN ") + Tmp + TEXT(",");
	}
	SQL.RemoveFromEnd(TEXT(","));
	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::AddTableField(const FString &InTableName, const TMap<FString, FMysqlFieldType> &InFields, FString& ErrorMsg)
{
	FString SQL = TEXT("ALTER TABLE ") + InTableName + TEXT(" ADD ");

	SimpleMysqlAssist::FieldTypeArrayToString(SQL, InFields);
	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::ModifyTableFieldType(const FString &InTableName, const TMap<FString, FMysqlFieldType> &InFields, FString& ErrorMsg)
{
	FString SQL = TEXT("ALTER TABLE ") + InTableName + TEXT(" MODIFY ");

	SimpleMysqlAssist::FieldTypeArrayToString(SQL, InFields);
	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::ChangeTableField(const FString &InTableName, const TArray<FSimpleMysqlModifyField> &InChangeFields, FString& ErrorMsg)
{
	FString SQL = TEXT("ALTER TABLE ") + InTableName + TEXT(" CHANGE ");
	for (const auto &Tmp : InChangeFields)
	{
		SQL += Tmp.ToString() + TEXT(",");
	}
	SQL.RemoveFromEnd(TEXT(","));
	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::RenameTable(const FString &InOldTableName, const FString &InNewTableName,FString &ErrorMsg)
{
	FString SQL = TEXT("ALTER TABLE ") + InOldTableName + TEXT(" RENAME TO ") + InNewTableName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::SimpleInsertTableData(const FString& TableName, const TArray<FString> &InsetValueDatas, const FSimpleMysqlQueryParameters &QueryParam, FString& ErrorMsg, bool bIgnore/* = false*/)
{
	FString SQL = TEXT("INSERT ");// ;
	if (bIgnore)
	{
		SQL += TEXT("IGNORE ");
	}
	SQL += TEXT("INTO ") + TableName;

	SimpleMysqlAssist::SpawnValue(SQL, InsetValueDatas);

	if (QueryParam.IsValid())
	{
		QueryParam.ToString(SQL);
	}

	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::GetSelectTableDataSR(const FSimpleMysqlQueryParameters &QueryParam, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	FString SQL;
	if (GetSelectTableDataSQL(SQL, QueryParam))
	{
		return QueryLinkStoreResult(SQL, Results, ErrMesg, Debug);
	}
	
	return false;
}

bool FSimpleMysqlLink::GetSelectTableDataUR( const FSimpleMysqlQueryParameters &QueryParam, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	FString SQL;
	if (GetSelectTableDataSQL(SQL,QueryParam))
	{
		return QueryLinkUseResult(SQL, Results, ErrMesg, Debug);
	}

	return false;
}

bool FSimpleMysqlLink::GetSelectTableDataByFuntion(const FString& SQL, EMysqlQuerySaveType SaveType, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, const FSimpleMysqlDebugResult& Debug)
{
	FSimpleMysqlQueryParameters QueryParam;
	QueryParam.FuntionSettings.VariableName = SQL;

	return GetSelectTableData(QueryParam, Results, ErrMesg, SaveType,Debug);
}

bool FSimpleMysqlLink::CopyTableAToBFastInternal(FString &SQL,const FString &CopySourceTable, const FString &CopyTargetTable, const TMap<FString, FMysqlFieldType> &InsertFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg)
{
	if (InsertFields.Num())
	{
		SQL += TEXT("(");

		//Create field
		SimpleMysqlAssist::FieldTypeArrayToString(SQL, InsertFields);

		SQL += TEXT(")");
	}

	if (QueryParam.IsValid())
	{
		SQL += TEXT(" AS (");

		QueryParam.ToString(SQL);

		SQL += TEXT(");");
	}
	else
	{
		ErrorMsg = TEXT("Query parameters are meaningless.");
		return false;
	}

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::GetStoreResult(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug)
{
	if (MYSQL_RES *Result = mysql_store_result(Mysql))
	{
		GetResult(Result, Results);

		if (Debug.bPrintToScreen || Debug.bPrintToLog)
		{
			PrintResult(Results, Debug.bPrintToScreen, Debug.bPrintToLog);
		}

		mysql_free_result(Result);
	}

	ErrMesg = UTF8_TO_TCHAR(mysql_error(Mysql));

	return ErrMesg.IsEmpty();
}

bool FSimpleMysqlLink::GetUseResult(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug)
{
	if (MYSQL_RES *Result = mysql_use_result(Mysql))
	{
		GetResult(Result, Results);

		if (Debug.bPrintToScreen || Debug.bPrintToLog)
		{
			PrintResult(Results, Debug.bPrintToScreen, Debug.bPrintToLog);
		}	

		mysql_free_result(Result);
	}

	ErrMesg = UTF8_TO_TCHAR(mysql_error(Mysql));

	return ErrMesg.IsEmpty();
}

bool FSimpleMysqlLink::SelectNewDB(const FString &NewDB,FString &ErrMesg)
{
	int32 Ret = mysql_ping(Mysql);
	if (Ret == 0)
	{
		if (mysql_select_db(Mysql, TCHAR_TO_UTF8(*NewDB)) == 0)
		{
			DB = NewDB;
			return true;
		}		
	}
	else
	{
		if (mysql_real_connect(
			Mysql,
			TCHAR_TO_UTF8(*Host),
			TCHAR_TO_UTF8(*User),
			TCHAR_TO_UTF8(*Pawd),
			TCHAR_TO_UTF8(*DB),
			Port,
			Unix_Socket == TEXT("") ? 0 : TCHAR_TO_UTF8(*Unix_Socket),
			ClientFlag))//
		{
			if (mysql_select_db(Mysql, TCHAR_TO_UTF8(*NewDB)) == 0)
			{
				DB = NewDB;

				return true;
			}
			
			ErrMesg = UTF8_TO_TCHAR(mysql_error(Mysql));	
		}
		else
		{
			ErrMesg = UTF8_TO_TCHAR(mysql_error(Mysql));
		}
	}
	
	return false;
}

bool FSimpleMysqlLink::QueryLink(const FString &SQL,FString &ErrMesg)
{
	int32 Ret = mysql_ping(Mysql);
	if (Ret == 0)
	{
		if ((Ret = mysql_query(Mysql, TCHAR_TO_UTF8(*SQL))) != 0)
		{
			ErrMesg = UTF8_TO_TCHAR(mysql_error(Mysql));
			return false;
		}
	}
	else
	{
		if (mysql_real_connect(
			Mysql,
			TCHAR_TO_UTF8(*Host), 
			TCHAR_TO_UTF8(*User),
			TCHAR_TO_UTF8(*Pawd),
			TCHAR_TO_UTF8(*DB),
			Port,
			Unix_Socket == TEXT("") ? 0 : TCHAR_TO_UTF8(*Unix_Socket),
			ClientFlag))//
		{
			mysql_set_character_set(Mysql, "utf8");
			if ((Ret = mysql_query(Mysql, TCHAR_TO_UTF8(*SQL))) != 0)
			{
				ErrMesg = UTF8_TO_TCHAR(mysql_error(Mysql));
				return false;
			}
		}
		else
		{
			ErrMesg = UTF8_TO_TCHAR(mysql_error(Mysql));
			return false;
		}
	}

	return true;
}

bool FSimpleMysqlLink::QueryLinkStoreResult(const FString &SQL, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug)
{
	FString ErrMsgQuery;
	if (QueryLink(SQL, ErrMsgQuery))
	{
		GetStoreResult(Results, ErrMesg, Debug);
	}

	ErrMesg += ErrMsgQuery;

	return ErrMesg.IsEmpty();
}

bool FSimpleMysqlLink::QueryLinkUseResult(const FString &SQL, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug)
{
	FString ErrMsgQuery;
	if (QueryLink(SQL, ErrMsgQuery))
	{
		GetUseResult(Results, ErrMesg, Debug);
	}

	ErrMesg += ErrMsgQuery;

	return ErrMesg.IsEmpty();
}

bool FSimpleMysqlLink::CreateAndSelectDataBase(const FString &DataBaseName, EMysqlCharset Charset, FString &ErrorMsg)
{
	FString CreateDataBaseError;
	if (CreateDataBase(DataBaseName, Charset, CreateDataBaseError))
	{
		SelectNewDB(DataBaseName, ErrorMsg);
	}

	ErrorMsg += CreateDataBaseError;

	return ErrorMsg.IsEmpty();
}

bool FSimpleMysqlLink::CreateDataBase(const FString &DataBaseName, EMysqlCharset Charset, FString &ErrorMsg)
{
	FString SQL = TEXT("CREATE DATABASE ") + DataBaseName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::DropDataBase(const FString &DataBaseName, FString &ErrorMsg)
{
	FString SQL = TEXT("DROP DATABASE ") + DataBaseName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::CreateTable(const FString &TableName, const TMap<FString, FMysqlFieldType> &InFields, const TArray<FString> &InPrimaryKeys, const FMysqlCreateTableParam &Param, FString &ErrorMsg)
{
	FString SQL = TEXT("CREATE TABLE `");
	SQL += (TableName + TEXT("`("));

	//Create field
	SimpleMysqlAssist::FieldTypeArrayToString(SQL,InFields);

	//Create primary key
	//PRIMARY KEY(1,2,3,4,5,6),
	if (InPrimaryKeys.Num())
	{
		SQL += TEXT("PRIMARY KEY(`");
		for (auto &Tmp : InPrimaryKeys)
		{
			SQL += (Tmp + TEXT(","));
		}
		SQL.RemoveFromEnd(TEXT(","));

		SQL += TEXT("`),");
	}
	
	SQL.RemoveFromEnd(",");
	SQL += TEXT(")");
	
	SQL += Param.ToString();

	SQL += ";";

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::CreateTemporaryTable(const FString &TableName, const TMap<FString, FMysqlFieldType> &InFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg)
{
	FString ContentString;
	if (QueryParam.IsValid())
	{
		QueryParam.ToString(ContentString);
	}

	FString SQL = TEXT("CREATE TEMPORARY TABLE `") + TableName + TEXT("`(");

	//Create field
	SimpleMysqlAssist::FieldTypeArrayToString(SQL, InFields);
	SQL += TEXT(") ") + ContentString + TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::ShowTables(const FString &TableName, EMysqlQuerySaveType SaveType, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug)
{
	FSimpleMysqlQueryParameters QueryParam;
	QueryParam.TableName = TableName;
	QueryParam.AddField(TEXT("*"));

	return GetSelectTableData(QueryParam, Results, ErrMesg, SaveType, Debug);
}

bool FSimpleMysqlLink::CreateTableALikeB(const FString &A, const FString &B, FString &ErrorMsg)
{
	FString SQL = TEXT("CREATE TABLE ") + A + TEXT(" LIKE ") + B + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::CreateTemporaryTableALikeB(const FString &A, const FString &B, FString &ErrorMsg)
{
	FString SQL = TEXT("CREATE TEMPORARY TABLE ") + A + TEXT(" LIKE ") + B + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::CopyTableAToB(const FString &A, const FString &B, const TArray<FString> &Fields, FString &ErrorMsg)
{
	FString Error;
	if (CreateTableALikeB(B,A, Error))
	{
		TMap<FString, FString> MapFields;
		for (auto &Tmp : Fields)
		{
			MapFields.Add(Tmp, TEXT(""));
		}

		FSimpleMysqlQueryParameters QueryParameters;
		QueryParameters.TableName = A;
		for (auto &Tmp : Fields)
		{
			QueryParameters.InFields.Add(FSimpleMysqlVariableSettings());
			QueryParameters.InFields[QueryParameters.InFields.Num() - 1].VariableName = Tmp;
		}

		InsertTableData(B, MapFields, QueryParameters, ErrorMsg);
		ErrorMsg += Error;	
	}

	return ErrorMsg.IsEmpty();
}

bool FSimpleMysqlLink::CopyTableAToTemporaryB(const FString &TableA, const FString &TemporaryTableB, const TArray<FString> &Fields, FString &ErrorMsg)
{
	FString Error;
	if (CreateTemporaryTableALikeB(TemporaryTableB, TableA, Error))
	{
		TMap<FString, FString> MapFields;
		for (auto &Tmp : Fields)
		{
			MapFields.Add(Tmp, TEXT(""));
		}

		FSimpleMysqlQueryParameters QueryParameters;
		QueryParameters.TableName = TableA;
		for (auto &Tmp : Fields)
		{
			QueryParameters.InFields.Add(FSimpleMysqlVariableSettings());
			QueryParameters.InFields[QueryParameters.InFields.Num() - 1].VariableName = Tmp;
		}

		InsertTableData(TemporaryTableB, MapFields, QueryParameters, ErrorMsg);
		ErrorMsg += Error;
	}

	return ErrorMsg.IsEmpty();
}

bool FSimpleMysqlLink::CopyTableAToBFast(const FString &CopySourceTable,const TMap<FString, FMysqlFieldType> &InsertFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg)
{
	FString SQL = TEXT("CREATE TABLE ") + QueryParam.TableName;
	return CopyTableAToBFastInternal(SQL, CopySourceTable, QueryParam.TableName, InsertFields, QueryParam, ErrorMsg);
}

bool FSimpleMysqlLink::CopyTableAToTemporaryBFast(const FString &CopySourceTable,const TMap<FString, FMysqlFieldType> &InsertFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg)
{
	FString SQL = TEXT("CREATE TEMPORARY TABLE ") + QueryParam.TableName;
	return CopyTableAToBFastInternal(SQL, CopySourceTable, QueryParam.TableName, InsertFields, QueryParam, ErrorMsg);
}

bool FSimpleMysqlLink::DropTemporaryTable(const FString &TemporaryTableName, FString &ErrorMsg)
{
	FString SQL = TEXT("DROP TEMPORARY TABLE IF EXISTS") + TemporaryTableName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::DropTable(const FString &TableName, FString &ErrorMsg)
{
	FString SQL = TEXT("DROP TABLE IF EXISTS") + TableName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::TruncateTable(const FString &TableName, FString &ErrorMsg)
{
	FString SQL = TEXT("TRUNCATE TABLE ") + TableName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::DeleteFromTable(const FString &TableName, FString &ErrorMsg)
{
	FString SQL = TEXT("DELETE FROM ") + TableName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::OptimiseTable(const FString &TableName, FString &ErrorMsg)
{
	FString SQL = TEXT("OPTIMISE TABLE ") + TableName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::DeleteFromTableWhereData(const FString &TableName, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg)
{
	FString SQL = TEXT("DELETE FROM ") + TableName;
	SimpleMysqlAssist::ConditionToString(SQL, Condition);
	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::PrintResult(const TArray<FSimpleMysqlResult> &Results,bool bPrintToScreen /*= true*/, bool bPrintToLog /*= true*/)
{
	auto CallMainThreadWork = [&]()
	{
		if (Results.Num())
		{
			//|  AC	  | 5 / 2 = 2
			//|ABCDEFH|
			//
			//Get the maximum value of content plus field
			auto LenMax = [&]()->int32
			{
				int32 MaxLen = 0;

				//Find the maximum value of the field
				for (auto &Tmp : Results[0].Rows)
				{
					if (MaxLen < Tmp.Key.Len())
					{
						MaxLen = Tmp.Key.Len();
					}
				}

				//Find value
				for (auto &Tmp : Results)
				{
					for (auto &MapTmp : Tmp.Rows)
					{
						if (MaxLen < MapTmp.Value.Len())
						{
							MaxLen = MapTmp.Value.Len();
						}
					}
				}

				return MaxLen;
			};

			int32 MaxLen = LenMax();

			//
			auto PrintString = [&](const FString &InString, FString &OutStirng)
			{
				int32 Len = InString.Len();
				int32 AddLen = MaxLen - Len;

				int32 R = 0;
				int32 L = 0;

				if (AddLen != 0)
				{
					if (AddLen % 2 == 0)//even numbers
					{
						R = AddLen / 2;
						L = R;
					}
					else
					{
						L = AddLen / 2;
						R = L + 1;
					}
				}

				L += 2;
				R += 2;

				OutStirng += TEXT("|");
				for (int32 j = 0; j < L; j++)
				{
					OutStirng += " ";
				}

				OutStirng += InString;

				for (int32 j = 0; j < R; j++)
				{
					OutStirng += " ";
				}
			};

			auto DrawTable = [&](
				const TCHAR *StartTable,
				const TCHAR *MidTable,
				const TCHAR *BetweenTable,
				const TCHAR *EndTable)
			{
				FString RowString = StartTable;
				for (int32 i = 0; i < Results[0].Rows.Num(); i++)
				{
					for (int32 j = 0; j < MaxLen; j+=3)
					{
						RowString += BetweenTable;
					}

					if (i == (Results[0].Rows.Num() - 1))
					{
						RowString += EndTable;
					}
					else
					{
						RowString += MidTable;
					}
				}

				Mysql_Printf(*RowString, false, bPrintToLog, FColor::Red, 100.f);
			};

			DrawTable(TEXT("|--"), TEXT("-|-"), TEXT("---"), TEXT("--|"));

			//Draw field
			{
				FString RowString;
				for (auto &TmpString : Results[0].Rows)
				{
					PrintString(TmpString.Key, RowString);
				}

				RowString += TEXT("  |");
				Mysql_Printf(*RowString, bPrintToScreen, bPrintToLog, FColor::Red, 100.f);
			}

			//Draw values
			for (auto &Tmp : Results)
			{
				//DrawTable(TEXT("┣"), TEXT("╋"), TEXT("━"), TEXT("┫"));
				DrawTable(TEXT("|--"), TEXT("-|-"), TEXT("---"), TEXT("--|"));
				//Draw values
				FString RowString;
				for (auto &TmpString : Tmp.Rows)
				{
					PrintString(TmpString.Value, RowString);
				}

				RowString += "|";
				Mysql_Printf(*RowString, bPrintToScreen, bPrintToLog, FColor::Red, 100.f);
			}

			//end
		//	DrawTable(TEXT("┗"), TEXT("┻"), TEXT("━"), TEXT("┛"));
			DrawTable(TEXT("|__"), TEXT("_|_"), TEXT("___"), TEXT("__|"));
		}
	};

	if (FPlatformTLS::GetCurrentThreadId() == GGameThreadId)
	{
		CallMainThreadWork();
	}
	else
	{
		//call main thread
		FGraphEventRef EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			CallMainThreadWork();
		},
		TStatId(),
		NULL,
		ENamedThreads::GameThread);

		//Asynchronous thread blocking
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);
	}

	return Results.Num() != 0;
}

bool FSimpleMysqlLink::CreateFunction(const FString &InFunctionName, const TMap<FString, FMysqlFieldType> &InParameter, const FString &InFunctionBody, FString& ErrMesg , EMysqlVariableType InReturnsType /*= EMysqlVariableType::INT*/, const FString &InComment /*= TEXT("")*/)
{
	FString SQL = TEXT("CREATE FUNCTION ") + InFunctionName + TEXT("(");

	UEnum *MysqlVariableEnum = StaticEnum<EMysqlVariableType>();
	FString FieldTypeString = MysqlVariableEnum->GetNameStringByIndex((int32)InReturnsType);

	//Create field
	SimpleMysqlAssist::FieldTypeArrayToString(SQL, InParameter);
	SQL += TEXT(") RETURNS ") + FieldTypeString + TEXT(" ");

	if (!InComment.IsEmpty())
	{
		SQL += (TEXT("COMMENT \"") + InComment + TEXT("\" "));
	}

	SQL += TEXT("BEGIN ") + InFunctionBody + TEXT(" END;");

	return QueryLink(SQL, ErrMesg);
}

bool FSimpleMysqlLink::ExecuteFunction(const FString &InFunctionName, const TArray<FString> &InParameter, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	FString SQL = InFunctionName + TEXT("(");
	for (auto &Tmp : InParameter)
	{
		SQL += TEXT("'")  + Tmp + TEXT("',");
	}
	SQL.RemoveFromEnd(TEXT(","));
	SQL += TEXT(")");

	return GetSelectTableDataByFuntion(SQL, SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::DropFunction(const FString &InFunctionName, FString& ErrMesg)
{
	FString SQL = TEXT("DROP FUNCTION ") + InFunctionName + TEXT(";");
	return QueryLink(SQL,ErrMesg);
}

bool FSimpleMysqlLink::ShowCreateFunction(const FString &InFunctionName, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType,const FSimpleMysqlDebugResult& Debug)
{
	FString SQL = TEXT("SHOW CREATE FUNCTION ") + InFunctionName + TEXT(";");
	return QueryLinkResult(SQL,Results,ErrMesg,SaveType, Debug);
}

bool FSimpleMysqlLink::ShowFunctionsStatus(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return QueryLinkResult(TEXT("SHOW FUNCTION STATUS;"), Results, ErrMesg, SaveType, Debug);
}

bool FSimpleMysqlLink::DefiningVariables(const FString &InVariablesName, const FString &InValue, FString &ErrMesg)
{
	return QueryLink((TEXT("SET ") + InVariablesName + TEXT("=") + InValue + TEXT(";")),ErrMesg);
}

bool FSimpleMysqlLink::CreateProcedure(const FString &InProcedureName, const TMap<FString, FMysqlFieldType> &InParameter, const FString &InProcedureBody, FString& ErrMesg, const FString &InComment /*= TEXT("")*/)
{
	FString SQL = TEXT("CREATE PROCEDURE ") + InProcedureName + TEXT("(");

	//Create field
	SimpleMysqlAssist::FieldTypeArrayToString(SQL, InParameter);
	SQL += TEXT(") ");

	if (!InComment.IsEmpty())
	{
		SQL += (TEXT("COMMENT \"") + InComment + TEXT("\" "));
	}

	SQL += TEXT("BEGIN ") + InProcedureBody + TEXT(" END;");

	return QueryLink(SQL, ErrMesg);
}

bool FSimpleMysqlLink::ExecuteProcedure(const FString &InProcedureName, const TArray<FString> &InParameter, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	FString SQL = TEXT("CALL ") + InProcedureName + TEXT("(");
	for (auto &Tmp : InParameter)
	{
		if (Tmp.Contains("IN "))
		{
			SQL += TEXT("'") + Tmp + TEXT("',");
		}
		else
		{
			SQL += Tmp;
		}
	}
	SQL.RemoveFromEnd(TEXT(","));
	SQL += TEXT(");");

	return QueryLinkResult(SQL, Results, ErrMesg, SaveType, Debug);
}

bool FSimpleMysqlLink::DropProcedure(const FString &InProcedureName, FString& ErrMesg)
{
	FString SQL = TEXT("DROP PROCEDURE ") + InProcedureName + TEXT(";");
	return QueryLink(SQL, ErrMesg);
}

bool FSimpleMysqlLink::ShowCreateProcedure(const FString &InProcedureName, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	FString SQL = TEXT("SHOW CREATE PROCEDURE ") + InProcedureName + TEXT(";");
	return QueryLinkResult(SQL, Results, ErrMesg, SaveType, Debug);
}

bool FSimpleMysqlLink::ShowProceduresStatus(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return QueryLinkResult(TEXT("SHOW PROCEDURE STATUS;"), Results, ErrMesg, SaveType, Debug);
}

bool FSimpleMysqlLink::GetASCII(const FString& InString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("ASCII('%s')"), *InString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::CharLength(const FString& InString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("CHAR_LENGTH('%s')"), *InString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::CharacterLength(const FString& InString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("CHARACTER_LENGTH('%s')"), *InString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Concat(const TArray<FString>& InStrings, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	FString SQL;
	for (auto &Tmp : InStrings)
	{
		SQL += TEXT("'") + Tmp + TEXT("'") + TEXT(",");
	}
	SQL.RemoveFromEnd(TEXT(","));

	return GetSelectTableDataByFuntion(FString::Printf(TEXT("CONCAT(%s)"), *SQL), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::ConcatWS(const TCHAR* InSeparator, const TArray<FString>& InStrings, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	FString SQL;
	for (auto& Tmp : InStrings)
	{
		SQL += TEXT("'") + Tmp + TEXT("'") + TEXT(",");
	}
	SQL.RemoveFromEnd(TEXT(","));

	return GetSelectTableDataByFuntion(FString::Printf(TEXT("CONCAT_WS('%s',%s)"), InSeparator, *SQL), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Field(const TCHAR* InQueryString, const TArray<FString>& InStrings, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	FString SQL;
	for (auto& Tmp : InStrings)
	{
		SQL += TEXT("'") + Tmp + TEXT("'") + TEXT(",");
	}
	SQL.RemoveFromEnd(TEXT(","));

	return GetSelectTableDataByFuntion(FString::Printf(TEXT("FIELD('%s',%s)"), InQueryString, *SQL), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::FindInSet(const TCHAR* InQueryString, const FString& InString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("ASCII('%s')"), *InString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Format(float InNumber, int8 KeepDecimalPoint, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("FORMAT('%f','%i')"), InNumber, KeepDecimalPoint), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Insert(const FString& InSourceString, int32 InStartPos, const TCHAR* InReplacedString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("INSERT('%s',%i,%i,%s)"), *InSourceString, InStartPos,FString(InReplacedString).Len(), InReplacedString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Locate(const FString& InSourceString, const TCHAR* InFindString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("LOCATE('%s','%s')"), *InSourceString, InFindString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Lcase(const FString& InSourceString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("LCASE('%s')"), *InSourceString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Lower(const FString& InSourceString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("LOWER('%s')"), *InSourceString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Upper(const FString& InSourceString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("ASCII('%s')"), *InSourceString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Ucase(const FString& InSourceString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("UCASE('%s')"), *InSourceString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Left(const FString& InSourceString, int32 ReturnStringLen, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("LEFT('%s',%i)"), *InSourceString, ReturnStringLen), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Right(const FString& InSourceString, int32 ReturnStringLen, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("RIGHT('%s',%i)"), *InSourceString, ReturnStringLen), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Lpad(const FString& InSourceString, int32 StringLen, const FString& InAddString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("LPAD('%s',%i,'%s')"), *InSourceString, StringLen,*InAddString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Rpad(const FString& InSourceString, int32 StringLen, const FString& InAddString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("RPAD('%s',%i,'%s')"), *InSourceString, StringLen, *InAddString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Ltrim(const FString& InSourceString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("LTRIM('%s')"), *InSourceString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Rtrim(const FString& InSourceString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("RTRIM('%s')"), *InSourceString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Trim(const FString& InSourceString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("TRIM('%s')"), *InSourceString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Mid(const FString& InSourceString, int32 InStartPos, int32 InStringLen, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("MID('%s',%i,%i)"), *InSourceString, InStartPos, InStringLen), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::SubString(const FString& InSourceString, int32 InStartPos, int32 InStringLen, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("SUBSTRING('%s',%i,%i)"), *InSourceString, InStartPos, InStringLen), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::SubStr(const FString& InSourceString, int32 InStartPos, int32 InStringLen, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("SUBSTR('%s')"), *InSourceString, InStartPos, InStringLen), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::SubStringIndex(const FString& S, const TCHAR* InDelimiter, int32 InNumber, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("SUBSTRING_INDEX('%s','%s',%i)"), *S, InDelimiter, InNumber), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Position(const FString& InSourceString, const TCHAR* InMatchingString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("POSITION('%s','%s')"), *InSourceString, InMatchingString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Repeat(const FString& InSourceString, int32 InTimes, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("REPEAT('%s',%i)"), *InSourceString, InTimes), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Replace(const FString& InSourceString, const TCHAR* ToReplace, const TCHAR* InReplaced, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("REPLACE('%s','%s','%s')"), *InSourceString, ToReplace, InReplaced), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Reverse(const FString& InSourceString, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("REVERSE('%s')"), *InSourceString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Space(const int32 InSpaceNumber, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("SPACE(%i)"), InSpaceNumber), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Strcmp(const FString& S1, const FString& S2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug )
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("STRCMP('%s','%s')"), *S1,*S2), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::YearWeek(const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("YEARWEEK('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::WeekOfYear(const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("WEEKOFYEAR('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::WeekDay(const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("WEEKDAY('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Year(const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("YEAR('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Week(const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("WEEK('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::ToDays(const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("TO_DAYS('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::TimeStamp(const FString& InExpression, const FString& InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("TIMESTAMP('%s','%s')"), *InExpression,*InInterval), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::TimeDiff(const FString& InTime1, const FString& InTime2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("TIMEDIFF('%s','%s')"), *InTime1, *InTime2), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::TimeToSec(const FString& InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("TIME_TO_SEC('%s')"), *InTime), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::TimeFormat(const FString& InTime, const FString& InFormat, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("TIME_FORMAT('%s')"), *InTime,*InFormat), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Time(const FString& InExpression, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("TIME('%s')"), *InExpression), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::SysDate(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(TEXT("SYSDATE('%s')"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::SubTime(const FString& InTime, int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("SUBTIME('%s','%ll')"),*InTime, InSecond),SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::SubDate(const FString& InDate, int64 InDay, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("SUBDATE('%s','%ll')"), *InDate, InDay), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::StringToDate(const FString& InString, const FString& InFormatMask, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("STR_TO_DATE('%s','%ll')"), *InString, *InFormatMask), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::SecToTime(const int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("SEC_TO_TIME('%ll')"), InSecond), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Second(const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("SECOND('%s')"), *InTime), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Quarter(const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("QUARTER('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::PeriodDiff(const int64 Period1, const int64 Period2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("PERIOD_DIFF('%ll','%ll')"),Period1 ,Period2), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::PeriodAdd(const int64 InPeriod, const int64 InMonths, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("PERIOD_ADD('%ll','%ll')"), InPeriod, InMonths), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Now(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(TEXT("NOW()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Minute(const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("MINUTE('%s')"),*InTime), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::MicroSecond(const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("MICROSECOND('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::MakeTime(const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("MAKETIME('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::MakeDate(const int32 InYear, const int64 DayOfYear, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("MAKEDATE('%i','%ll')"), InYear, DayOfYear), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::LocalTimeStamp(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(TEXT("LOCALTIMESTAMP()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::LocalTime(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(TEXT("LOCALTIME()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::LastDay(const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("LAST_DAY('%s')"),*InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Hour(const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("HOUR('%s')"), *InTime), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::FromDays(const int64 InDay, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("FROM_DAYS('%ll')"), InDay), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Extract(const FString &InDate, const EMysqlDateType DateType, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	UEnum *TypeEnum = StaticEnum<EMysqlDateType>();
	FString DT = TypeEnum->GetNameStringByIndex((int32)DateType);

	return GetSelectTableDataByFuntion(FString::Printf(TEXT("EXTRACT( %s FROM '%s')"),*DT,*InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::DayOfYear(const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DAYOFYEAR('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::DayOfWeek(const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DAYOFWEEK('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::DayOfMonth(const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DAYOFMONTH('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::DayName(const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DAYNAME('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Day(const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DAY('%s')"), *InDate), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::DateSub(const FString& InDate, const int64 InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DATE_SUB('%s', INTERVAL %ll DAY)"), *InDate, InInterval), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::DateFormat(const FString& InDate, const FString& InFormat, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DATE_FORMAT('%s','%s')"), *InDate,*InFormat), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::AddDate(const FString& InDate, const int64 InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("ADDDATE('%s','%ll')"), *InDate, InInterval), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::DateDiff(const FString& InD1, const FString& InD2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DATEDIFF('%s','%s')"), *InD1, *InD2), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Date(const FString& InExpression, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DATE('%s')"),*InExpression), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::CurTime(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(TEXT("CURTIME()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::CurrentTime(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(TEXT("CURRENT_TIME()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::CurrentTimeStamp(TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(TEXT("CURRENT_TIMESTAMP()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::AddTime(const FString& InDate, const int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("DATEDIFF('%s','%ll')"),*InDate, InSecond), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetSelectVersion(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(TEXT("VERSION()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetSelectDatabase(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(TEXT("DATABASE()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetSelectUser(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(TEXT("USER()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetShowStatus(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return QueryLinkResult(TEXT("SHOW STATUS;"), Results, ErrMesg, SaveType, Debug);
}

bool FSimpleMysqlLink::GetShowVariables(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return QueryLinkResult(TEXT("SHOW VARIABLES;"), Results, ErrMesg, SaveType, Debug);
}

bool FSimpleMysqlLink::Bin(const int64 Value, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult& Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("BIN('%ll')"), Value), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Binary(const FString& Value, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("BINARY('%s')"), *Value), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Cast(const FString& Value, EMysqlVariableType VariableType, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	UEnum* MysqlVariableEnum = StaticEnum<EMysqlVariableType>();
	FString FieldTypeString = MysqlVariableEnum->GetNameStringByIndex((int32)VariableType);

	return GetSelectTableDataByFuntion(FString::Printf(TEXT("CAST('%s' AS '%s')"), *Value, *FieldTypeString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Conv(const FString& Value, int64 F1, int64 F2, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("Conv('%s','%ll','%ll')"), *Value, F1,F2), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Convert(const FString& InString, EMysqlCharset Charset, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	UEnum* MysqlVariableEnum = StaticEnum<EMysqlCharset>();
	FString TypeString = MysqlVariableEnum->GetNameStringByIndex((int32)Charset);

	return GetSelectTableDataByFuntion(FString::Printf(TEXT("CONVERT('%s' USING '%s')"), *InString,*TypeString), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::BinocularOperation(const FString& Expression, const FString& ValueA, const FString& ValueB, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("IF(%s,'%s','%s')"), *Expression, *ValueA, *ValueB), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::IsNULL(const FString& Expression, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("ISNULL(%s)"), *Expression), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::NULLIf(const FString& ValueA, const FString& ValueB, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("NULLIF('%s','%s')"), *ValueA,*ValueB), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::Charset(const FString& Value, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(FString::Printf(TEXT("CHARSET('%s')"), *Value), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetConnection_ID(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(TEXT("CONNECTION_ID()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetCurrent_user(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(TEXT("CURRENT_USER()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetLast_Insert_ID(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(TEXT("LAST_INSERT_ID()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetSession_user(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(TEXT("SESSION_USER()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::GetSystem_user(TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/, const FSimpleMysqlDebugResult &Debug /*= FSimpleMysqlDebugResult()*/)
{
	return GetSelectTableDataByFuntion(TEXT("SYSTEM_USER()"), SaveType, Results, ErrMesg, Debug);
}

bool FSimpleMysqlLink::StartTransaction(FString &ErrorMsg)
{
	FString SQL = TEXT("START TRANSACTION;");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::SetAutoCommit(bool bAuto, FString &ErrorMsg)
{
	FString SQL = TEXT("SET AUTOCOMMIT = ") + FString::Printf(TEXT("%i"), bAuto)+ TEXT(";");	
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::Commit(FString &ErrorMsg)
{
	FString SQL = TEXT("COMMIT;");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::SetSavePointName(const FString &SaveName, FString &ErrorMsg)
{
	FString SQL = TEXT("SAVEPOINT ") + SaveName;
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::ReleaseSavePoint(const FString &SaveName, FString &ErrorMsg)
{
	FString SQL = TEXT("RELEASE SAVEPOINT ") + SaveName + TEXT(";");
	return QueryLink(SQL, ErrorMsg);
}

bool FSimpleMysqlLink::Rollback(const FString &SaveName, FString &ErrorMsg)
{
	FString SQL = TEXT("ROLLBACK ");
	if (!SaveName.IsEmpty())
	{
		SQL += TEXT("TO ") + SaveName;
	}
	SQL += TEXT(";");

	return QueryLink(SQL, ErrorMsg);
}

uint32 FSimpleMysqlLink::ToMySqlClientFlag(ESimpleClientFlags ClientFlags) const
{
	switch (ClientFlags)
	{
	case ESimpleClientFlags::Client_Long_Password:
		return 1;
	case ESimpleClientFlags::Client_Found_Rows:
		return 2;
	case ESimpleClientFlags::Client_Long_Flag:
		return 4;
	case ESimpleClientFlags::Client_Connect_With_db:
		return 8;
	case ESimpleClientFlags::Client_No_Schema:
		return 16;
	case ESimpleClientFlags::Client_Compress:
		return 32;
	case ESimpleClientFlags::Client_ODBC:
		return 64;
	case ESimpleClientFlags::Client_Local_Files:
		return 128;
	case ESimpleClientFlags::Client_Ignore_Space:
		return 256;
	case ESimpleClientFlags::Client_Protocol_41:
		return 512;
	case ESimpleClientFlags::Client_Interactive:
		return 1024;
	case ESimpleClientFlags::Client_SSL:
		return 2048;
	case ESimpleClientFlags::Client_Ignore_Sigpipe:
		return 4096;
	case ESimpleClientFlags::Client_Transactions:
		return 8192;
	case ESimpleClientFlags::Client_Reserved:
		return 16384;
	case ESimpleClientFlags::Client_Reserved2:
		return 32768;
	case ESimpleClientFlags::Client_Multi_Statements:
		return (1UL << 16);
	case ESimpleClientFlags::Client_Multi_Results:
		return (1UL << 17);
	case ESimpleClientFlags::Client_PS_Multi_Results:
		return (1UL << 18);
	}

	return 0;
}

void FSimpleMysqlLink::GetResult(st_mysql_res *Result, TArray<FSimpleMysqlResult> &Results)
{
	int32 NumRow = mysql_num_fields(Result);
	while (MYSQL_ROW SQLRow = mysql_fetch_row(Result))
	{
		Results.Add(FSimpleMysqlResult());
		FSimpleMysqlResult *R = &Results[Results.Num() - 1];

		FString RowString;
		for (int i = 0; i < NumRow; i++)
		{
			if (SQLRow[i] != NULL)
			{
				char* o = SQLRow[i];

				//It has to be because of the garbled code
				std::string c_name = mysql_fetch_field_direct(Result, i)->name;
				std::string c_value = SQLRow[i];

				R->Rows.Add(UTF8_TO_TCHAR(c_name.c_str()) , UTF8_TO_TCHAR(c_value.c_str()));
			}
			else
			{
				//It has to be because of the garbled code
				std::string c_name = mysql_fetch_field_direct(Result, i)->name;

				R->Rows.Add(c_name.c_str(), TEXT("NULL"));
			}
		}
	}
}

bool FSimpleMysqlLink::GetSelectTableDataSQL(FString &SQL,const FSimpleMysqlQueryParameters &QueryParam)
{
	bool bSelectTableDataSQLSuccess = QueryParam.ToString(SQL);
	SQL += TEXT(";");

	return bSelectTableDataSQLSuccess;
}

void FSimpleMysqlLink::FBindAllocation::Free()
{
	if (Size != 0)
	{
		uint8 *Byteode = (uint8*)BindParam;
		for (uint32 i = 0;i < Size;i++)
		{
			MYSQL_BIND * TmpBind = (MYSQL_BIND*)Byteode;

			FMemory::Free(TmpBind->buffer);
			TmpBind->buffer = nullptr;

			FMemory::Free(TmpBind->length);
			TmpBind->length = nullptr;

			FMemory::Free(TmpBind);

			Byteode += sizeof(MYSQL_BIND);
		}

		Size = 0;
		BindParam = nullptr;
	}
}
#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif