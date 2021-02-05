//include the below additional libraries
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
//https://www.youtube.com/watch?v=yNniOHn9Xe0
using namespace std;
int main()
{
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
    //define handles and variables
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLHANDLE sqlEnvHandle;
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];

    //initializations
    sqlConnHandle = NULL;
    sqlStmtHandle = NULL;
    //allocations
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        goto COMPLETED;
    //output
    cout << "Attempting connection to SQL Server...";
    cout << "\n";
    //connect to SQL Server  
    //I am using a trusted connection and port 14808
    //it does not matter if you are using default or named instance
    //just make sure you define the server name and the port
    //You have the option to use a username/password instead of a trusted connection
    //but is more secure to use a trusted connection
    switch (SQLDriverConnect(sqlConnHandle,
        NULL,
        //(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=master;UID=username;PWD=password;",
        (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=dolphin;Trusted=true;",
        SQL_NTS,
        retconstring,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT))
    {
    case SQL_SUCCESS:
        cout << "Successfully connected to SQL Server";
        cout << "\n";
        break;
    case SQL_SUCCESS_WITH_INFO:
        cout << "Successfully connected to SQL Server";
        cout << "\n";
        break;
    case SQL_INVALID_HANDLE:
        cout << "Could not connect to SQL Server";
        cout << "\n";
        goto COMPLETED;
    case SQL_ERROR:
        cout << "Could not connect to SQL Server";
        cout << "\n";
        goto COMPLETED;
    default:
        break;
    }
    //if there is a problem connecting then exit application
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        goto COMPLETED;
    //output
    cout << "\n";
    cout << "Executing T-SQL query...";
    cout << "\n";
    //if there is a problem executing the query then exit application
    //else display query result
    //if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT @@VERSION", SQL_NTS))
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT * from [user]", SQL_NTS)) 
    {
        cout << "Error querying SQL Server";
        cout << "\n";
        goto COMPLETED;
    }
    else 
    {
        SQLCHAR sqlVersion[SQL_RESULT_LEN];//char[]
        //declare output variable and pointer
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) //line
        {
            for (int i = 1; i <= 10; i++)//column
            {
                SQLGetData(sqlStmtHandle, i, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);
                //display query result
                if (ptrSqlVersion>0)//return char length
                {
                    cout <<i<<"->"<< sqlVersion <<"\t";
                }
                else
                {
                    cout <<i<< "->NULL\t";
                }  
            }
            cout << endl;
        }
    }
    //close connection and free resources
COMPLETED:
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
    //pause the console window - exit when key is pressed
    cout << "\nPress any key to exit...";
    getchar();
}




















/*  MkDB.c  To get this working in DevC++ create a new C project in its own directory
    such as MkDB, then copy this file into a C source code file and name it Main.c.
    Then go to the 'Project' on the main menu and select 'Project Options'.  A tabbed
    dialog box will open and select the 'Parameters' tab.  The 'Linker' list box at the
    far right has a button under it  which when clicked will display an 'Open File'
    dialog box where you will have to navigate to the 'lib' directory.  It is a multi-
    file selection dialog box and you will have to select the libodbc32.a and the
    libodbccp32.a libraries.
*/
//#include "windows.h"
//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
//#include "odbcinst.h"
//#include "sql.h"
//#include "sqlext.h"
//
//typedef struct tagSQL
//{
//    SQLCHAR         szCnIn[512];
//    SQLCHAR         szCnOut[512];
//    short int       iBytes;
//    SWORD           swStrLen;
//    SQLHENV         hEnvr;
//    SQLHDBC         hConn;
//    unsigned int    blnConnected;
//}SQL, * lpSql;
//
//typedef struct tagDiagRec
//{
//    SQLINTEGER         iNativeErrPtr;
//    SQLSMALLINT        iTextLenPtr;
//    SQLCHAR            szErrMsg[512];
//    SQLCHAR            szErrCode[8];
//}DIAGNOSTICRECORD;
//
//
//void ODBCConnect(SQLCHAR* szDB, lpSql sql)
//{
//    DIAGNOSTICRECORD dr;
//    char lpBuffer[512];
//    DWORD nBufLen = 512;
//    UINT iResult;
//
//    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sql->hEnvr);
//    SQLSetEnvAttr(sql->hEnvr, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
//    SQLAllocHandle(SQL_HANDLE_DBC, sql->hEnvr, &sql->hConn);
//    strcpy(sql->szCnIn, "DRIVER=Microsoft Access Driver (*.mdb);DBQ=");
//    GetCurrentDirectory(nBufLen, lpBuffer);
//    printf("lpBuffer=%s\n", lpBuffer);
//    strcat(sql->szCnIn, lpBuffer);
//    strcat(sql->szCnIn, "\\");
//    strcat(sql->szCnIn, szDB);
//    strcat(sql->szCnIn, ";");
//    printf("sql->szCnIn = %s\n", sql->szCnIn);
//    iResult =
//        SQLDriverConnect
//        (
//            sql->hConn,
//            NULL,
//            sql->szCnIn,
//            (SQLSMALLINT)strlen(sql->szCnIn),
//            sql->szCnOut,
//            512,
//            &sql->swStrLen,
//            SQL_DRIVER_NOPROMPT
//        );
//    if (!iResult)
//        sql->blnConnected = TRUE;
//    else
//    {
//        SQLGetDiagRec
//        (
//            SQL_HANDLE_DBC,
//            sql->hConn,
//            1,
//            dr.szErrCode,
//            &dr.iNativeErrPtr,
//            dr.szErrMsg,
//            512,
//            &dr.iTextLenPtr
//        );
//        printf("dr.szErrCode = %s\n", dr.szErrCode);
//        printf("dr.szErrMsg  = %s\n", dr.szErrMsg);
//        sql->blnConnected = FALSE;
//        SQLDisconnect(sql->hConn);
//        SQLFreeHandle(SQL_HANDLE_DBC, sql->hConn);
//        SQLFreeHandle(SQL_HANDLE_ENV, sql->hEnvr);
//    }
//}
//
//
//void ODBCDisconnect(lpSql sql)
//{
//    SQLDisconnect(sql->hConn);
//    SQLFreeHandle(SQL_HANDLE_DBC, sql->hConn);
//    SQLFreeHandle(SQL_HANDLE_ENV, sql->hEnvr);
//}
//
//
//void InstallerError(void)
//{
//    DWORD pErr;
//    SQLCHAR szErrMsg[512];
//    WORD cbMsgBuffer = 512;
//    WORD cbRet;
//    WORD wErrNum = 1;
//
//    while (SQLInstallerError(wErrNum, &pErr, szErrMsg, cbMsgBuffer, &cbRet) != SQL_NO_DATA)
//    {
//        printf("%u\t%u\t%s\n", wErrNum, pErr, szErrMsg);
//        wErrNum++;
//    };
//
//    return;
//}
//
//
//UINT blnCreateDB(SQLCHAR* szDBName)
//{
//    SQLCHAR szCreate[24];
//
//    printf("szDBName=%s\n", szDBName);
//    strcpy(szCreate, "CREATE_DB=");
//    strcat(szCreate, szDBName);
//    printf("szCreate=%s\n", szCreate);
//    if (SQLConfigDataSource(0, ODBC_ADD_DSN, "Microsoft Access-Treiber (*.mdb)", szCreate))
//    {
//        printf("SQLConfigDataSource() returned TRUE\n");
//        return TRUE;
//    }
//    else
//    {
//        InstallerError();
//        return FALSE;
//    }
//}
//
//
//UINT blnMakeTable(lpSql sql)
//{
//    SQLCHAR szQuery[256];
//    SQLHSTMT hStmt;
//
//    strcpy(
//        szQuery,
//        "CREATE TABLE Table1 " \
//        "(" \
//        "Id LONG  NOT NULL PRIMARY KEY," \
//        "Float_Point DOUBLE," \
//        "Date_Field DATETIME," \
//        "Text_Field CHAR(30)" \
//        ");");
//    printf("%s\n", szQuery);
//    SQLAllocHandle(SQL_HANDLE_STMT, sql->hConn, &hStmt);
//    if (SQLExecDirect(hStmt, szQuery, SQL_NTS) == 0)
//    {
//        puts("Table1 Successfully Created!");
//        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
//        return(TRUE);
//    }
//    else
//    {
//        puts("Table Creation Failure!");
//        return(FALSE);
//    }
//}
//
//
//TIMESTAMP_STRUCT ParseDate(SQLCHAR* szDate, SQLCHAR* szFormat, SQLCHAR* szDelimiter)
//{
//    UINT i = 0, j = 0, k = 0;
//    TIMESTAMP_STRUCT ts;
//    SQLCHAR buf[3][8];          //buf[0] for month, buf[1] for day, buf[2] for year
//    SQLCHAR* p;
//
//    memset(buf, 0, sizeof(buf));  //zero out buf[]
//    p = szDate;
//    for (i = 0; i < strlen(szDate); i++)
//    {
//        if (*p != *szDelimiter)
//        {
//            buf[j][k++] = *p;
//            buf[j][k + 1] = '\0';
//        }
//        else
//        {
//            j++;
//            k = 0;
//        }
//        p++;
//    }
//    if (!strcmpi(szFormat, "MDY"))
//    {
//        ts.month = atoi(buf[0]);
//        ts.day = atoi(buf[1]);
//        ts.year = atoi(buf[2]);
//    }
//    if (!strcmpi(szFormat, "DMY"))
//    {
//        ts.day = atoi(buf[0]);
//        ts.month = atoi(buf[1]);
//        ts.year = atoi(buf[2]);
//    }
//    if (!strcmpi(szFormat, "YMD"))
//    {
//        ts.year = atoi(buf[0]);
//        ts.month = atoi(buf[1]);
//        ts.day = atoi(buf[2]);
//    }
//
//    return ts;
//}
//
//
//UINT blnInsert(lpSql sql)
//{
//    TIMESTAMP_STRUCT ts;
//    DIAGNOSTICRECORD dr;
//    SQLHSTMT         hStmt;
//    SQLINTEGER       iJnk;
//    SQLINTEGER       iNts = SQL_NTS;
//    UINT             iId[] = { 1,2,3,4 };
//    double           dblNum[] = { 3.14159,1.23456,15.1234,0.54321 };
//    SQLCHAR* szDate[] = { "11/15/1952","6/30/1969","1/1/2006","4/1/2006" };
//    SQLCHAR* szStr[] = { "My Birthday","Walk On Moon?","Some String","April Fools Day" };
//    SQLCHAR          szQuery[100], szString[30];
//    UINT             i, id, iRet = FALSE;
//    double           dbl;
//
//    if (SQLAllocHandle(SQL_HANDLE_STMT, sql->hConn, &hStmt) == SQL_SUCCESS)
//    {
//        strcpy(
//            szQuery,
//            "INSERT INTO Table1 " \
//            "(" \
//            "Id," \
//            "Float_Point," \
//            "Date_Field," \
//            "Text_Field" \
//            ") " \
//            "VALUES(?,?,?,?);");
//        printf("%s\n", szQuery);
//        if (SQLPrepare(hStmt, szQuery, SQL_NTS) == SQL_SUCCESS)
//        {
//            SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, &iJnk);
//            SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &dbl, 0, &iJnk);
//            SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_TYPE_DATE, 0, 0, &ts, 0, &iJnk);
//            SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 29, 0, szString, 30, &iNts);
//            printf("\n                                                         SQLExecute(hStmt)\n");
//            printf("iId      Double           Date           String           0=SQL_SUCCESS  \n");
//            printf("========================================================================\n");
//            for (i = 0; i < sizeof(iId) / sizeof(iId[0]); i++)
//            {
//                id = iId[i]; dbl = dblNum[i];
//                ts = ParseDate(szDate[i], "mdy", "/");
//                strcpy(szString, szStr[i]);
//                if (SQLExecute(hStmt) == SQL_SUCCESS)
//                    printf("%u\t%f\t%s\t%s\t\t%u\n", id, dbl, szDate[i], szString, SQL_SUCCESS);
//                else
//                {
//                    SQLGetDiagRec
//                    (
//                        SQL_HANDLE_DBC,
//                        sql->hConn,
//                        1,
//                        dr.szErrCode,
//                        &dr.iNativeErrPtr,
//                        dr.szErrMsg,
//                        512,
//                        &dr.iTextLenPtr
//                    );
//                    printf("dr.szErrCode = %s\n", dr.szErrCode);
//                    printf("dr.szErrMsg  = %s\n", dr.szErrMsg);
//                    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
//                    return FALSE;
//                }
//            }
//            iRet = TRUE;
//            printf("\n");
//        }
//        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
//    }
//
//    return iRet;
//}
//
//
//SQLCHAR* MkDate(TIMESTAMP_STRUCT* ts)
//{
//    SQLCHAR szMonth[4], szDay[4], szYear[8];
//    static  SQLCHAR szDate[12];
//
//    sprintf(szMonth, "%u", ts->month);
//    sprintf(szDay, "%u", ts->day);
//    sprintf(szYear, "%u", ts->year);
//    strcpy(szDate, szMonth);
//    strcat(szDate, "/");
//    strcat(szDate, szDay);
//    strcat(szDate, "/");
//    strcat(szDate, szYear);
//
//    return szDate;
//}
//
//
//UINT blnDumpData(lpSql sql)
//{
//    SQLHSTMT          hStmt;
//    SQLCHAR           szQuery[100];
//    SQLINTEGER        iJnk;
//    UINT              iId;
//    double            dblNum;
//    TIMESTAMP_STRUCT  ts;
//    SQLCHAR           szString[30];
//
//    if (SQLAllocHandle(SQL_HANDLE_STMT, sql->hConn, &hStmt) == SQL_SUCCESS)
//    {
//        strcpy(
//            szQuery,
//            "SELECT " \
//            "Table1.Id," \
//            "Table1.Float_Point," \
//            "Table1.Date_Field," \
//            "Table1.Text_Field " \
//            "FROM " \
//            "Table1;");
//        printf("%s\n\n", (szQuery));
//        SQLBindCol(hStmt, 1, SQL_C_ULONG, &iId, 0, &iJnk);
//        SQLBindCol(hStmt, 2, SQL_C_DOUBLE, &dblNum, 0, &iJnk);
//        SQLBindCol(hStmt, 3, SQL_C_TYPE_DATE, &ts, 0, &iJnk);
//        SQLBindCol(hStmt, 4, SQL_C_CHAR, szString, 30, &iJnk);
//        puts(" iId      Double           Date           String");
//        puts("=====================================================");
//        if (SQLExecDirect(hStmt, szQuery, SQL_NTS) == SQL_SUCCESS)
//        {
//            while (SQLFetch(hStmt) != SQL_NO_DATA)
//            {
//                printf("%u\t%f\t%s\t%s\n", iId, dblNum, MkDate(&ts), szString);
//            }
//        }
//        SQLCloseCursor(hStmt);
//        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
//        return TRUE;
//    }
//
//    return FALSE;
//}
//
//
//int main(int argc, char* argv[])
//{
//    SQLCHAR szDBName[24];
//    SQL sql;
//
//    strcpy(szDBName, "TestData.mdb");
//    if (blnCreateDB(szDBName))
//    {
//        ODBCConnect(szDBName, &sql);
//        if (sql.blnConnected == TRUE)
//        {
//            puts("We're Connected!");
//            if (blnMakeTable(&sql))
//            {
//                puts("Table1 Successfully Created!");
//                if (blnInsert(&sql) == TRUE)
//                    printf("Data Insertion Successful!\n\n");
//                else
//                    puts("Data Insertion Failure!");
//                if (blnDumpData(&sql) == TRUE)
//                    puts("\nblnDumpData==TRUE");
//                else
//                    puts("\nblnDumpData==FALSE");
//            }
//            else
//                puts("Table Creation Failure!");
//            ODBCDisconnect(&sql);
//        }
//        else
//            puts("Connection Failure!");
//    }
//    else
//        puts("Database Creation Failure!");
//    system("PAUSE");
//
//    return 0;
//}

/*
szDBName=TestData.mdb
szCreate=CREATE_DB=TestData.mdb
SQLConfigDataSource() returned TRUE
lpBuffer=C:\Program Files\Microsoft Visual Studio\My Projects\MkDB\Release
sql->szCnIn = DSN=MS Access Database;DBQ=C:\Program Files\Microsoft Visual Studio\My Projects\MkDB\Rel
ease\TestData.mdb;DefaultDir=C:\Program Files\Microsoft Visual Studio\My Projects\MkDB\Release;DriverI
d=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;
We're Connected!
CREATE TABLE Table1 (Id LONG  NOT NULL PRIMARY KEY,Float_Point DOUBLE,Date_Field DATETIME,Text_Field C
HAR(30));
Table1 Successfully Created!
Table1 Successfully Created!
INSERT INTO Table1 (Id,Float_Point,Date_Field,Text_Field) VALUES(?,?,?,?);

                                                         SQLExecute(hStmt)
iId      Double           Date           String           0=SQL_SUCCESS
========================================================================
1       3.141590        11/15/1952      My Birthday             0
2       1.234560        6/30/1969       Walk On Moon?           0
3       15.123400       1/1/2006        Some String             0
4       0.543210        4/1/2006        April Fools Day         0

Data Insertion Successful!

SELECT Table1.Id,Table1.Float_Point,Table1.Date_Field,Table1.Text_Field FROM Table1;

 iId      Double           Date           String
=====================================================
1       3.141590        11/15/1952      My Birthday
2       1.234560        6/30/1969       Walk On Moon?
3       15.123400       1/1/2006        Some String
4       0.543210        4/1/2006        April Fools Day

blnDumpData==TRUE
Press any key to continue . . .
*/














