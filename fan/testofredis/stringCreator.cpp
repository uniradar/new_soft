#include"stringCreator.h"
#include <stdlib.h>

/*stringCreator::stringCreator()
{
	String.clear();
}*/


void stringCreator::Add(int number)
{
	string temp = to_string(number);
	String += temp;
}

void stringCreator::Add(char* str)
{
	string temp = str;
	String += temp;
}
void stringCreator::Add(string str)
{
	String += str;
}
string stringCreator::getString()
{
	return String;
}

void stringCreator::clear()
{
	String.clear();
}

string stringCreator::createInt(int maxnum)
{
	int k = rand() % maxnum + 1;
	string temp = to_string(k);
	return temp;
}
string stringCreator::createString(int maxlen)
{
	string temp;
	char zm[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N' };
	int max = rand() % maxlen + 1;
	for (int i = 1; i < max; i++)
	{
		int k = rand() % 14 + 1;
		temp += zm[k];
	}
	return temp;
}