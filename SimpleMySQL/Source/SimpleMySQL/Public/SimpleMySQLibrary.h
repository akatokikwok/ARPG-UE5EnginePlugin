// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "Core/SimpleMysqlLinkType.h"
#include "SimpleMySQLibrary.generated.h"

class USimpleMysqlObject;

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "MySQLLibrary"))
class SIMPLEMYSQL_API USimpleMySQLLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (Keywords = "SimpleMySQL sample test testing"), Category = "SimpleMySQLTesting")
	static void MySqlMain();

	//MySQL object operation
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "SimpleMySQL"))
	static USimpleMysqlObject *CreateMysqlObject(
			UObject* WorldContextObject,
			const FString &InUser,
			const FString &InHost,
			const FString &InPawd,
			const FString &InDB,
			const int32  InPort,
			const TArray<ESimpleClientFlags> &InClientFlag,
			const FString &Unix_Socket = TEXT(""));

	/**
	 * Preprocessing query statements can be used to preprocess queries.
	 * The most commonly used method is to prepare statements and insert statements.
	 * The inserted contents can be replaced by question marks. The good thing to do is to prevent injection attacks .
	 * @param SQL					SQL complete query statement.
	 * @param InQueryStmtParam		Preprocessing of parameter insertion statement .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * Return whether the query is successful.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool QueryLinkStmt(USimpleMysqlObject *Object, const FString& SQL, const TArray<FSimpleMysqlQueryStmt>& InQueryStmtParam, FString& ErrMesg);

	//Operation of database
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CreateDataBase(USimpleMysqlObject *Object, const FString &DataBaseName, EMysqlCharset Charset, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CreateAndSelectDataBase(USimpleMysqlObject *Object, const FString &DataBaseName, EMysqlCharset Charset, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool SelectNewDB(USimpleMysqlObject *Object, const FString &NewDB, FString &ErrMesg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool DropDataBase(USimpleMysqlObject *Object, const FString &DataBaseName, FString &ErrorMsg);

	//Table operation
	//////////////////////////////////////////////////////////////////////////
	//Create table
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CreateTable(USimpleMysqlObject *Object, const FString &TableName, const TMap<FString, FMysqlFieldType> &InFields, const TArray<FString> &InPrimaryKeys, const FMysqlCreateTableParam &Param, FString &ErrorMsg);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CreateTemporaryTable(USimpleMysqlObject *Object, const FString &TableName, const TMap<FString, FMysqlFieldType> &InFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CopyTableAToB(USimpleMysqlObject *Object, const FString &A, const FString &B, const TArray<FString> &Fields, FString &ErrorMsg);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CopyTableAToTemporaryB(USimpleMysqlObject *Object, const FString &TableA, const FString &TemporaryTableB, const TArray<FString> &Fields, FString &ErrorMsg);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ShowTables(USimpleMysqlObject *Object, const FString &TableName, EMysqlQuerySaveType SaveType, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CopyTableAToBFast(USimpleMysqlObject *Object, const FString &CopySourceTable, const TMap<FString, FMysqlFieldType> &InsertFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CopyTableAToTemporaryBFast(USimpleMysqlObject *Object, const FString &CopySourceTable, const TMap<FString, FMysqlFieldType> &InsertFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool DropTemporaryTable(USimpleMysqlObject *Object, const FString &TemporaryTableName, FString &ErrorMsg);

	//Deleting all tables to release memory does not support rolling back InnoDB and MyISAM to clear all tables
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool DropTable(USimpleMysqlObject *Object, const FString &TableName, FString &ErrorMsg);

	//Clearing table to release memory does not support rollback InnoDB and MyISAM clearing all
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool TruncateTable(USimpleMysqlObject *Object, const FString &TableName, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool DeleteFromTable(USimpleMysqlObject *Object, const FString &TableName, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool DeleteFromTableWhereData(USimpleMysqlObject *Object, const FString &TableName, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool OptimiseTable(USimpleMysqlObject *Object, const FString &TableName, FString &ErrorMsg);

	//Metadata operation
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetSelectVersion(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType = EMysqlQuerySaveType::STORE_RESULT);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetSelectDatabase(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType = EMysqlQuerySaveType::STORE_RESULT);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetSelectUser(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType = EMysqlQuerySaveType::STORE_RESULT);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetShowStatus(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType = EMysqlQuerySaveType::STORE_RESULT);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetShowVariables(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType = EMysqlQuerySaveType::STORE_RESULT);
	
	//Field operation
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool IsFieldExistInTable(USimpleMysqlObject *Object, const FString &InTableName, const FString &InFieldName, FString &ErrMesg);
	//query
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool GetSelectTableData(USimpleMysqlObject *Object,const FSimpleMysqlQueryParameters &QueryParam, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool GetSelectTableDataSQL(USimpleMysqlObject *Object, FString &SQL,const FSimpleMysqlQueryParameters &QueryParam);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool QueryLinkResult(USimpleMysqlObject *Object, const FString &SQL, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool QueryLink(USimpleMysqlObject *Object, const FString &SQL, FString &ErrMesg);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool PrintResult(USimpleMysqlObject *Object, const TArray<FSimpleMysqlResult> &Results, bool bPrintToScreen = true, bool bPrintToLog = true);

	//Update
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool UpdateTableData(USimpleMysqlObject *Object, const FString &TableName, const TArray<FSimpleMysqlAssignment> &InFields, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ReplaceTableData(USimpleMysqlObject *Object, const FString &TableName, const TArray<FSimpleMysqlReplaceParameters> &InReplaces, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg);
	
	//Insert
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool InsertTableData(USimpleMysqlObject *Object, const FString& TableName, const TMap<FString, FString> &InsetKeyValueDatas, const FSimpleMysqlQueryParameters &QueryParam, FString& ErrorMsg, bool bIgnore = false);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool SimpleInsertTableData(USimpleMysqlObject *Object, const FString& TableName, const TArray<FString> &InsetValueDatas, const FSimpleMysqlQueryParameters &QueryParam, FString& ErrorMsg, bool bIgnore = false);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ReplaceIfExistence(USimpleMysqlObject *Object, const FString& TableName, const TMap<FString, FString> &InsetKeyValueDatas, FString& ErrorMsg);

	//Transaction operation
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool StartTransaction(USimpleMysqlObject *Object, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool SetAutoCommit(USimpleMysqlObject *Object, bool bAuto, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool Commit(USimpleMysqlObject *Object, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool SetSavePointName(USimpleMysqlObject *Object, const FString &SaveName, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool Rollback(USimpleMysqlObject *Object, const FString &SaveName, FString &ErrorMsg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ReleaseSavePoint(USimpleMysqlObject *Object, const FString &SaveName, FString &ErrorMsg);

	//Custom function
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CreateFunction(USimpleMysqlObject *Object, const FString &InFunctionName, const TMap<FString, FMysqlFieldType> &InParameter, const FString &InFunctionBody, FString& ErrMesg, EMysqlVariableType InReturnsType = EMysqlVariableType::MYSQL_LONG, const FString &InComment = TEXT(""));
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ExecuteFunction(USimpleMysqlObject *Object, const FString &InFunctionName, const TArray<FString> &InParameter, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool DropFunction(USimpleMysqlObject *Object, const FString &InFunctionName, FString& ErrMesg);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ShowCreateFunction(USimpleMysqlObject *Object, const FString &InFunctionName, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ShowFunctionsStatus(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	//Custom stored procedure
	/**
	 * Define our MySQL variables .
	 * @param InVariablesName		Describe what our variables are called .
	 * @param InValue				Value of variable specific initialization.
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * Return whether the query is successful.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool DefiningVariables(USimpleMysqlObject *Object, const FString &InVariablesName, const FString &InValue, FString &ErrMesg);

	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool CreateProcedure(USimpleMysqlObject *Object, const FString &InProcedureName, const TMap<FString, FMysqlFieldType> &InParameter, const FString &InProcedureBody, FString& ErrMesg, const FString &InComment = TEXT(""));
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ExecuteProcedure(USimpleMysqlObject *Object, const FString &InProcedureName, const TArray<FString> &InParameter, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool DropProcedure(USimpleMysqlObject *Object, const FString &InProcedureName, FString& ErrMesg);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ShowCreateProcedure(USimpleMysqlObject *Object, const FString &InProcedureName, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);
	
	UFUNCTION(BlueprintCallable, meta = (Category = "SimpleMySQL"))
	static bool ShowProceduresStatus(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);
//////////////////////////////////////////////////////////////////////////
	/**
	 * Return the year and week (0 to 53), 0 in mode means Sunday, 1 means Monday, and so on .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT YEARWEEK("2017-06-15");
		-> 201724
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool YearWeek(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Calculation date InDate is the week ordinal of the year, ranging from 0 to 53 (About fifty-three weeks a year).
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT WEEKOFYEAR('2011-11-11 11:11:11')
		-> 45
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool WeekOfYear(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Date InDate is the day of the week, 0 is Monday, 1 is Tuesday
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT WEEKDAY("2017-06-15");
		-> 3
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool WeekDay(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Return year .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT YEAR("2017-06-15");
		-> 2017
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Year(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Calculation date InDate is the week ordinal of the year, ranging from 0 to 53 (About fifty-three weeks a year).
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT WEEK('2011-11-11 11:11:11')
		-> 45
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Week(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Days from calculation date d to January 1, 0000.
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT TO_DAYS('0001-01-01 01:01:01')
		-> 366
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool ToDays(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * When a single parameter is used, the function returns a date or date time expression; when there are 2 parameters, the parameters are added.
	 * @param InExpression			Expression of statement or fill in date similar to 2022-10-12 10:25:23.
	 * @param InInterval			Time like content .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT TIMESTAMP("2017-07-23",  "13:10:11");
		-> 2017-07-23 13:10:11
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool TimeStamp(USimpleMysqlObject *Object, const FString& InExpression, const FString& InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Calculate time difference .
	 * @param InTime1				Given custom time 1.
	 * @param InTime2				Given custom time 2.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT TIMESTAMP("2017-07-23",  "13:10:11");
		-> 2017-07-23 13:10:11
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool TimeDiff(USimpleMysqlObject *Object, const FString& InTime1, const FString& InTime2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Convert time InTime to seconds .
	 * @param InTime				Time to convert.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT TIME_TO_SEC('1:12:00')
		-> 4320
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool TimeToSec(USimpleMysqlObject *Object, const FString& InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Display time InTime as required by expression format.
	 * @param InTime				Time to convert.
	 * @param InFormat				You can customize the layout format, such as %M %Y %D %r, etc .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT TIME_FORMAT('11:11:11','%r')
		-> 11:11:11 AM
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool TimeFormat(USimpleMysqlObject *Object, const FString& InTime, const FString& InFormat, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Extract the time part of the incoming expression .
	 * @param InExpression			Expression of statement or fill in date similar to 2022-10-12 10:25:23.
	 * @param InFormat				You can customize the layout format, such as %M %Y %D %r, etc .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT TIME("19:30:10");
		-> 19:30:10
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Time(USimpleMysqlObject *Object, const FString& InExpression, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the current date and time .
	 * @param InFormat				You can customize the layout format, such as %M %Y %D %r, etc .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT SYSDATE()
		-> 2020-06-19 20:59:41
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool SysDate(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Time InTime minus InSecond seconds  .
	 * @param InTime				Specific time like 2011-11-11 11:11:11.
	 * @param InSecond				Time seconds .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT SUBTIME('2011-11-11 11:11:11', 5)
		->2011-11-11 11:11:06 (Second)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool SubTime(USimpleMysqlObject *Object, const FString& InTime, int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Date InDate minus InDay days .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param InDay					Time day .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT SUBDATE('2011-11-11 11:11:11', 1)
		->2011-11-10 11:11:11 (Default is day)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool SubDate(USimpleMysqlObject *Object, const FString& InDate, int64 InDay, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Convert string to date .
	 * @param InString				Text description week like august 10 2020.
	 * @param InFormatMask			You can customize the layout format, such as %M %Y %D %r, etc .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT STR_TO_DATE("August 10 2017", "%M %d %Y");
		-> 2017-08-10
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool StringToDate(USimpleMysqlObject *Object, const FString& InString, const FString& InFormatMask, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Converts time s in seconds to minutes and seconds .
	 * @param InSecond				Time seconds .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT SEC_TO_TIME(4320)
		-> 01:12:00
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool SecToTime(USimpleMysqlObject *Object, const int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the second value in InTime.
	 * @param InTime				Specific time like 2011-11-11 11:11:11.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT SECOND('1:2:3')
		-> 3
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Second(USimpleMysqlObject *Object, const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Return date D is the season, return 1 to 4 .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT QUARTER('2011-11-11 11:11:11')
		-> 4
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Quarter(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the month difference between two periods.
	 * @param Period1				Month compared 1.
	 * @param Period2				Month compared 2.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT PERIOD_DIFF(201710, 201703);
		-> 7
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool PeriodDiff(USimpleMysqlObject *Object, const int64 Period1, const int64 Period2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Add a period for the year month combination date.
	 * @param InPeriod				A year with numbers 202006.
	 * @param InMonths				Months.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT PERIOD_ADD(201703, 5);
		-> 201708
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool PeriodAdd(USimpleMysqlObject *Object, const int64 InPeriod, const int64 InMonths, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Get current time .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT NOW()
		-> 2020-09-19 20:57:41
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Now(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the minute value in InTime.
	 * @param InTime				Specific time like 2011-11-11 11:11:11.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT MINUTE('1:2:3')
		-> 2
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Minute(USimpleMysqlObject *Object, const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the number of microseconds corresponding to the date parameter .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT MICROSECOND("2020-06-20 09:34:00.000023");
		-> 23
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool MicroSecond(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Combination time, parameters are hour, minute and second respectively .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT MAKETIME(11, 35, 4);
		-> 11:35:04
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool MakeTime(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns a date based on the given parameter year and the number of days in the year day of year .
	 * @param InYear				Like 2020
	 * @param DayOfYear				Like 1 ~ 12
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT MAKEDATE(2020, 3);
		-> 2020-01-03
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool MakeDate(USimpleMysqlObject *Object, const int32 InYear, const int64 DayOfYear, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the current date and time .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT LOCALTIMESTAMP()
		-> 2018-09-19 20:57:43
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool LocalTimeStamp(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the current date and time .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT LOCALTIME()
		-> 2018-09-19 20:57:43
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool LocalTime(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * The last day of the month returned to the given date .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT LAST_DAY("2020-06-20");
		-> 2020-06-30
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool LastDay(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the hour value in InTime.
	 * @param InTime				Specific time like 11:11:11.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT HOUR('1:2:3')
		-> 1
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Hour(USimpleMysqlObject *Object, const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Calculate the date n days after January 1, 0000 .
	 * @param InTime				Specific time like 11:11:11.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT FROM_DAYS(1111)
		-> 0003-01-16
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool FromDays(USimpleMysqlObject *Object, const int64 InDay, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Gets the specified value from date D, and type specifies the returned value .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param DateType				About the type of date .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT EXTRACT(MINUTE FROM '2011-11-11 11:11:11')
		-> 11
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Extract(USimpleMysqlObject *Object, const FString &InDate, const EMysqlDateType DateType, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Calculation date InDate is the day of the year .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT DAYOFYEAR('2011-11-11 11:11:11');
		->315
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool DayOfYear(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Date InDate today is the day of the week, 1 Sunday, 2 Monday, and so on .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT DAYOFWEEK('2011-11-11 11:11:11')
		->6
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool DayOfWeek(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Calculation date InDate is the day of the month .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT DAYOFMONTH('2011-11-11 11:11:11')
		->11
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool DayOfMonth(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * The return date InDate is the day of the week, such as Monday, Tuesday .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT DAYNAME('2011-11-11 11:11:11')
		->Friday
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool DayName(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the date part of the date value InDate.
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT DAY("2017-06-15");
		-> 15
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Day(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Function to subtract a specified time interval from a date.
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param InInterval			Time interval in days.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		//Subtract 2 days from the OrderDate field in the orders table:
		SELECT OrderId,DATE_SUB(OrderDate,INTERVAL 2 DAY) AS OrderPayDate FROM Orders
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool DateSub(USimpleMysqlObject *Object, const FString& InDate, const int64 InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Display date InDate as required by expression InFormat.
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param InFormat				You can customize the layout format, such as %M %Y %D %r, etc .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT DATE_FORMAT('2011-11-11 11:11:11','%Y-%m-%d %r')
		-> 2011-11-11 11:11:11 AM
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool DateFormat(USimpleMysqlObject *Object, const FString& InDate, const FString& InFormat, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Date after calculation start date D plus a time period .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23.
	 * @param InInterval			Time interval in days.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT ADDDATE('2011-11-11 11:11:11',1)
		-> 2011-11-12 11:11:11    (Default is day)
		SELECT ADDDATE('2011-11-11 11:11:11', INTERVAL 5 MINUTE)
		-> 2011-11-11 11:16:11 (The value of type is similar to the function listed above)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool AddDate(USimpleMysqlObject *Object, const FString& InDate, const int64 InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Days between calculation dates InD1 - > InD2 .
	 * @param InD1					Fill in date similar to 2022-10-12 10:25:23.
	 * @param InD2					Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT DATEDIFF('2001-01-01','2001-02-02')
		-> -32
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool DateDiff(USimpleMysqlObject *Object, const FString& InD1, const FString& InD2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Extract a date value from a date or datetime expression .
	 * @param InExpression			About date expression.
	 * @param InD2					Fill in date similar to 2022-10-12 10:25:23.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT DATE("2017-06-15");
		-> 2017-06-15
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool Date(USimpleMysqlObject *Object, const FString& InExpression, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Return to current time .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CURTIME();
		-> 19:59:02
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool CurTime(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the current date and time .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CURRENT_TIME();
		-> 19:59:02
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool CurrentTime(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the current date and time .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CURRENT_TIMESTAMP()
		-> 2018-09-19 20:57:43
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool CurrentTimeStamp(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Time InDate plus InSecond seconds .
	 * @param InDate				Fill in date similar to 2022-10-12 10:25:23..
	 * @param InSecond				Time seconds .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT ADDTIME('2011-11-11 11:11:11', 5)
		->2011-11-11 11:11:16 (Second)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|Date"))
	static bool AddTime(USimpleMysqlObject *Object, const FString& InDate, const int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	//函数操作
	//////////////////////////////////////////////////////////////////////////
	/**
	 * Binary encoding of return x .
	 * @param Value					Number written .
	 * @param InSecond				Time seconds .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT BIN(15);
		-> 1111
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool Bin(USimpleMysqlObject* Object, const int64 Value, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);

	/**
	 * Converts the string s to a binary string .
	 * @param Value					String to be converted to binary .
	 * @param InSecond				Time seconds .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT BINARY "Renzhai";
		-> Renzhai
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool Binary(USimpleMysqlObject* Object, const FString& Value, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);

	/**
	 * Shifting clause .
	 * @param Value					String to be converted .
	 * @param InSecond				Time seconds .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CAST("2018-08-29" AS DATE);
		-> 2018-08-29
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool Cast(USimpleMysqlObject* Object, const FString& Value, EMysqlVariableType VariableType, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Return F1 base number to F2 base number .
	 * @param Value					Specific value .
	 * @param F1					Custom base conversion rules .
	 * @param F2					Custom base conversion rules .
	 * @param InSecond				Time seconds .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CONV(15, 10, 2);
		-> 1111
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool Conv(USimpleMysqlObject* Object, const FString& Value, int64 F1, int64 F2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);

	/**
	 * Function to change the character set of string s to CS .
	 * @param InString				String to be converted .
	 * @param Charset				Converted character set .
	 * @param InSecond				Time seconds .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CHARSET('ABC')
		->utf-8
		SELECT CHARSET(CONVERT('ABC' USING gbk))
		->gbk
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool Convert(USimpleMysqlObject* Object, const FString& InString, EMysqlCharset Charset, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * If the expression expr holds, the result V1 is returned; otherwise, the result V2 is returned.
	 * @param Expression			Based on expression .
	 * @param ValueA				A.
	 * @param ValueB				B.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT IF(1 > 0,'Y','F')
		->Y
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool BinocularOperation(USimpleMysqlObject* Object, const FString& Expression, const FString& ValueA, const FString& ValueB, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);
	
	/**
	 * If the value of V1 is not null, V1 is returned, otherwise V2 is returned. 
	 * @param Expression			Based on expression .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT IFNULL(null,'Hello Word')
		->Hello Word
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool IsNULL(USimpleMysqlObject* Object, const FString& Expression, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);
	
	/**
	 * Compare two strings. If the strings expr1 and expr2 are equal, null is returned. Otherwise, expr1 is returned. 
	 * @param ValueA				Comparison value.
	 * @param ValueB				Compared value.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT NULLIF(25, 25);
		->
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool NULLIf(USimpleMysqlObject* Object, const FString& ValueA, const FString& ValueB, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);
	
	/**
	 * character set . 
	 * @param InExpression			Expression.
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CHARSET(CONVERT('ABC' USING gbk))
		->gbk
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool Charset(USimpleMysqlObject* Object, const FString& Value, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the number of connections to the server . 
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CONNECTION_ID();
		-> 1
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetConnection_ID(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug);

	/**
	 * Return to current user .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CURRENT_USER();
		-> root@loclhost
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetCurrent_user(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);

	/**
	 * Returns the most recently generated Auto_ Increment value .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT LAST_INSERT_ID();
		->6
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetLast_Insert_ID(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);

	/**
	 * Return to current user .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CURRENT_USER();
		-> root@loclhost
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetSession_user(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);

	/**
	 * Return to current user .
	 * @param Results				The result set is used for content obtained through MySQL server .
	 * @param ErrMesg				Error message if there is an error or warning in the query, MySQL will be notified in the form of text.
	 * @param SaveType				Whether you want to read the query results from the local cache or get the query results from the server through the network.
	 * @param Debug					It is used for testing in UE, whether to print to screen or log or to process all .
	 * Return whether the query is successful.
	 * @Example :
		SELECT CURRENT_USER();
		-> root@loclhost
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static bool GetSystem_user(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType , const FSimpleMysqlDebugResult& Debug);

	//结构NULL值
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable,BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static TArray<FString> GetStringNULLArray();

	UFUNCTION(BlueprintCallable, BlueprintPure,meta = (Category = "SimpleMySQL|NULL"))
	static TArray<FSimpleMysqlQueryOrderBy> GetOrderByNULLArray();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static TArray<FSimpleMysqlJoinParameters> GetJoinParamNULLArray();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static TArray<FSimpleMysqlAssignment> GetAssignmentNULLArray();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static TArray<FSimpleMysqlComparisonOperator> GetComparisonOperatorNULLArray();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static TArray<FSimpleMysqlReplaceParameters> GetReplaceParametersNULLArray();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static TMap<FString, FString> GetMapStringNULL();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static FSimpleMysqlQueryParameters GetNULLQueryParameters();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static TMap<FString, FMysqlFieldType> GetFieldNULL(); 

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static FSimpleMysqlJoinParameters GetJoinParametersNULL();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "SimpleMySQL|NULL"))
	static FSimpleMysqlModifyField GetModifyFieldNULL();
};