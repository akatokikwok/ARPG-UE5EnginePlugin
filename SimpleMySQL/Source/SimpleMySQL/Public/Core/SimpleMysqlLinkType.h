// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleMysqlLinkType.generated.h"

//Mysql所有的变量类型
UENUM(BlueprintType)
enum class EMysqlVariableType :uint8
{
	MYSQL_DECIMAL							 UMETA(DisplayName = "Decimal"),	//DECIMAL(M,D)/NUMERIC  M > D ? M+ 2 : D+ 2
	MYSQL_TINY								 UMETA(DisplayName = "Small Int"),	//SMALLINT		2 //s
	MYSQL_SHORT								 UMETA(DisplayName = "Short"),		//SHORT			2 //s
	MYSQL_LONG								 UMETA(DisplayName = "integer"),	//INTEGER		4 //s
	MYSQL_FLOAT								 UMETA(DisplayName = "Float"),		//FLOAT			4 //s
	MYSQL_DOUBLE							 UMETA(DisplayName = "Double"),		//DOUBLE/REAL	8 //s
	MYSQL_NULL								 UMETA(DisplayName = "Null"),		//NULL
	MYSQL_TIMESTAMP							 UMETA(DisplayName = "Time Stamp"), //TIMESTAMP		8
	MYSQL_LONGLONG							 UMETA(DisplayName = "Big Int"),	//BIGINT		8 //s
	MYSQL_INT24								 UMETA(DisplayName = "Medium Int"),	//MEDIUMINT		3 //ns
	MYSQL_DATE								 UMETA(DisplayName = "Date"),		//DATE			3
	MYSQL_TIME								 UMETA(DisplayName = "Time"),		//TIME			3
	MYSQL_DATETIME							 UMETA(DisplayName = "Date Time"),	//DATETIME		8
	MYSQL_YEAR								 UMETA(DisplayName = "Year"),		//YEAR			1
	MYSQL_NEWDATE							 UMETA(DisplayName = "New Date"),	//NEWDATE
	MYSQL_VARCHAR							 UMETA(DisplayName = "Varchar"),	//VARCHAR
	MYSQL_BIT								 UMETA(DisplayName = "Bit"),		//BIT(M)			
	MYSQL_TIMESTAMP2						 UMETA(DisplayName = "Time Stamp2"),
	MYSQL_DATETIME2							 UMETA(DisplayName = "Date Time2"),
	MYSQL_TIME2								 UMETA(DisplayName = "Time2"),
	MYSQL_JSON			= 245				 UMETA(DisplayName = "Json"),		//@@
	MYSQL_NEWDECIMAL	= 246				 UMETA(DisplayName = "New Decimal"),//
	MYSQL_ENUM			= 247				 UMETA(DisplayName = "Enum"),		//				1 or 2   65535个
	MYSQL_SET			= 248				 UMETA(DisplayName = "Set"),		//				1,2,4,8
	MYSQL_TINY_BLOB		= 249				 UMETA(DisplayName = "Tiny Blob"),  //@@				//s
	MYSQL_MEDIUM_BLOB	= 250				 UMETA(DisplayName = "Medium Blob"),//@@				//s
	MYSQL_LONG_BLOB		= 251				 UMETA(DisplayName = "Long Blob"),	//@@				//s
	MYSQL_BLOB			= 252				 UMETA(DisplayName = "Blob"),		//@@ 				//s
	MYSQL_VAR_STRING	= 253				 UMETA(DisplayName = "VarString"),	//@@ //VARCHAR
	MYSQL_STRING		= 254				 UMETA(DisplayName = "String"),		//@@ //CHAR/VARCHAR 
	MYSQL_GEOMETRY		= 255				 UMETA(DisplayName = "Geometry"),	//   Geometry(PINT(经度 维度)) 
};

UENUM(BlueprintType)
enum class EMysqlSaveEngine :uint8
{
	//___________________________________________________________________________________________________________________________________
	//Engine			//|Comment														 |Support	| Transactions  |XA		|Savepoints	|			
	//____________________|_______________________________________________________________________________________________________________
	InnoDB,				//|Supports transactions, row-level locking, and foreign keys	 |	YES		|	YES			|YES	|	YES		|
	MyISAM,				//|MyISAM storage engine										 |	DEFAULT	|	NO			|NO		|	NO		|
	MEMORY,				//|Hash based, stored in memory, useful for temporary tables	 |	YES		|	NO			|NO		|	NO		|
	ARCHIVE,			//|Archive storage engine										 |	YES		|	NO			|NO		|	NO		|
	CSV,				//|CSV storage engine											 |	YES		|	NO			|NO		|	NO		|
	PERFORMANCE_SCHEMA,	//|Performance Schema											 |	YES		|	NO			|NO		|	NO		|
	MRG_MYISAM,			//|Collection of identical MyISAM tables						 |	YES		|	NO			|NO		|	NO		|
	BLACKHOLE,			//|/dev/null storage engine (anything you write to it disappears)|	YES		|	NO			|NO		|	NO		|
	FEDERATED,			//|Federated MySQL storage engine								 |	NO		|	NULL		|NULL	|	NULL	|
	//___________________________________________________________________________________________________________________________________
};

UENUM(BlueprintType)
enum class ESimpleClientFlags :uint8
{
	Client_None									,
	Client_Long_Password						,/* new more secure passwords */
	Client_Found_Rows							,/* Found instead of affected rows */
	Client_Long_Flag							,/* Get all column flags */
	Client_Connect_With_db						,/* One can specify db on connect */
	Client_No_Schema							,/* Don't allow database.table.column */
	Client_Compress								,/* Can use compression protocol */
	Client_ODBC									,/* Odbc client */
	Client_Local_Files							,/* Can use LOAD DATA LOCAL */
	Client_Ignore_Space							,/* Ignore spaces before '(' */
	Client_Protocol_41							,/* New 4.1 protocol */
	Client_Interactive							,/* This is an interactive client */
	Client_SSL								 	,/* Switch to SSL after handshake */
	Client_Ignore_Sigpipe					    ,/* IGNORE sigpipes */
	Client_Transactions							,/* Client knows about transactions */
	Client_Reserved								,/* Old flag for 4.1 protocol  */
	Client_Reserved2						    ,/* Old flag for 4.1 authentication */
	Client_Multi_Statements						,/* Enable/disable multi-stmt support */
	Client_Multi_Results						,/* Enable/disable multi-results */
	Client_PS_Multi_Results						,/* Multi-results in PS-protocol */
};

//
UENUM(BlueprintType)
enum class EMysqlCharset :uint8
{
	None,
	armscii8,
	ascii,
	big5,
	binary,
	cp1250,
	cp1251,
	cp1256,
	cp1257,
	cp850,
	cp852,
	cp866,
	cp932,
	dec8,
	eucjpms,
	euckr,
	gb2312,
	gbk,
	geostd8,
	greek,
	bebrew,
	hp8,
	keybcs2,
	koi8r,
	koi8u,
	latin1,
	latin2,
	latin5,
	latin7,
	macce,
	macroman,
	sjis,
	swe7,
	tis620,
	ucs2,
	ujis,
	utf16,
	utf32,
	utf8,
	utf8mb4
};

//UENUM(BlueprintType)
//enum class EMysqlDirection :uint8
//{
//
//};

UENUM(BlueprintType)
enum class EMysqlDateType :uint8
{
	MICROSECOND				UMETA(DisplayName = "Microsecond"),					
	SECOND					UMETA(DisplayName = "Second"),
	MINUTE					UMETA(DisplayName = "Minute"),
	HOUR					UMETA(DisplayName = "Hour"),
	DAY						UMETA(DisplayName = "Day"),
	WEEK					UMETA(DisplayName = "Week"),
	MONTH					UMETA(DisplayName = "Month"),
	QUARTER					UMETA(DisplayName = "Quarter"),
	YEAR					UMETA(DisplayName = "Year"),
	SECOND_MICROSECOND		UMETA(DisplayName = "Second Microsecond"),
	MINUTE_MICROSECOND		UMETA(DisplayName = "Minute Microsecond"),
	MINUTE_SECOND			UMETA(DisplayName = "Minute Second"),
	HOUR_MICROSECOND		UMETA(DisplayName = "Hour Microsecond"),
	HOUR_SECOND				UMETA(DisplayName = "Hour Second"),
	HOUR_MINUTE				UMETA(DisplayName = "Hour Minute"),
	DAY_MICROSECOND			UMETA(DisplayName = "Day Microsecond"),
	DAY_SECOND				UMETA(DisplayName = "Day Second"),
	DAY_MINUTE				UMETA(DisplayName = "Day Minute"),
	DAY_HOUR				UMETA(DisplayName = "Day Hour"),
	YEAR_MONTH				UMETA(DisplayName = "Year Month")
};

UENUM(BlueprintType)
enum class EMysqlJoinType :uint8
{
	NONE					 UMETA(DisplayName = "None"),
	INNER					 UMETA(DisplayName = "Inner"),
	LEFT 					 UMETA(DisplayName = "Left"),
	RIGHT					 UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)
enum class EMysqlQuerySaveType :uint8
{
	STORE_RESULT  UMETA(DisplayName = "Store Result"),
	USE_RESULT    UMETA(DisplayName = "Use Result"),
};

UENUM(BlueprintType)
enum class EMysqlComparisonOperator :uint8
{
	EQUAL					UMETA(DisplayName = "="),
	GREATER_THAN			UMETA(DisplayName = ">"),
	LESS_THAN				UMETA(DisplayName = "<"),
	GREATER_THAN_OR_EQUAL	UMETA(DisplayName = ">="),
	LESS_THAN_OR_EQUAL		UMETA(DisplayName = "<="),
	NOT_EQUAL				UMETA(DisplayName = "!="),
	BETWEEN					UMETA(DisplayName = ">=min&&<=max"),
	NOT_BETWEEN				UMETA(DisplayName = "!(>=min&&<=max)"),
	SQL_IN					UMETA(DisplayName = "In"),
	NOT_IN					UMETA(DisplayName = "Not In"),
	COMPARE_NULL_VALUES		UMETA(DisplayName = "<=>"),
	LIKE					UMETA(DisplayName = "like"),
	REGEXP_OR_RLIKE			UMETA(DisplayName = "Regular matching"),
	IS_NULL					UMETA(DisplayName = "Is_null"),
	IS_NOT_NULL				UMETA(DisplayName = "Is not null")
};

UENUM(BlueprintType)
enum class EMysqlLogicalOperators :uint8
{
	NONE    UMETA(DisplayName = "NONE"),
	NOT		UMETA(DisplayName = "!"),
	AND		UMETA(DisplayName = "&&"),
	OR		UMETA(DisplayName = "||"),
	XOR		UMETA(DisplayName = "^"),
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FMysqlFieldType
{
	GENERATED_USTRUCT_BODY()

	FMysqlFieldType();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|FieldType")
		bool bUnsignedVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|FieldType")
		EMysqlVariableType VariableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|FieldType")
		int64 VariableLen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|FieldType")
		int64 DecimalPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|FieldType")
		bool bNULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|FieldType")
		FString DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|FieldType")
		bool bAutoIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|FieldType")
		FString Comment;

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FMysqlCreateTableParam
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|CreateTableParam")
	EMysqlSaveEngine SaveEngine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|CreateTableParam")
	EMysqlCharset Charset;

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|MysqlResult")
	TMap<FString,FString> Rows;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlDebugResult
{
	GENERATED_USTRUCT_BODY()

	FSimpleMysqlDebugResult();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|DebugResult", meta = (EditCondition = "bDebug"))
	bool bPrintToScreen; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|DebugResult", meta = (EditCondition = "bDebug"))
	bool bPrintToLog;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlQueryOrderBy
{
	GENERATED_USTRUCT_BODY()

	FSimpleMysqlQueryOrderBy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|OrderBy")
	FString FieldName;

	//asc  //由大到小
	//desc //由小到大
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|OrderBy")
	bool bDesc;
};



USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlJoinParameters
{
	GENERATED_USTRUCT_BODY()

	FSimpleMysqlJoinParameters();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	EMysqlJoinType JoinType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString TableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString Condition;

	FString ToString() const;
};


USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlAssignment
{
	GENERATED_USTRUCT_BODY()

		FSimpleMysqlAssignment();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|Assignment")
		FString A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|Assignment")
		EMysqlComparisonOperator ComparisonOperator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|Assignment")
		FString B;

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlComparisonOperator
{
	GENERATED_USTRUCT_BODY()

		FSimpleMysqlComparisonOperator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|ComparisonOperator")
		FSimpleMysqlAssignment Assignment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|ComparisonOperator")
		EMysqlLogicalOperators LogicalOperators;

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlVariableSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|VariableSettings")
	FString VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|VariableSettings")
	FString AsName;

	bool IsValid() const;

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlQueryParameters
{
	GENERATED_USTRUCT_BODY()

	FSimpleMysqlQueryParameters();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FSimpleMysqlVariableSettings FuntionSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString TableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	TArray<FSimpleMysqlVariableSettings> InFields;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	bool bDistinct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FSimpleMysqlJoinParameters JoinParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	TArray<FSimpleMysqlComparisonOperator> Condition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	TArray<FString> GroupBy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	TArray<FSimpleMysqlQueryOrderBy> OrderBy;

	//x = 第几个,y = 偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FVector2D Limit;

	//是否进行汇总
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	bool bWithRollup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString Having;

	void AddField(const FString &Name);

	bool IsValid() const;

	bool ToString(FString &SQL) const;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlReplaceParameters
{
	GENERATED_USTRUCT_BODY()

	FSimpleMysqlReplaceParameters();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString FieldName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString OldDataValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString NewDataValue;

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlModifyField
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString FieldOldName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FString FieldNewName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryParameters")
	FMysqlFieldType NewFieldType;

	FString ToString() const;
};

//(?,?,?)
USTRUCT(BlueprintType)
struct SIMPLEMYSQL_API FSimpleMysqlQueryStmt
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryStmt")
	FString Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleMySQL|QueryStmt")
	EMysqlVariableType VariableType;

	uint32 GetValueTypeLen() const;

	unsigned long *GetValueTypeLenMemory() const;

	void GetValue(void **InValue) const;

	
};