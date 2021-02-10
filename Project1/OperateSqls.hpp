#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<ctime>
#include "AddSql.hpp"
#include<vector>
#include<io.h>
using namespace std;

class OperateSql
{
public:
    AddSql sql;
    explicit OperateSql();
    ~OperateSql();
    void startInsert(string path,string name,string logpath);
};

OperateSql::OperateSql()  {}
OperateSql::~OperateSql(){}

void OperateSql:: startInsert(string path, string name,string logpath)
{
    long timeBegin = clock();
    long timeEnd;
    sql.initSql();
    sql.connSql();
    ifstream ifs(path);
    ofstream ofs(logpath,ios::app);
    long i = 0;
    char bf[300] = { 0 };
    if (ifs.is_open())
    {
        
        string createtable ="create table "+name+"(id int identity(1,1), primary key (id),Timestamp datetime2(3) not null,Bid float not null,Ask float not null,)";
        ofs << createtable << endl;
        cout << createtable << endl;
        sql.executQuery(createtable);
        cout<< "Create table " << name <<endl<< endl;

        if (ifs.good())
            ifs.getline(bf, 200);
      
        stringstream ss;
        string tsp;
        ss << "INSERT INTO "<<name<<"(Timestamp,Bid,Ask)VALUES\n";
        while (!ifs.eof())
        {
            ifs.getline(bf, 200);
            tsp = bf;
            ss << "('" << tsp.substr(0, 4) << '-' << tsp.substr(4, 2) << '-'
                << tsp.substr(6, 11) << '.' << tsp.substr(18, 3) + "'";

            int idx = tsp.find_last_of(',');
            tsp = tsp.substr(21, idx - 22);
            int idx2 = tsp.find_last_of(',');
            tsp = tsp.substr(0, idx2);
            ss << tsp << ")," << endl;
            ++i;
            if (0 == i%1000)
            {
                int len = ss.str().length();
                tsp = ss.str().substr(0, len - 2) + ';';
                sql.executQuery(tsp);
                ss.str("");
                ss << "INSERT INTO " << name << "(Timestamp,Bid,Ask)VALUES";
            }
        }
        if (ss.good())
        {
            int len = ss.str().length();
            tsp = ss.str().substr(0, len - 2) + ';';
            sql.executQuery(tsp);
            ss.str("");
        }
    }

    ifs.close();
    timeEnd = clock();
    cout << "Use times " << timeEnd - timeBegin << 
        " finished the insert.\n\n";
    ofs << "Use " << timeEnd - timeBegin <<
        "milliseconds to insert "<<i<<" lines.\n\n";
    ofs.close();
}


//string createtable = "use MoviesProject"
//"IF EXISTS(SELECT * FROM sys.objects WHERE name = 'Movies')"
//"BEGIN"
//"DROP TABLE Movies;"
//"END"
//"create table Users("
//"Id int not null,"
//"Age int not null,"
//"Gender bit not null,"
//"Occupation varchar(150)"
//")";