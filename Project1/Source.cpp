#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<ctime>
#include "AddSql.hpp"
using namespace std;

int main()
{
    AddSql sql;
    sql.initSql();
    sql.connSql();

    ifstream ifs("H:/Forex/AUDCAD.csv");
    long timeBegin = 0;
    long timeEnd = 0;

    char bf[300] = { 0 };

    if (ifs.is_open())
    {
        timeBegin = clock();
        if (ifs.good())
            ifs.getline(bf, 200);
        int i = 0;
        int j = 0;
        stringstream ss;
        string tsp;
        ss << "INSERT INTO AUDCAD(Timestamp,Bid,Ask)VALUES\n";
        while (ifs.good())
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
            if (1000==i)
            {
                int len = ss.str().length();
                tsp = ss.str().substr(0, len - 2) + ';';
                sql.executQuery(tsp);
                //cout << tsp << endl;
                j++;
                i = 0;
                ss.str("");
                ss << "INSERT INTO AUDCAD(Timestamp,Bid,Ask)VALUES\n";
                //system("pause");
            }
        }
        if (ss.good())
        {
            int len = ss.str().length();
            tsp = ss.str().substr(0, len - 2)+';';
            sql.executQuery(tsp);
            ss.str("");
        }
        
        timeEnd = clock();
        cout << j << endl;
    }
    cout << "Total time is:" << timeEnd - timeBegin << endl;
    ifs.close();
    system("pause");
    return 0;
}


