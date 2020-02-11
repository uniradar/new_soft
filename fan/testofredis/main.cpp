
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hiredis\hiredis.h"
#define NO_QFORKIMPL
#include "Win32_Interop\win32fixes.h"

#pragma comment(lib,"hiredis.lib")
#pragma comment(lib,"Win32_Interop.lib")

#include <ctime>

#include "stringCreator.h"

using namespace std;

string createDateID(int times, int base = 10000);

int main(int argc, char** argv) {
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	redisContext* c = redisConnectWithTimeout((char*)"127.0.0.1", 6379, timeout);
	if (c->err) {
		printf("Connection error: %s\n", c->errstr);
	}
	else
	{
		printf("Connection sucessfull\n");

		redisReply* replay;

		string setSQL;
		int times = 9999;

		double max = 0;
		double min = 0;
		double num = 0;
		double avg = 0;
		double idnum = 0;

		stringCreator crt;

		//	增加数据
		//-----------------------------------------------
		/*
		for (int i = 0; i < times; i++)
		{

			crt.Add("hmset date:");
			crt.Add(i);
			crt.Add(" DateID ");
			string DateID = createDateID(i);
			crt.Add(DateID);
			crt.Add(" CreatorName ");
			string CreatorName = crt.createString(20);
			crt.Add(CreatorName);
			crt.Add(" PID ");
			string PID = crt.createInt(99999);
			crt.Add(PID);
			crt.Add(" CreateTime ");
			string CreateTime = crt.createInt(12);
			CreateTime += "-";
			string tmp = crt.createInt(12);
			CreateTime += tmp;
			crt.Add(CreateTime);
			crt.Add(" LifeTime ");
			string LifeTime = crt.createInt(9999);
			crt.Add(LifeTime);
			string DateType = crt.createString(4);
			crt.Add(" DateType ");
			crt.Add(DateType);
			crt.Add(" DateFormat ");
			string DateFormat = crt.createString(20);
			crt.Add(DateFormat);
			crt.Add(" DateVer ");
			string DateVer = crt.createInt(99);
			crt.Add(DateVer);
			crt.Add(" DateBuff ");
			string DateBuff = crt.createString(256);
			crt.Add(DateBuff);

			setSQL = crt.getString();
			crt.clear();

			char *set = (char*)setSQL.data();


			//增加索引
			string indexSQL;
			stringCreator ind;
			ind.Add("zadd date.dateid.index ");
			ind.Add(i);
			ind.Add(" ");
			ind.Add(DateID);
			indexSQL = ind.getString();
			ind.clear();

			char *index = (char*)indexSQL.data();


			LARGE_INTEGER nFreq;
			LARGE_INTEGER t1;
			LARGE_INTEGER t2;
			double dt;

			QueryPerformanceFrequency(&nFreq);
			QueryPerformanceCounter(&t1);


			replay = (redisReply*)redisCommand(c, set);
			replay = (redisReply*)redisCommand(c, index);


			QueryPerformanceCounter(&t2);
			dt = (t2.QuadPart - t1.QuadPart) / (double)nFreq.QuadPart;
			//printf("Running time : %f ms\t", dt);
			num += dt;
			if (dt > max)
				max = dt;
			if (dt < min)
				min = dt;


			if (!replay)
			{
				cout << "error..." << endl;
			}
			else
			{
				cout << set << endl;
				cout << index << endl;
				setSQL.clear();
				indexSQL.clear();
			}

		}*/
		
		//-----------------------------------------------
		//查询数据
		string getSQL;
		for (int i = 0; i < 9999; i++)
		{
			string temp;
			//-----------------------------------------------
			//直接查询不通过索引
			crt.Add("hgetall date:");
			crt.Add(i);	
			getSQL = crt.getString();
			crt.clear();
			
			//-----------------------------------------------

			//-----------------------------------------------
			//通过索引查询键名再查询数据
			/*crt.Add("zscore date.dateid.index ");
			crt.Add(createDateID(i));
			temp = crt.getString();
			char *getid = (char*)temp.data();
			
			LARGE_INTEGER nFreq2;
			LARGE_INTEGER t12;
			LARGE_INTEGER t22;
			double dt2;

			QueryPerformanceFrequency(&nFreq2);
			QueryPerformanceCounter(&t12);

			replay = (redisReply*)redisCommand(c, getid);

			QueryPerformanceCounter(&t22);
			dt2 = (t22.QuadPart - t12.QuadPart) / (double)nFreq2.QuadPart;

			idnum += dt2;

			int k = atoi(replay->str);
			crt.clear();
			crt.Add("hgetall date:");
			crt.Add(k);
			getSQL = crt.getString();*/
			//-----------------------------------------------

			char *get = (char*)getSQL.data();


			LARGE_INTEGER nFreq;
			LARGE_INTEGER t1;
			LARGE_INTEGER t2;
			double dt;

			QueryPerformanceFrequency(&nFreq);
			QueryPerformanceCounter(&t1);


			replay = (redisReply*)redisCommand(c, get);


			QueryPerformanceCounter(&t2);
			dt = (t2.QuadPart - t1.QuadPart) / (double)nFreq.QuadPart;
			//cout << replay->str << endl;
			num += dt;
			if (dt > max)
				max = dt;
			if (dt < min)
				min = dt;


			if (!replay)
			{
				cout << "error..." << endl;
			}
			cout << get << endl;
			getSQL.clear();
			crt.clear();
		}

		cout << "max:" << max << endl;
		cout << "min:" << min << endl;
		cout << "avg:" << num / 9999 << endl;
		cout << "num:" << num << endl;

	}
	redisFree(c);
	system("pause");
	return 0;
}

string createDateID(int times, int base)
{
	string DateID;
	int temp = base;
	temp += times;
	DateID = to_string(temp);
	return DateID;
}
