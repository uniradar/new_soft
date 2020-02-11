#ifndef STRINGCREATOR_H
#define STRINGCREATOR_H
#include <iostream>
#include <string>

using namespace std;

class stringCreator
{
public:

	//stringCreator();
	//~stringCreator();

	void Add(int number);
	void Add(char* str);
	void Add(string str);
	void clear();

	string createInt(int maxnum);
	string createString(int maxlen);

	string getString();



private:
	string String;
};
#endif