#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<ctime>
#include "AddSql.hpp"
#include<vector>
#include<io.h>
#include "OperateSqls.hpp"

using namespace std;

void getFiles(string path, vector<string>& files);
void getFilesName(string path, vector<string>& files);
char* str2char(string src);

int main()
{
	char path1[300] = "C:\\Test";
	string log = "C:/download/log.txt";
	vector<string> filenames1;
	getFiles(path1, filenames1);
	vector<string> filenames2;
	getFilesName(path1, filenames2);
	
	int nums = filenames2.size();
	for (int i = 0; i < nums; i++)
	{
		OperateSql ops;

		string b1 = filenames1[i].c_str();
		string b2 = filenames2[i].c_str();
		char* fileWithtail = str2char(b1);
		char* testChar = str2char(b2);
		char noSuffixName[100];
		int	indexPoint = b2.find_first_of('.', 0);

		for (int j = 0; j < indexPoint; j++) {
			noSuffixName[j] = testChar[j];
		}
		noSuffixName[indexPoint] = '\0';
		ops.startInsert(fileWithtail, noSuffixName,log);
	}
    return 0;
}

void getFiles(string path, vector<string>& files)
{
	long   hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void getFilesName(string path, vector<string>& files)
{
	long   hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					//getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
					getFiles(fileinfo.name, files);
			}
			else
			{
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
				files.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

char* str2char(string src)
{
	char* dst = new char[255];
	int i;
	for (i = 0; i <= src.length(); i++)
		dst[i] = src[i];
	dst[i] = '\0';
	return dst;
}


/*ifstream ifs("J:/Forex/AUDCHF.csv");
	long timeBegin = 0;
	long timeEnd = 0;

	char bf[300] = { 0 };
	if (ifs.is_open())
	{
		timeBegin = clock();
		if (ifs.good())
			ifs.getline(bf, 200);
		int i = 0;
		stringstream ss;
		string tsp;
		ss << "INSERT INTO AUDCHF(Timestamp,Bid,Ask)VALUES\n";
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
				i = 0;
				ss.str("");
				ss << "INSERT INTO AUDCHF(Timestamp,Bid,Ask)VALUES\n";
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
	}
	cout << "Total time is:" << timeEnd - timeBegin << endl;
	ifs.close();
	system("pause");*/