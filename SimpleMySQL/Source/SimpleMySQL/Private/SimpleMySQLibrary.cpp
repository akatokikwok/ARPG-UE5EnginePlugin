// Copyright (C) RenZhai.2020.All Rights Reserved.
#include "SimpleMySQLibrary.h"
#include "MySQL/mysql.h"
#include "Core/SimpleMysqlLink.h"

//链接测试  查询
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const char *db = "";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	MYSQL_RES *result = NULL;
//
//	int32 result_int = INDEX_NONE;
//
//	mysql_init(&mysql);使用 unix连接方式，unix_socket为null时，表明不使用socket或管道机制，最后一个参数经常设置为0
//	if (mysql_real_connect(&mysql,host,user,pawd,table,port,0,0))//链接
//	{
//		//SELECT * FROM character
//		if ((result_int = mysql_query(&mysql,"SELECT * FROM W")) == 0)
//		{
//			//64MB
//			if ((result = mysql_store_result(&mysql)) != NULL)
//			{
//				while (MYSQL_ROW sql_row = mysql_fetch_row(result))
//				{
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,*FString::Printf(TEXT("name %s"), ANSI_TO_TCHAR(sql_row[0])));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("age %s"), ANSI_TO_TCHAR(sql_row[1])));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("size %s"), ANSI_TO_TCHAR(sql_row[2])));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("hhhhh %s"), ANSI_TO_TCHAR(sql_row[3])));
//				}
//			}
//		}
//		else
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "query sql failed!");
//			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	if (result != NULL)
//	{
//		mysql_free_result(result);
//	}
//
//	mysql_close(&mysql);
//}

//init
//void USimpleMySQLLibrary::MySqlMain()
//{
//	MYSQL mysql;
//	mysql_library_init(NULL,NULL,NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}

//链接
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	/*CLIENT_COMPRESS 
//	CLIENT_FOUND_ROWS
//	CLIENT_IGNORE_SPACE
//	CLIENT_INTERACTIVE
//	CLIENT_LOCAL_FILES
//	CLIENT_MULTI_STATEMENTS
//	CLIENT_MULTI_RESULTS  
//	CLIENT_NO_SCHEMA db_name.tbl_name.col_name ODBC
//	CLIENT_ODBC  
//	CLIENT_SSL*/ //openSSL
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect success !");
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//	
//	int ret = mysql_ping(&mysql);
//	if (ret == 0)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping success!");
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}
//
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	int time = 4;
//	//MYSQL_OPT_RECONNECT 重新链接
//	//MYSQL_OPT_CONNECT_TIMEOUT 超时的时间 “ ”
//	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT,&time);
//
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect success !");
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	 ret = mysql_ping(&mysql);
//	if (ret == 0)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping success!");
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//
//	mysql_close(&mysql);
//	mysql_library_end();
//
//}

//Mysql查询
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	my_bool reconnect = 1;
//	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);
//	int BufSize = 1024 * 1024 * 64 ;//1MB
//	//MYSQL_OPT_MAX_ALLOWED_PACKET 64MB
//	ret = mysql_options(&mysql, MYSQL_OPT_MAX_ALLOWED_PACKET, &BufSize);
//
//	char select[] = "SELECT name FROM W";
// 	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		//mysql_real_query(&mysql, select, strlen(select));
//		if ((ret = mysql_query(&mysql, select)) == 0)
//		{
//			if (MYSQL_RES *result = mysql_store_result(&mysql))
//			{
//				while (MYSQL_ROW sql_row = mysql_fetch_row(result))
//				{
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name [%s]"), ANSI_TO_TCHAR(sql_row[0])));
//				}
//			}
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	ret = mysql_ping(&mysql);
//	if (ret == 0)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping success!");
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}

//对查询结果进行服务器读取或者本地缓存读取
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	my_bool reconnect = 1;
//	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);
//	int BufSize = 1024 * 1024 * 64;//1MB
//	//MYSQL_OPT_MAX_ALLOWED_PACKET 64MB
//	ret = mysql_options(&mysql, MYSQL_OPT_MAX_ALLOWED_PACKET, &BufSize);
//
//	char select[] = "SELECT name FROM W";
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		//mysql_real_query(&mysql, select, strlen(select));
//		if ((ret = mysql_query(&mysql, select)) == 0)
//		{
//			//mysql_use_result(); mysql_store_result
//			if (MYSQL_RES *result = mysql_use_result(&mysql))
//			{
//				while (MYSQL_ROW sql_row = mysql_fetch_row(result))
//				{
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name [%s]"), ANSI_TO_TCHAR(sql_row[0])));
//				}
//			}
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	ret = mysql_ping(&mysql);
//	if (ret == 0)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping success!");
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}
//
//
//void USimpleMySQLLibrary::MySqlMain()
//{
//	//Text
//	/*
//		char  255  char 固定大小
//		varchar 255 -> text 可变长度
//		text  65,535     255平方 可变长度
//		tinytext 255  可变长度
//		blob     65,535
//		mediumtext 16,777,215 个字符的字符串。64四次方
//		mediumblob 16,777,215
//		longtext	4,294,967,295 个字符
//		longblob	4,294,967,295 个字符
//		ENUM(x,y,z,etc.)  65535 "" = 0
//		SET()
//	*/
//	//Number
//	/*
//		int(size)  -2147483648到2147483647  0到4294967295 size= 11
//		bigint(size) -9223372036854775808 到 9223372036854775807 0到18446744073709551615 size= 20
//		tinyint(size) -128到127 0到255 size = 4
//		smallint(size) -32768到32767 无符号0到65535 size = 6
//		mediumint(size) -8388608到8388607 无符号的范围是0到16777215  size = 9
//		float(size,d)
//		double(size,d)
//		decimal(size,d)作为字符串存储的 double 类型
//	*/
//	//Date/time
//	/*
//		DATE() YYYY-MM-DD 1000-01-01 ~ 9999-12-31  2020-6-27
//		DATETIME() YYYY-MM-DD HH:MM:SS  1000-01-01 00:00:00~ 9999-12-31 23:59:59 2020-6-27 09:30:01
//		TIMESTAMP() 1970-01-01 00:00:01 - 至今 用秒来存储
//		TIME() HH:MM:SS -838:59:59 ~ 838:59:59
//		YEAR() 
//	*/
//
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	char select[] = "SELECT name FROM W";
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		//1.减少服务器负荷
//		//2.提高服务器响应的速度
//		//3.可以提供参数机制，让客户有更多查询方法
//
//		//MYSQL_STMT
//		//MYSQL_BIND
//
//		//MYSQL_STMT *mysql_stmt_init(MYSQL *mysql)     
//		//mysql_stmt_close(MYSQL_STMT *)
//
//		//int mysql_stmt_prepare(MYSQL_STMT *stmt, const char *query, unsigned long length)
//		//my_bool mysql_stmt_bind_param(MYSQL_STMT *stmt, MYSQL_BIND *bind) 
//		//my_bool mysql_stmt_bind_result(MYSQL_STMT *stmt, MYSQL_BIND *bind) 
//
//		//int mysql_stmt_execute(MYSQL_STMT *stmt)
//		//int mysql_stmt_store_result(MYSQL_STMT *stmt)  
//		//int mysql_stmt_fetch(MYSQL_STMT *stmt) 
//
//		MYSQL_STMT *Ptr = mysql_stmt_init(&mysql);
//
//		char *SQL = "INSERT INTO ppp1 VALUES(?,?)";//放置注入攻击
//		int ret = mysql_stmt_prepare(Ptr, SQL, FString(ANSI_TO_TCHAR(SQL)).Len());
//		if (ret != 0)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_stmt_error(Ptr));
//			return;
//		}
//		int32 id = 100;
//		MYSQL_BIND Pram[2];
//		FMemory::Memset(Pram, 0, sizeof(Pram));
//		Pram[0].buffer_type = MYSQL_TYPE_LONG;
//		Pram[0].buffer = &id;
//		Pram[0].length = 0;
//
//		char *content = "Hello";
//		uint32 contentlen = strlen(content);
//		Pram[1].buffer_type = MYSQL_TYPE_VARCHAR;
//		//Pram[1].is_null = 0;
//		Pram[1].buffer = content;
//		Pram[1].length = (unsigned long*)&contentlen;
//		Pram[1].buffer_length = contentlen;
//		
//		ret = mysql_stmt_bind_param(Ptr, Pram);
//		if (ret != 0)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_stmt_error(Ptr));
//			return;
//		}
//
//		ret = mysql_stmt_execute(Ptr);
//		if (ret != 0)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_stmt_error(Ptr));
//			return;
//		}
//
//		mysql_stmt_close(Ptr);
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}
//
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	my_bool reconnect = 1;
//	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);
//	int BufSize = 1024 * 1024 * 64;//1MB
//	//MYSQL_OPT_MAX_ALLOWED_PACKET 64MB
//	ret = mysql_options(&mysql, MYSQL_OPT_MAX_ALLOWED_PACKET, &BufSize);
//
//	char select[] = "SELECT * FROM W";
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		//mysql_real_query(&mysql, select, strlen(select));
//		if ((ret = mysql_query(&mysql, select)) == 0)
//		{
//			//mysql_use_result(); mysql_store_result
//			if (MYSQL_RES *result = mysql_use_result(&mysql))
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("mysql_num_fields = [%i]"), mysql_num_rows(result)));
//
//				while (MYSQL_ROW sql_row = mysql_fetch_row(result))
//				{
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name [%s]"), ANSI_TO_TCHAR(sql_row[0])));
//				}
//			}
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	ret = mysql_ping(&mysql);
//	if (ret == 0)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping success!");
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}
//

//// mysql_errno和mysql_field_count
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	my_bool reconnect = 1;
//	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);
//	int BufSize = 1024 * 1024 * 64;//1MB
//	//MYSQL_OPT_MAX_ALLOWED_PACKET 64MB
//	ret = mysql_options(&mysql, MYSQL_OPT_MAX_ALLOWED_PACKET, &BufSize);
//
//	char select[] = "SELECT name FROM W";
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		//mysql_real_query(&mysql, select, strlen(select));
//		if ((ret = mysql_query(&mysql, select)) == 0)
//		{		
//			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("mysql_field_count = [%i]"), mysql_field_count(&mysql)));
//			mysql_errno(&mysql);
//			{
//				//  1005 : 创建表失败
//				//	1006 : 创建数据库失败
//				//	1007 : 数据库已存在，创建数据库失败
//				//	1008 : 数据库不存在，删除数据库失败
//				//	1009 : 不能删除数据库文件导致删除数据库失败
//				//	1010 : 不能删除数据目录导致删除数据库失败
//				//	1011 : 删除数据库文件失败
//				//	1012 : 不能读取系统表中的记录
//				//	1016 : 无法打开文件
//				//	1020 : 记录已被其他用户修改
//				//	1021 : 硬盘剩余空间不足，请加大硬盘可用空间
//				//	1022 : 关键字重复，更改记录失败
//				//	1023 : 关闭时发生错误
//				//	1024 : 读文件错误
//				//	1025 : 更改名字时发生错误
//				//	1026 : 写文件错误
//				//	1032 : 记录不存在
//				//	1036 : 数据表是只读的，不能对它进行修改
//				//	1037 : 系统内存不足，请重启数据库或重启服务器
//				//	1038 : 用于排序的内存不足，请增大排序缓冲区
//				//	1040 : 已到达数据库的最大连接数，请加大数据库可用连接数
//				//	1041 : 系统内存不足
//				//	1042 : 无效的主机名
//				//	1043 : 无效连接
//				//	1044 : 当前用户没有访问数据库的权限
//				//	1045 : 不能连接数据库，用户名或密码错误
//				//	1040 : 最大连接数
//				//	1048 : 字段不能为空
//				//	1049 : 数据库不存在
//				//	1050 : 数据表已存在
//				//	1051 : 数据表不存在
//				//	1054 : 字段不存在
//				//	1065 : 无效的SQL语句，SQL语句为空
//				//	1081 : 不能建立Socket连接
//				//	1114 : 数据表已满，不能容纳任何记录
//				//	1116 : 打开的数据表太多
//				//	1129 : 数据库出现异常，请重启数据库
//				//	1130 : 连接数据库失败，没有连接数据库的权限
//				//	1133 : 数据库用户不存在
//				//	1141 : 当前用户无权访问数据库
//				//	1142 : 当前用户无权访问数据表
//				//	1143 : 当前用户无权访问数据表中的字段
//				//	1146 : 数据表不存在
//				//	1147 : 未定义用户对数据表的访问权限
//				//	1149 : SQL语句语法错误
//				//	1158 : 网络错误，出现读错误，请检查网络连接状况
//				//	1159 : 网络错误，读超时，请检查网络连接状况
//				//	1160 : 网络错误，出现写错误，请检查网络连接状况
//				//	1161 : 网络错误，写超时，请检查网络连接状况
//				//	1062 : 字段值重复，入库失败
//			}
//
//			//mysql_use_result(); mysql_store_result
//			if (MYSQL_RES *result = mysql_use_result(&mysql))
//			{
//				while (MYSQL_ROW sql_row = mysql_fetch_row(result))
//				{
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name [%s]"), ANSI_TO_TCHAR(sql_row[0])));
//				}
//			}
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	ret = mysql_ping(&mysql);
//	if (ret == 0)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping success!");
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "mysql_ping failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}
//
//
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	my_bool reconnect = 1;
//	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);
//	int BufSize = 1024 * 1024 * 64;//1MB
//	//MYSQL_OPT_MAX_ALLOWED_PACKET 64MB
//	ret = mysql_options(&mysql, MYSQL_OPT_MAX_ALLOWED_PACKET, &BufSize);
//
//	char select[] = "SELECT name FROM W";
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		if ((ret = mysql_query(&mysql, select)) == 0)
//		{
//			if (MYSQL_RES *result = mysql_use_result(&mysql))
//			{
//				while (MYSQL_ROW sql_row = mysql_fetch_row(result))
//				{
//					unsigned long *len = mysql_fetch_lengths(result);// \0 "renzhai" strlen()
//
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name len [%i]"), *len));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name [%s]"), ANSI_TO_TCHAR(sql_row[0])));
//				}
//			}
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}
//
//
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	my_bool reconnect = 1;
//	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);
//	int BufSize = 1024 * 1024 * 64;//1MB
//	//MYSQL_OPT_MAX_ALLOWED_PACKET 64MB
//	ret = mysql_options(&mysql, MYSQL_OPT_MAX_ALLOWED_PACKET, &BufSize);
//
//	char select[] = "SELECT name FROM W";
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		if ((ret = mysql_query(&mysql, select)) == 0)
//		{
//			if (MYSQL_RES *result = mysql_use_result(&mysql))
//			{
//				int num_fields = mysql_num_fields(result);
//				while (MYSQL_ROW sql_row = mysql_fetch_row(result))
//				{
//					for (int i = 0 ;i < num_fields; i++)
//					{
//						MYSQL_FIELD *find_sql = mysql_fetch_field_direct(result, i);
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name [%s]"), find_sql->name));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("org_name [%s]"), find_sql->org_name));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("table [%s]"), find_sql->table));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("org_table [%s]"), find_sql->org_table));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("db [%s]"), find_sql->db));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("catalog [%s]"), find_sql->catalog));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("def [%s]]"), find_sql->def));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("length [%i]"), find_sql->length));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("max_length [%i]"), find_sql->max_length));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name_length [%i]"), find_sql->name_length));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("org_name_length [%i]"), find_sql->org_name_length));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("db_length [%i]"), find_sql->db_length));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("catalog_length [%i]"), find_sql->catalog_length));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("flags [%i]"), find_sql->flags));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("def_length [%i]"), find_sql->def_length));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("decimals [%i]"), find_sql->decimals));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("charsetnr [%i]"), find_sql->charsetnr));
//						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("enum_field_types [%i]"), find_sql->type));
//						//enum enum_field_types type; /* Type of field. See mysql_com.h for types */
//						//void *extension;
//					}
//					
//					unsigned long *len = mysql_fetch_lengths(result);// \0 "renzhai" strlen()
//
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name len [%i]"), *len));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name [%s]"), ANSI_TO_TCHAR(sql_row[0])));
//				}
//			}
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}

//
//void USimpleMySQLLibrary::MySqlMain()
//{
//	const char *user = "root";
//	const char *host = "127.0.0.1";//localhost
//	const char *pawd = "root";
//	const char *table = "hello";
//	const uint32 port = 3306;
//
//	MYSQL mysql;
//	mysql_library_init(NULL, NULL, NULL);//保证线程安全
//	mysql_init(&mysql);//分配内存
//
//	my_bool reconnect = 1;
//	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);
//	int BufSize = 1024 * 1024 * 64;//1MB
//	//MYSQL_OPT_MAX_ALLOWED_PACKET 64MB
//	ret = mysql_options(&mysql, MYSQL_OPT_MAX_ALLOWED_PACKET, &BufSize);
//
//	char select[] = "SELECT * FROM W";
//	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, 0))//链接
//	{
//		if ((ret = mysql_query(&mysql, select)) == 0)
//		{
//			if (MYSQL_RES *result = mysql_use_result(&mysql))
//			{
//				int num_fields = mysql_num_fields(result);
//				MYSQL_FIELD *find_sql_org = mysql_fetch_field(result);
//				for (int i = 0; i < num_fields; i++)
//				{
//					MYSQL_FIELD *find_sql = &find_sql_org[i];
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name [%s]"), find_sql->name));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("org_name [%s]"), find_sql->org_name));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("table [%s]"), find_sql->table));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("org_table [%s]"), find_sql->org_table));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("db [%s]"), find_sql->db));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("catalog [%s]"), find_sql->catalog));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("def [%s]]"), find_sql->def));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("length [%i]"), find_sql->length));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("max_length [%i]"), find_sql->max_length));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("name_length [%i]"), find_sql->name_length));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("org_name_length [%i]"), find_sql->org_name_length));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("db_length [%i]"), find_sql->db_length));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("catalog_length [%i]"), find_sql->catalog_length));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("flags [%i]"), find_sql->flags));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("def_length [%i]"), find_sql->def_length));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("decimals [%i]"), find_sql->decimals));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("charsetnr [%i]"), find_sql->charsetnr));
//					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, *FString::Printf(TEXT("enum_field_types [%i]"), find_sql->type));
//					//enum enum_field_types type; /* Type of field. See mysql_com.h for types */
//					//void *extension;
//				}
//			}		
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, mysql_error(&mysql));
//	}
//
//	mysql_close(&mysql);
//	mysql_library_end();
//}
//

void USimpleMySQLLibrary::MySqlMain()
{
	return;

	const char *user = "root";
	const char *host = "127.0.0.1";//localhost
	const char *pawd = "root";
	const char *table = "hello";
	const uint32 port = 3306;

	MYSQL mysql;
	mysql_library_init(NULL, NULL, NULL);//保证线程安全
	mysql_init(&mysql);//分配内存

	my_bool reconnect = 1;
	int ret = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);
	int BufSize = 1024 * 1024 * 64;//1MB
	//MYSQL_OPT_MAX_ALLOWED_PACKET 64MB
	ret = mysql_options(&mysql, MYSQL_OPT_MAX_ALLOWED_PACKET, &BufSize);
	//CREATE DATABASE 数据库名 //创建数据库
	//CREATE DATABASE IF NOT EXISTS renzhai DEFAULT CHARSET utf8 COLLATE utf8_general_ci;如果该数据存在就不创建了
	//DROP DATABASE 数据库名 //删除数据库
	//CREATE TABLE 表名 (列名 列类型);
	//DROP TABLE 表名 将表全部删除 释放内存 不支持回滚 Innodb 和 MyISAM 全部清除
	//TRUNCATE TABLE 表名 清除表 释放内存 不支持回滚 Innodb 和 MyISAM 全部清除
	//delete from 表名 where 删除条件 Innodb 和 MyISAM
	//optimise table 表名 真正释放内存
	//use hello;delete from hp where name=\"xiaohei\";optimise table hp;
	//insert into 表名(字段1，字段2，字段3，字段4) values (1,2,3,4); 横向插入 
	//insert into 表名 values(id,"name",'2020-09-06');
	//SELECT * FROM W LIMIT 2 ,4;  3-7
	//LIMIT 2 ,4 ;3-7
	//LIMIT 2;0,2
	//order by 字段名			从小到大	 排序
	//order by 字段名 desc		从大到小  排序
	//where 字段 = xxx 条件				条件查询			
	//where 字段 between "1" and "123";	范围查询
	//where 字段 like "";				模糊查询
	//where 字段 in ("","");				通过固定条件查询
	//where 判断 and 判断;				查询符合条件的数据
	//where 判断 or 判断;	
	//left join 表  on  判断				左关联
	//left join 表  on  判断				右关联
	//Where 判断 
	//Where A = B
	//Where A != B
	//Where A > B
	//Where A >= B
	//Where A <= B
	//Where 判断 and 判断 
	//Where 判断 or 判断 
	//group by 
	//update 表名 set 字段="asdasdqwe",字段="asdawd".... where 
	//update 表名 set 字段=replace(字段,'老名字','新名字') 判断条件
	//select * from w where name like 模糊的字段 模糊查询
	//XXX% 前面符合XXX全部查询出来
	//%XXX 后缀是XXX全部查询出来
	//XX%bb 中间无论是什么 只要左右是XX bb全部查询出来
	//_a //___zhai %zhai
	//_a_
	//a_
	//[]
	//[^]
	//group by
	//count()

	//inner join     on 
	//right join 表  on  判断	
	//left join 表  on  判断	

	// IS NULL
	// IS NOT NULL

	//事务满足4个条件
	//原子操作
	//一致
	//隔离
	//持久
	//ENGINE=InnoDB
	//START TRANSACTION 开启一个事务
	//set autocommit = 0 //将自动提交设置为手动
	//SQL语句 插入数据
	//COMMIT 提交
	//ROLLBACK 回滚
	//set autocommit = 1 设置回来
	//savepoint 存储点名字
	//release savepoint 存储点名字
	//rollback to 存储点名字
	//锁
	//InnoDB 行级别的锁  支持
	//MyISAM 表级别锁 不支持回滚
	//alter 
	//alter table 表名 engine=myisam;innodb
	//ALTER TABLE 表名 DROP 字段;
	//ALTER TABLE 表名 ADD 字段 类型;
	//ALTER TABLE 表名 modify 字段 类型(char(100));
	//ALTER TABLE 表名 CHANGE 字段1 字段2 类型;
	//ALTER TABLE 表名 MODIFY 字段 BIGINT NOT NULL DEFAULT value;
	//ALTER TABLE 表名 RENAME TO 新表名;
	//ALTER TABLE 表名 ALTER 字段 DROP DEFAULT;
	//ALTER TABLE 表名 ADD INDEX 索引名称(字段(长度));

	//create index 索引名称 on 表(字段(长度));
	//show index from 表;


	// create temporary table 表名 (字段 类型，...) ;
	// create temporary table 表名 (字段 类型，...) SELECT 字段, 字段 FROM 表名 where 条件;
	// DROP TEMPORARY TABLE IF EXISTS 表;
	// insert into 临时表名 (字段 ,字段，... or *) SELECT 字段 ,字段，... or * FROM 要插入的表;
	// insert into 表 (字段 ,字段，... or *) SELECT 字段 ,字段，... or * FROM 表;   复制表
	//memory存储引擎 
	//innodb存储引擎 myisam存储引擎 

	//DROP TEMPORARY TABLE IF EXISTS hello_c; 
	//CREATE TABLE 表名 LIKE 表名1; //创建相同表
	//INSERT INTO 表名 SELECT * FROM 表名1;
	//CREATE TABLE 表名 AS (SELECT 字段, 字段 FROM 表名1);
	//CREATE TABLE 表名 AS (SELECT 字段, 字段 as 重新起名 FROM 表名1);
	//CREATE TABLE 表名(字段 INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY) AS (SELECT * FROM 表名);
	//SELECT VERSION();
	//SELECT DATABASE();
	//SELECT USER();
	//SHOW STATUS;
	//primary key (字段1, 字段2...) //字段不能为NULL
	//UNIQUE  (字段1, 字段2...)	//保证唯一性 
	//insert ignore into 表名 (字段1, 字段2... ) values( '值1', '值2' ...); //存在就跳过
	//replace into 表名 (字段1, 字段2... ) values( '值1', '值2' ...); //存在 就替换
	//SELECT COUNT(*) as 别名, 字段1, 字段2 FROM 表名  GROUP BY 字段1, 字段2 HAVING 条件; 统计重复数据
	//SELECT DISTINCT 字段1, 字段2 FROM 表名; 过滤重复数据1
	//SELECT 字段1 FROM 表名 GROUP BY(字段1);   过滤重复数据2

	char select[] = "SHOW STATUS ;";//
	if (mysql_real_connect(&mysql, host, user, pawd, table, port, 0, CLIENT_MULTI_STATEMENTS))//链接
	{
		//char *sql = "begin;";//begin START TRANSACTION
		//if ((ret = mysql_query(&mysql, sql)) != 0)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
		//}
		if ((ret = mysql_query(&mysql, select)) == 0)
		{
			if (MYSQL_RES *result = mysql_store_result(&mysql))
			{
				int32 num_row = mysql_num_fields(result);
				while (MYSQL_ROW sql_row = mysql_fetch_row(result))
				{
					FString RowString;
					for (int i = 0; i < num_row; i++)
					{
						if (sql_row[i] != NULL)
						{
							int32 Len = FString(ANSI_TO_TCHAR(sql_row[i])).Len() / 2;
							int32 Add_Len = 10 - Len;//剩余空间的 长度
							RowString += "|";
							for (int j = 0; j < Add_Len; j++)
							{
								RowString += " ";
							}

							RowString += ANSI_TO_TCHAR(sql_row[i]);

							for (int j = 0; j < Add_Len; j++)
							{
								RowString += " ";
							}
						}
						else
						{
							int32 Len = FString("NULL").Len() / 2;
							int32 Add_Len = 10 - Len;//剩余空间的 长度
							RowString += "|";
							for (int j = 0; j < Add_Len; j++)
							{
								RowString += " ";
							}

							RowString += "NULL";

							for (int j = 0; j < Add_Len; j++)
							{
								RowString += " ";
							}
						}
					}

					RowString += "|";
#ifdef  PLATFORM_PROJECT
					GEngine->AddOnScreenDebugMessage(-1, 1000, FColor::Red, *RowString);
#endif
				}
			}
		}

		//char *sql = "insert into cpy_table (id,date_table,number) SELECT id,table_date,number FROM hp;";
		//if ((ret = mysql_query(&mysql, sql)) != 0)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
		//}

		//sql = "SHOW CREATE TABLE cpy_table;";//ROLLBACK WORK
		//if ((ret = mysql_query(&mysql, sql)) == 0)
		//{
		//	if (MYSQL_RES *result = mysql_store_result(&mysql))
		//	{
		//		int32 num_row = mysql_num_fields(result);
		//		while (MYSQL_ROW sql_row = mysql_fetch_row(result))
		//		{
		//			FString RowString;
		//			for (int i = 0; i < num_row; i++)
		//			{
		//				if (sql_row[i] != NULL)
		//				{
		//					int32 Len = FString(ANSI_TO_TCHAR(sql_row[i])).Len() / 2;
		//					int32 Add_Len = 10 - Len;//剩余空间的 长度
		//					RowString += "|";
		//					for (int j = 0; j < Add_Len; j++)
		//					{
		//						RowString += " ";
		//					}

		//					RowString += ANSI_TO_TCHAR(sql_row[i]);

		//					for (int j = 0; j < Add_Len; j++)
		//					{
		//						RowString += " ";
		//					}
		//				}
		//				else
		//				{
		//					int32 Len = FString("NULL").Len() / 2;
		//					int32 Add_Len = 10 - Len;//剩余空间的 长度
		//					RowString += "|";
		//					for (int j = 0; j < Add_Len; j++)
		//					{
		//						RowString += " ";
		//					}

		//					RowString += "NULL";

		//					for (int j = 0; j < Add_Len; j++)
		//					{
		//						RowString += " ";
		//					}
		//				}
		//			}

		//			RowString += "|";

		//			GEngine->AddOnScreenDebugMessage(-1, 1000, FColor::Red, *RowString);
		//		}
		//	}
		//}
		//sql = "savepoint hello_v1;";//ROLLBACK WORK
		//if ((ret = mysql_query(&mysql, sql)) != 0)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
		//}

		//sql = "DROP TABLE hello_c;";
		//if ((ret = mysql_query(&mysql, sql)) != 0)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
		//}

		//sql = "insert into hp (name,table_date) values('hello333','2064-07-14');";
		//if ((ret = mysql_query(&mysql, sql)) == 0)
		//{
		//	if (MYSQL_RES *result = mysql_store_result(&mysql))
		//	{
		//		int32 num_row = mysql_num_fields(result);
		//		while (MYSQL_ROW sql_row = mysql_fetch_row(result))
		//		{
		//			FString RowString;
		//			for (int i = 0; i < num_row; i++)
		//			{
		//				if (sql_row[i] != NULL)
		//				{
		//					int32 Len = FString(ANSI_TO_TCHAR(sql_row[i])).Len() / 2;
		//					int32 Add_Len = 10 - Len;//剩余空间的 长度
		//					RowString += "|";
		//					for (int j = 0; j < Add_Len; j++)
		//					{
		//						RowString += " ";
		//					}

		//					RowString += ANSI_TO_TCHAR(sql_row[i]);

		//					for (int j = 0; j < Add_Len; j++)
		//					{
		//						RowString += " ";
		//					}
		//				}
		//				else
		//				{
		//					int32 Len = FString("NULL").Len() / 2;
		//					int32 Add_Len = 10 - Len;//剩余空间的 长度
		//					RowString += "|";
		//					for (int j = 0; j < Add_Len; j++)
		//					{
		//						RowString += " ";
		//					}

		//					RowString += "NULL";

		//					for (int j = 0; j < Add_Len; j++)
		//					{
		//						RowString += " ";
		//					}
		//				}
		//			}

		//			RowString += "|";

		//			GEngine->AddOnScreenDebugMessage(-1, 1000, FColor::Red, *RowString);
		//		}
		//	}
		//}

		//sql = "rollback to savepoint hello_v1;";//
		//if ((ret = mysql_query(&mysql, sql)) != 0)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
		//}

		//sql = "RELEASE SAVEPOINT hello_v1;";//
		//if ((ret = mysql_query(&mysql, sql)) != 0)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
		//}

		//sql = "COMMIT;";
		//if ((ret = mysql_query(&mysql, sql)) != 0)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
		//}

		//sql = "set autocommit = 1;";
		//if ((ret = mysql_query(&mysql, sql)) != 0)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
		//}
#ifdef PLATFORM_PROJECT
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
#endif
	}
	else
	{
#ifdef PLATFORM_PROJECT
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "connect failed!");
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, ANSI_TO_TCHAR(mysql_error(&mysql)));
#endif	
	}

	mysql_close(&mysql);
	mysql_library_end();

	/*
	SYSTEM_USER()
	SESSION_USER()
	NULLIF(x,y) x=y return null; x
	LAST_INSERT_ID() 返回最近生成的 AUTO_INCREMENT 值
	ISNULL (expression) 判断表达式是否为 NULL
	IF(expr,v1,v2) 如果表达式 expr 成立，返回结果 v1；否则，返回结果 v2。
	CURRENT_USER()返回当前用户
	CONVERT(s USING cs) 函数将字符串 s 的字符集变成 cs
	CHARSET() 字符集
	CONV(x,f1,f2) 返回 f1 进制数变成 f2 进制数
	CONNECTION_ID() 返回服务器的连接数
	COALESCE(expr1, expr2, ...., expr_n) 返回参数中的第一个非空表达式（从左向右）
	CAST(x AS type) 转换数据类型
	BINARY(s) 将字符串 s 转换为二进制字符串
	BIN(x) 返回 x 的二进制编码


	YEARWEEK(date, mode) 0-53  366/7  返回年份及第几周（0到53），mode 中 0 表示周天，1表示周一，以此类推
	YEAR(d) 返回年份
	WEEKOFYEAR(d) 计算日期 d 是本年的第几个星期，范围是 0 到 53
	WEEKDAY(d) 日期 d 是星期几，0 表示星期一，1 表示星期二
	WEEK(d) 计算日期 d 是本年的第几个星期，范围是 0 到 53
	TO_DAYS(d) 计算日期 d 距离 0000 年 1 月 1 日的天数
	TIMESTAMP(expression, interval) 单个参数时，函数返回日期或日期时间表达式；有2个参数时，将参数加和
	TIMEDIFF(time1, time2) 计算时间差值
	TIME_TO_SEC(t) 将时间 t 转换为秒
	TIME_FORMAT(t,f) 按表达式 f 的要求显示时间 t
	TIME(expression) 提取传入表达式的时间部分
	SYSDATE()	返回当前日期和时间
	SUBTIME(t,n) 时间 t 减去 n 秒的时间
	STR_TO_DATE(string, format_mask) 将字符串转变为日期
	SEC_TO_TIME(s) 将以秒为单位的时间 s 转换为时分秒的格式
	SECOND(t) 返回 t 中的秒钟值
	QUARTER(d) 返回日期d是第几季节，返回 1 到 4
	PERIOD_DIFF(period1, period2) 返回两个时段之间的月份差值
	PERIOD_ADD(period, number)为 年-月 组合日期添加一个时段
	NOW() 返回当前日期和时间
	MONTH(d) 返回日期d中的月份值，1 到 12
	MONTHNAME(d)返回日期当中的月份名称，如 November
	MINUTE(t) 返回 t 中的分钟值
	MICROSECOND(date) 返回日期参数所对应的微秒数
	MAKETIME(hour, minute, second) 组合时间，参数分别为小时、分钟、秒
	MAKEDATE(year, day-of-year) 基于给定参数年份 year 和所在年中的天数序号 day-of-year 返回一个日期
	LOCALTIMESTAMP() 返回当前日期和时间
	LOCALTIME() 返回当前日期和时间
	LAST_DAY(d) 返回给给定日期的那一月份的最后一天
	HOUR(t) 返回 t 中的小时值
	FROM_DAYS(n) 计算从 0000 年 1 月 1 日开始 n 天后的日期
	EXTRACT(type FROM d) 从日期 d 中获取指定的值，type 指定返回的值。
	DAYOFYEAR(d) 计算日期 d 是本年的第几天
	DAYOFWEEK(d) 日期 d 今天是星期几，1 星期日，2 星期一，以此类推
	DAYOFMONTH(d) 计算日期 d 是本月的第几天
	DAYNAME(d) 返回日期 d 是星期几，如 Monday,Tuesday
	DAY(d) 返回日期值 d 的日期部分
	DATE_SUB(date,INTERVAL expr type) 函数从日期减去指定的时间间隔。
	DATE_FORMAT(d,f) 按表达式 f的要求显示日期 d
	ADDDATE(d，INTERVAL expr type) 计算起始日期 d 加上一个时间段后的日期
	DATEDIFF(d1,d2) 计算日期 d1->d2 之间相隔的天数
	DATE() 从日期或日期时间表达式中提取日期值
	CURTIME() 返回当前时间
	CURRENT_TIMESTAMP()返回当前日期和时间
	CURRENT_TIME() 返回当前时间
	CURRENT_DATE() 返回当前日期
	CURDATE() 返回当前日期
	ADDTIME(t,n)时间 t 加上 n 秒的时间

	ABS(x) 返回 x 的绝对值
	ACOS(x) 求 x 的反余弦值(参数是弧度)
	ASIN(x) 求反正弦值(参数是弧度)
	ATAN(x) 求反正切值(参数是弧度)
	ATAN2(n, m) 求反正切值(参数是弧度)
	AVG(expression) 返回一个表达式的平均值，expression 是一个字段
	SELECT AVG(Price) FROM Products

	CEIL(x) 返回大于或等于 x 的最小整数　
	CEILING(x) 返回大于或等于 x 的最小整数　
	COS(x) 求余弦值(参数是弧度)
	COT(x) 求余切值(参数是弧度)
	COUNT(expression) 返回查询的记录总数，expression 参数是一个字段或者 * 号
	DEGREES(x)将弧度转换为角度　　
	n DIV m
	EXP(x) 返回 e 的 x 次方　　 e=2.718281828
	FLOOR(x) 返回小于或等于 x 的最大整数　　
	GREATEST(expr1, expr2, expr3, ...) 返回列表中的最大值
	LEAST(expr1, expr2, expr3, ...) 返回列表中的最小值
	LN(n) 返回数字的自然对数，以 e 为底。
	LOG(x) 或 LOG(base, x) 返回自然对数(以 e 为底的对数)，如果带有 base 参数，则 base 为指定带底数。　
	LOG10(x) 返回以 10 为底的对数　　
	LOG2(x) 返回以 2 为底的对数
	MAX(expression)返回字段 expression 中的最大值
	SELECT MAX(Price) FROM hp;
	MIN(expression) 返回字段 expression 中的最小值
	SELECT MIN(Price) FROM hp;
	MOD(x,y) 返回 x 除以 y 以后的余数　
	PI() 返回圆周率(3.141593）　　
	POW(x,y) 返回 x 的 y 次方　
	POWER(x,y) 返回 x 的 y 次方
	RADIANS(x) 将角度转换为弧度　　
	RAND() 返回 0 到 1 的随机数　　
	ROUND(x) 返回离 x 最近的整数
	SIGN(x) 返回 x 的符号，x 是负数、0、正数分别返回 -1、0 和 1
	SQRT(x) 返回 x 的平方根　　
	SUM(expression) 返回指定字段的总和
	SELECT SUM(id) AS daslfas FROM hp;
	TAN(x) 求正切值(参数是弧度)
	TRUNCATE(x,y) 返回数值 x 保留到小数点后 y 位的值（与 ROUND 最大的区别是不会进行四舍五入）

	UPPER(s) 将字符串转换为大写
	UCASE(s 将字符串转换为大写
	TRIM(s) 去掉字符串 s 开始和结尾处的空格
	SUBSTRING_INDEX(s, delimiter, number)
		返回从字符串 s 的第 number 个出现的分隔符 delimiter 之后的子串。
		如果 number 是正数，返回第 number 个字符左边的字符串。
		如果 number 是负数，返回第(number 的绝对值(从右边数))个字符右边的字符串。
	SUBSTRING(s, start, length) 从字符串 s 的 start 位置截取长度为 length 的子字符串
	SUBSTR(s, start, length)
	STRCMP(s1,s2) 比较字符串 s1 和 s2，如果 s1 与 s2 相等返回 0 ，如果 s1>s2 返回 1，如果 s1<s2 返回 -1
	SPACE(n) 返回 n 个空格
	RTRIM(s)去掉字符串 s 结尾处的空格
	RPAD(s1,len,s2) 在字符串 s1 的结尾处添加字符串 s2，使字符串的长度达到 len
	RIGHT(s,n) 返回字符串 s 的后 n 个字符
	LEFT(s,n) 返回字符串 s 的前 n 个字符
	REVERSE(s) 将字符串s的顺序反过来
	REPLACE(s,s1,s2) 将字符串 s2 替代字符串 s 中的字符串 s1
	REPEAT(s,n) 将字符串 s 重复 n 次
	POSITION(s1 IN s) 从字符串 s 中获取 s1 的开始位置
	MID(s,n,len) 从字符串 s 的 n 位置截取长度为 len 的子字符串，同 SUBSTRING(s,n,len)
	LTRIM(s)去掉字符串 s 开始处的空格
	LOWER(s) 将字符串 s 的所有字母变成小写字母
	LCASE(s) 将字符串 s 的所有字母变成小写字母
	LOCATE(s1,s) 从字符串 s 中获取 s1 的开始位置
	INSERT(s1,x,len,s2) 字符串 s2 替换 s1 的 x 位置开始长度为 len 的字符串
	FORMAT(x,n) 函数可以将数字 x 进行格式化 "#,###.##", 将 x 保留到小数点后 n 位，最后一位四舍五入。
	FIND_IN_SET(s1,s2) 返回在字符串s2中与s1匹配的字符串的位置
	FIELD(s,s1,s2...)返回第一个字符串 s 在字符串列表(s1,s2...)中的位置
	CONCAT_WS(x, s1,s2...sn) 同 CONCAT(s1,s2,...) 函数，但是每个字符串之间要加上 x，x 可以是分隔符
	CONCAT(s1,s2...sn) 字符串 s1,s2 等多个字符串合并为一个字符串
	CHARACTER_LENGTH(s) 返回字符串 s 的字符数
	CHAR_LENGTH(s) 返回字符串 s 的字符数
	ASCII(s) 返回字符串 s 的第一个字符的 ASCII 码。

	create function 函数名([参数列表]) returns 数据类型
	begin
	 sql语句;
	 return 值;
	end;
	例子 create function holleworld() returns int return 123;
	select helleworld();

	create function holleworld1() returns char(100)
	begin
		return CONCAT('qwe','eee','ddfa');
	end;
	select holleworld1();

	create function holleworld2(name varchar(15)) returns char(100)
	begin
		return CONCAT(name,':','hello');
	end;
	select holleworld2();

	IN		字段直接传进去
	OUT		取内容
	INOUT	传入和取值
	SET @Q = 1;
	create PROCEDURE holleworld4(IN name varchar(15))
	begin
		SET name = 'Hello';
	end;
	CALL holleworld4(@Q);
	select @Q;

	SET @Q1 = 0;
	SET @Q2 = 1;
	create PROCEDURE holleworld7(IN name varchar(15),INOUT name1 varchar(15))
	begin
		SET name = 'Hello';
		SET name1 = 'name1';
	end;
	CALL holleworld7(@Q1,Q2);
	select @Q1,@Q2;
	*/


}

USimpleMysqlObject * USimpleMySQLLibrary::CreateMysqlObject(UObject* WorldContextObject, const FString &InUser, const FString &InHost, const FString &InPawd, const FString &InDB, const int32 InPort, const TArray<ESimpleClientFlags> &ClientFlag, const FString &Unix_Socket /*= TEXT("\0")*//*= 0*/)
{
	USimpleMysqlObject * MysqlObject = NULL;

	auto CallMainThreadWork = [&]()->USimpleMysqlObject*
	{
#ifdef  PLATFORM_PROJECT
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			if (USimpleMysqlObject *Object = NewObject<USimpleMysqlObject>(World))
			{
				Object->InitMysql(InUser, InHost, InPawd, InDB, InPort, Unix_Socket, ClientFlag);	
			
				return Object;
			}
		}
		return nullptr;
#else
		USimpleMysqlObject* Object = NewObject<USimpleMysqlObject>();
		if (Object)
		{
			Object->InitMysql(InUser, InHost, InPawd, InDB, InPort, Unix_Socket, ClientFlag);
			Object->AddToRoot();
		}

		return Object;
#endif // PLATFORM_PROJECT
	};

	if (FPlatformTLS::GetCurrentThreadId() == GGameThreadId)
	{
		MysqlObject = CallMainThreadWork();
	}
	else
	{
		//呼叫主线程
		FGraphEventRef EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			MysqlObject = CallMainThreadWork();
		},
		TStatId(),
		NULL,
		ENamedThreads::GameThread);

		//异步线程 阻塞
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);
	}

	return MysqlObject;
}

bool USimpleMySQLLibrary::QueryLinkStmt(USimpleMysqlObject *Object, const FString& SQL, const TArray<FSimpleMysqlQueryStmt>& InQueryStmtParam, FString& ErrMesg)
{
	if (Object)
	{
		return Object->GetLink()->QueryLinkStmt(SQL,InQueryStmtParam,ErrMesg);
	}

	return false;
}

bool USimpleMySQLLibrary::QueryLink(USimpleMysqlObject *Object, const FString &SQL, FString &ErrMesg)
{
	if (Object)
	{
		return Object->GetLink()->QueryLink(SQL, ErrMesg);
	}

	return false;
}

bool USimpleMySQLLibrary::SelectNewDB(USimpleMysqlObject *Object, const FString &NewDB, FString &ErrMesg)
{
	if (Object)
	{
		return Object->GetLink()->SelectNewDB(NewDB, ErrMesg);
	}

	return false;
}

bool USimpleMySQLLibrary::CreateDataBase(USimpleMysqlObject *Object, const FString &DataBaseName, EMysqlCharset Charset, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->CreateDataBase(DataBaseName, Charset, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::CreateAndSelectDataBase(USimpleMysqlObject *Object, const FString &DataBaseName, EMysqlCharset Charset, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->CreateAndSelectDataBase(DataBaseName, Charset, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::DropDataBase(USimpleMysqlObject *Object, const FString &DataBaseName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->DropDataBase(DataBaseName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::CreateTable(USimpleMysqlObject *Object, const FString &TableName, const TMap<FString, FMysqlFieldType> &InFields, const TArray<FString> &InPrimaryKeys, const FMysqlCreateTableParam &Param, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->CreateTable(TableName, InFields, InPrimaryKeys, Param, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::CreateTemporaryTable(USimpleMysqlObject *Object, const FString &TableName, const TMap<FString, FMysqlFieldType> &InFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->CreateTemporaryTable(TableName, InFields, QueryParam, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::CopyTableAToB(USimpleMysqlObject *Object, const FString &A, const FString &B, const TArray<FString> &Fields, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->CopyTableAToB(A, B, Fields, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::CopyTableAToTemporaryB(USimpleMysqlObject *Object, const FString &TableA, const FString &TemporaryTableB, const TArray<FString> &Fields, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->CopyTableAToTemporaryB(TableA, TemporaryTableB, Fields, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::ShowTables(USimpleMysqlObject *Object, const FString &TableName, EMysqlQuerySaveType SaveType, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug)
{
	if (Object)
	{
		return Object->GetLink()->ShowTables(TableName, SaveType, Results, ErrMesg, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::CopyTableAToBFast(USimpleMysqlObject *Object, const FString &CopySourceTable, const TMap<FString, FMysqlFieldType> &InsertFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->CopyTableAToBFast(CopySourceTable, InsertFields, QueryParam, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::CopyTableAToTemporaryBFast(USimpleMysqlObject *Object, const FString &CopySourceTable, const TMap<FString, FMysqlFieldType> &InsertFields, const FSimpleMysqlQueryParameters &QueryParam, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->CopyTableAToTemporaryBFast(CopySourceTable, InsertFields, QueryParam, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::DropTemporaryTable(USimpleMysqlObject *Object, const FString &TemporaryTableName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->DropTemporaryTable(TemporaryTableName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::DropTable(USimpleMysqlObject *Object, const FString &TableName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->DropTable(TableName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::TruncateTable(USimpleMysqlObject *Object, const FString &TableName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->TruncateTable(TableName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::DeleteFromTable(USimpleMysqlObject *Object, const FString &TableName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->DeleteFromTable(TableName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::DeleteFromTableWhereData(USimpleMysqlObject *Object, const FString &TableName, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->DeleteFromTableWhereData(TableName, Condition, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::OptimiseTable(USimpleMysqlObject *Object, const FString &TableName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->OptimiseTable(TableName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::GetSelectVersion(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/)
{
	if (Object)
	{
		return Object->GetLink()->GetSelectVersion(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetSelectDatabase(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/)
{
	if (Object)
	{
		return Object->GetLink()->GetSelectDatabase(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetSelectUser(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/)
{
	if (Object)
	{
		return Object->GetLink()->GetSelectUser(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetShowStatus(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/)
{
	if (Object)
	{
		return Object->GetLink()->GetShowStatus(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetShowVariables(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult> &Results, FString &ErrMesg, const FSimpleMysqlDebugResult &Debug, EMysqlQuerySaveType SaveType /*= EMysqlQuerySaveType::STORE_RESULT*/)
{
	if (Object)
	{
		return Object->GetLink()->GetShowVariables(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetSelectTableData(USimpleMysqlObject* Object, const FSimpleMysqlQueryParameters& QueryParam, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->GetSelectTableData(QueryParam, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::PrintResult(USimpleMysqlObject *Object, const TArray<FSimpleMysqlResult> &Results, bool bPrintToScreen /*= true*/, bool bPrintToLog /*= true*/)
{
	if (Object)
	{
		return Object->GetLink()->PrintResult(Results, bPrintToLog, bPrintToScreen);
	}

	return false;
}

bool USimpleMySQLLibrary::UpdateTableData(USimpleMysqlObject *Object, const FString &TableName, const TArray<FSimpleMysqlAssignment> &InFields, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->UpdateTableData(TableName, InFields, Condition, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::ReplaceTableData(USimpleMysqlObject *Object, const FString &TableName, const TArray<FSimpleMysqlReplaceParameters> &InReplaces, const TArray<FSimpleMysqlComparisonOperator> &Condition, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->ReplaceTableData(TableName, InReplaces, Condition, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::InsertTableData(USimpleMysqlObject *Object, const FString& TableName, const TMap<FString, FString> &InsetKeyValueDatas, const FSimpleMysqlQueryParameters &QueryParam, FString& ErrorMsg, bool bIgnore)
{
	if (Object)
	{
		return Object->GetLink()->InsertTableData(TableName, InsetKeyValueDatas, QueryParam, ErrorMsg, bIgnore);
	}

	return false;
}

bool USimpleMySQLLibrary::SimpleInsertTableData(USimpleMysqlObject *Object, const FString& TableName, const TArray<FString> &InsetValueDatas, const FSimpleMysqlQueryParameters &QueryParam, FString& ErrorMsg, bool bIgnore)
{
	if (Object)
	{
		return Object->GetLink()->SimpleInsertTableData(TableName, InsetValueDatas, QueryParam, ErrorMsg, bIgnore);
	}

	return false;
}

bool USimpleMySQLLibrary::ReplaceIfExistence(USimpleMysqlObject *Object, const FString& TableName, const TMap<FString, FString> &InsetKeyValueDatas, FString& ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->ReplaceIfExistence(TableName, InsetKeyValueDatas, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::StartTransaction(USimpleMysqlObject *Object, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->StartTransaction(ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::SetAutoCommit(USimpleMysqlObject *Object, bool bAuto, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->SetAutoCommit(bAuto, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::Commit(USimpleMysqlObject *Object, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->Commit(ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::SetSavePointName(USimpleMysqlObject *Object, const FString &SaveName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->SetSavePointName(SaveName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::Rollback(USimpleMysqlObject *Object, const FString &SaveName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->Rollback(SaveName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::ReleaseSavePoint(USimpleMysqlObject *Object, const FString &SaveName, FString &ErrorMsg)
{
	if (Object)
	{
		return Object->GetLink()->ReleaseSavePoint(SaveName, ErrorMsg);
	}

	return false;
}

bool USimpleMySQLLibrary::CreateFunction(USimpleMysqlObject *Object, const FString &InFunctionName, const TMap<FString, FMysqlFieldType> &InParameter, const FString &InFunctionBody, FString& ErrMesg, EMysqlVariableType InReturnsType /*= EMysqlVariableType::INT*/, const FString &InComment /*= TEXT("")*/)
{
	if (Object)
	{
		return Object->GetLink()->CreateFunction(InFunctionName, InParameter, InFunctionBody, ErrMesg, InReturnsType, InComment);
	}

	return false;
}

bool USimpleMySQLLibrary::ExecuteFunction(USimpleMysqlObject *Object, const FString &InFunctionName, const TArray<FString> &InParameter, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->ExecuteFunction(InFunctionName, InParameter, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DropFunction(USimpleMysqlObject *Object, const FString &InFunctionName, FString& ErrMesg)
{
	if (Object)
	{
		return Object->GetLink()->DropFunction(InFunctionName, ErrMesg);
	}

	return false;
}

bool USimpleMySQLLibrary::ShowCreateFunction(USimpleMysqlObject *Object, const FString &InFunctionName, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->ShowCreateFunction(InFunctionName, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::ShowFunctionsStatus(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->ShowFunctionsStatus(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DefiningVariables(USimpleMysqlObject *Object, const FString &InVariablesName, const FString &InValue, FString &ErrMesg)
{
	if (Object)
	{
		return Object->GetLink()->DefiningVariables(InVariablesName, InValue, ErrMesg);
	}

	return false;
}

bool USimpleMySQLLibrary::CreateProcedure(USimpleMysqlObject *Object, const FString &InProcedureName, const TMap<FString, FMysqlFieldType> &InParameter, const FString &InProcedureBody, FString& ErrMesg, const FString &InComment /*= TEXT("")*/)
{
	if (Object)
	{
		return Object->GetLink()->CreateProcedure(InProcedureName, InParameter, InProcedureBody, ErrMesg, InComment);
	}

	return false;
}

bool USimpleMySQLLibrary::ExecuteProcedure(USimpleMysqlObject *Object, const FString &InProcedureName, const TArray<FString> &InParameter, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->ExecuteProcedure(InProcedureName, InParameter, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DropProcedure(USimpleMysqlObject *Object, const FString &InProcedureName, FString& ErrMesg)
{
	if (Object)
	{
		return Object->GetLink()->DropProcedure(InProcedureName, ErrMesg);
	}

	return false;
}

bool USimpleMySQLLibrary::ShowCreateProcedure(USimpleMysqlObject *Object, const FString &InProcedureName, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->ShowCreateProcedure(InProcedureName, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::ShowProceduresStatus(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->ShowProceduresStatus(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::YearWeek(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->YearWeek(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::WeekOfYear(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->WeekOfYear(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::WeekDay(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->WeekDay(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Year(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Year(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Week(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Week(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::ToDays(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->ToDays(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::TimeStamp(USimpleMysqlObject *Object, const FString& InExpression, const FString& InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->TimeStamp(InExpression, InInterval, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::TimeDiff(USimpleMysqlObject *Object, const FString& InTime1, const FString& InTime2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->TimeDiff(InTime1, InTime2, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::TimeToSec(USimpleMysqlObject *Object, const FString& InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->TimeToSec(InTime, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::IsFieldExistInTable(USimpleMysqlObject *Object, const FString &InTableName, const FString &InFieldName, FString &ErrMesg)
{
	if (Object)
	{
		return Object->GetLink()->IsFieldExistInTable(InTableName, InFieldName, ErrMesg);
	}

	return false;
}

bool USimpleMySQLLibrary::TimeFormat(USimpleMysqlObject *Object, const FString& InTime, const FString& InFormat, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->TimeFormat(InTime, InFormat, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Time(USimpleMysqlObject *Object, const FString& InExpression, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Time(InExpression, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::SysDate(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->SysDate(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::SubTime(USimpleMysqlObject *Object, const FString& InTime, int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->SubTime(InTime, InSecond, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::SubDate(USimpleMysqlObject *Object, const FString& InDate, int64 InDay, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->SubDate(InDate, InDay, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::StringToDate(USimpleMysqlObject *Object, const FString& InString, const FString& InFormatMask, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->StringToDate(InString, InFormatMask, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::SecToTime(USimpleMysqlObject *Object, const int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->SecToTime(InSecond, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Second(USimpleMysqlObject *Object, const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Second(InTime, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Quarter(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Quarter(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::PeriodDiff(USimpleMysqlObject *Object, const int64 Period1, const int64 Period2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->PeriodDiff(Period1, Period2, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::PeriodAdd(USimpleMysqlObject *Object, const int64 InPeriod, const int64 InMonths, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->PeriodAdd(InPeriod, InMonths, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Now(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Now(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Minute(USimpleMysqlObject *Object, const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Minute(InTime, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::MicroSecond(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->MicroSecond(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::MakeTime(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->MakeTime(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::MakeDate(USimpleMysqlObject *Object, const int32 InYear, const int64 DayOfYear, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->MakeDate(InYear, DayOfYear, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::LocalTimeStamp(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->LocalTimeStamp(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::LocalTime(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->LocalTime(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::LastDay(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->LastDay(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Hour(USimpleMysqlObject *Object, const FString &InTime, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Hour(InTime, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::FromDays(USimpleMysqlObject *Object, const int64 InDay, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->FromDays(InDay, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Extract(USimpleMysqlObject *Object, const FString &InDate, const EMysqlDateType DateType, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Extract(InDate, DateType, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DayOfYear(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->DayOfYear(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DayOfWeek(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->DayOfWeek(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DayOfMonth(USimpleMysqlObject *Object, const FString &InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->DayOfMonth(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DayName(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->DayName(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Day(USimpleMysqlObject *Object, const FString& InDate, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Day(InDate, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DateSub(USimpleMysqlObject *Object, const FString& InDate, const int64 InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->DateSub(InDate, InInterval, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DateFormat(USimpleMysqlObject *Object, const FString& InDate, const FString& InFormat, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->DateFormat(InDate, InFormat, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::AddDate(USimpleMysqlObject *Object, const FString& InDate, const int64 InInterval, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->AddDate(InDate, InInterval, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::DateDiff(USimpleMysqlObject *Object, const FString& InD1, const FString& InD2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->DateDiff(InD1, InD2, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Date(USimpleMysqlObject *Object, const FString& InExpression, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Date(InExpression, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::CurTime(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->CurTime(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::CurrentTime(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->CurrentTime(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::CurrentTimeStamp(USimpleMysqlObject *Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->CurrentTimeStamp(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::AddTime(USimpleMysqlObject *Object, const FString& InDate, const int64 InSecond, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->AddTime(InDate, InSecond, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Bin(USimpleMysqlObject* Object, const int64 Value, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Bin(Value, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Binary(USimpleMysqlObject* Object, const FString& Value, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Binary(Value, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Cast(USimpleMysqlObject* Object, const FString& Value, EMysqlVariableType VariableType, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Cast(Value, VariableType, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Conv(USimpleMysqlObject* Object, const FString& Value, int64 F1, int64 F2, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Conv(Value, F1, F2, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Convert(USimpleMysqlObject* Object, const FString& InString, EMysqlCharset Charset, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Convert(InString, Charset, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::BinocularOperation(USimpleMysqlObject* Object, const FString& Expression, const FString& ValueA, const FString& ValueB, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->BinocularOperation(Expression, ValueA, ValueB, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::IsNULL(USimpleMysqlObject* Object, const FString& Expression, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->IsNULL(Expression, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::NULLIf(USimpleMysqlObject* Object, const FString& ValueA, const FString& ValueB, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->NULLIf(ValueA, ValueB, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::Charset(USimpleMysqlObject* Object, const FString& Value, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->Charset(Value, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetConnection_ID(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->GetConnection_ID(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetCurrent_user(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->GetCurrent_user(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetLast_Insert_ID(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->GetLast_Insert_ID(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetSession_user(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->GetSession_user(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetSystem_user(USimpleMysqlObject* Object, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->GetSystem_user(Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

bool USimpleMySQLLibrary::GetSelectTableDataSQL(USimpleMysqlObject *Object, FString &SQL, const FSimpleMysqlQueryParameters &QueryParam)
{
	if (Object)
	{
		return Object->GetLink()->GetSelectTableDataSQL(SQL, QueryParam);
	}

	return false;
}

bool USimpleMySQLLibrary::QueryLinkResult(USimpleMysqlObject *Object, const FString &SQL, TArray<FSimpleMysqlResult>& Results, FString& ErrMesg, EMysqlQuerySaveType SaveType, const FSimpleMysqlDebugResult& Debug)
{
	if (Object)
	{
		return Object->GetLink()->QueryLinkResult(SQL, Results, ErrMesg, SaveType, Debug);
	}

	return false;
}

TArray<FString> USimpleMySQLLibrary::GetStringNULLArray()
{
	return TArray<FString>();
}

TArray<FSimpleMysqlAssignment> USimpleMySQLLibrary::GetAssignmentNULLArray()
{
	return TArray<FSimpleMysqlAssignment>();
}

TArray<FSimpleMysqlComparisonOperator> USimpleMySQLLibrary::GetComparisonOperatorNULLArray()
{
	return TArray<FSimpleMysqlComparisonOperator>();
}

TArray<FSimpleMysqlReplaceParameters> USimpleMySQLLibrary::GetReplaceParametersNULLArray()
{
	return TArray<FSimpleMysqlReplaceParameters>();
}

TMap<FString, FString> USimpleMySQLLibrary::GetMapStringNULL()
{
	return TMap<FString, FString>();
}

FSimpleMysqlQueryParameters USimpleMySQLLibrary::GetNULLQueryParameters()
{
	return FSimpleMysqlQueryParameters();
}

TMap<FString, FMysqlFieldType> USimpleMySQLLibrary::GetFieldNULL()
{
	return TMap<FString, FMysqlFieldType>();
}

FSimpleMysqlJoinParameters USimpleMySQLLibrary::GetJoinParametersNULL()
{
	return FSimpleMysqlJoinParameters();
}

FSimpleMysqlModifyField USimpleMySQLLibrary::GetModifyFieldNULL()
{
	return FSimpleMysqlModifyField();
}

TArray<FSimpleMysqlQueryOrderBy> USimpleMySQLLibrary::GetOrderByNULLArray()
{
	return TArray<FSimpleMysqlQueryOrderBy>();
}

TArray<FSimpleMysqlJoinParameters> USimpleMySQLLibrary::GetJoinParamNULLArray()
{
	return TArray<FSimpleMysqlJoinParameters>();
}
