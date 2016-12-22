// dbFinalPractice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "sql.h"
#include "sqlext.h"
#include "locale.h"

int main()
{
	std::cout << "주문 건수 가 2건 이상인 책은 다음과 같습니다.\n";
	_wsetlocale(LC_ALL, L"korean");
	int a = 0;
	SQLHENV hEnv; //환경 핸들
	SQLHDBC hDbcon; //디비 연결 핸들
	SQLHSTMT hStmt; //명령핸들
	SQLRETURN returnVal; //최후에 실행한 SQL문의 결과값
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbcon);
	returnVal = SQLConnect(hDbcon, (SQLWCHAR *)L"dbLee", SQL_NTS, (SQLWCHAR *)L"nextuser", SQL_NTS,
		(SQLWCHAR *)L"dbgood", SQL_NTS);
	if (returnVal == SQL_SUCCESS || returnVal == SQL_SUCCESS_WITH_INFO)
	{
		returnVal = SQLAllocHandle(SQL_HANDLE_STMT, hDbcon, &hStmt);
		if (returnVal == SQL_SUCCESS || returnVal == SQL_SUCCESS_WITH_INFO)
		{
			returnVal = SQLPrepare(hStmt, (SQLWCHAR*)L"USE hwkim;", SQL_NTS);
			returnVal = SQLExecute(hStmt);

		}
		else
		{
			hStmt = 0;
			return 1;
		}
	}
	else
	{
		return 1;
	}

	returnVal = SQLExecDirect(hStmt, (SQLWCHAR *)L"select b.name , count(b.idb) from book as b inner join order_has_book as o on o.book_idb = b.idb group by b.idb having count(b.idb) >= 2; ", SQL_NTS);
	
	wchar_t title[200];

	int num;
	int idx=1;
	SQLLEN len_title, leg_num;
	SQLBindCol(hStmt, 1, SQL_C_WCHAR, &title, 201, &len_title);
	SQLBindCol(hStmt, 2, SQL_C_LONG, &num, 0, &leg_num);
	
	while (true)
	{
		 
		returnVal = SQLFetch(hStmt);
		if (returnVal == SQL_ERROR)
		{
			return -1;
		}
		if (returnVal == SQL_NO_DATA_FOUND)
		{
			break;
		}
		if (returnVal == SQL_SUCCESS || returnVal == SQL_SUCCESS_WITH_INFO)
		{
			wprintf(L"제목:%s\n", title,num);
			
		}
		++idx;
	}

	if (hStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	if (hDbcon)
	{
		SQLDisconnect(hDbcon);
	}
	if (hDbcon)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, hDbcon);
	}
	if (hEnv)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	}
    return 0;

}

