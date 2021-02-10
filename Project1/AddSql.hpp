#pragma once
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include<string>
using namespace std;
class AddSql
{
private:
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLHANDLE sqlEnvHandle;
    SQLWCHAR retconstring[2048];

public:
    AddSql();
    ~AddSql();
    void initSql();
    void connSql();
    void executQuery(const string& query);
    void leave(SQLHANDLE sqlConnHandle, SQLHANDLE sqlStmtHandle, SQLHANDLE sqlEnvHandle);
};

void AddSql::executQuery(const string& query)
{
    wstring stemp = wstring(query.begin(), query.end());
    LPCWSTR sw = stemp.c_str();

    //cout << "\nExecuting T-SQL query...\n";
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)sw, SQL_NTS))//L"SELECT * from [user]"
    {
        cout << "Error querying SQL Server\n";
        leave(sqlConnHandle, sqlStmtHandle, sqlEnvHandle);
    }
    else
    {
        //cout << "Insert success" << endl;
        //leave(sqlConnHandle, sqlStmtHandle, sqlEnvHandle);
        //SQLCHAR sqlVersion[256];
        //SQLINTEGER ptrSqlVersion;
        //while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
        //{
        //    for (int i = 1; i <= 10; i++)//column
        //    {
        //        SQLGetData(sqlStmtHandle, i, SQL_CHAR, sqlVersion, 256, &ptrSqlVersion);
        //        if (ptrSqlVersion > 0)//return char length
        //        {
        //            cout << i << "->" << sqlVersion << "\t";
        //        }
        //        else
        //        {
        //            cout << i << "->NULL\t";
        //        }
        //    }
        //    cout << endl;
        //}
    }
}
void AddSql::leave(SQLHANDLE sqlConnHandle,
    SQLHANDLE sqlStmtHandle,
    SQLHANDLE sqlEnvHandle)
{
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
}

AddSql::AddSql()
{
    sqlConnHandle = NULL;
    sqlStmtHandle = NULL;
}
AddSql::~AddSql() {}
void AddSql::connSql()
{
    cout << "Attempting connection to SQL Server...\n";
    switch (SQLDriverConnect(sqlConnHandle,
        NULL,
        (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=dolphin;Trusted=true;",
        SQL_NTS,
        retconstring,
        2048,
        NULL,
        SQL_DRIVER_NOPROMPT))
    {
    case SQL_SUCCESS:
        cout << "Successfully connected to SQL Server\n";
        break;
    case SQL_SUCCESS_WITH_INFO:
        cout << "Successfully connected to SQL Server\n";
        break;
    case SQL_INVALID_HANDLE:
    {
        cout << "Could not connect to SQL Server\\n";
        leave(sqlConnHandle, sqlStmtHandle, sqlEnvHandle);
        break;
    }
    case SQL_ERROR:
    {
        cout << "Could not connect to SQL Server\n";
        leave(sqlConnHandle, sqlStmtHandle, sqlEnvHandle);
        break;
    }
    default:
        break;
    }
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        leave(sqlConnHandle, sqlStmtHandle, sqlEnvHandle);
    }
}
void AddSql::initSql()
{
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
    {
        leave(sqlConnHandle, sqlStmtHandle, sqlEnvHandle);
    }
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
    {
        leave(sqlConnHandle, sqlStmtHandle, sqlEnvHandle);
    }
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
    {
        leave(sqlConnHandle, sqlStmtHandle, sqlEnvHandle);
    }
}
