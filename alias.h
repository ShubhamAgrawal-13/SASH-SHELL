#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string>
#include<fcntl.h>
#include<iostream>
#include<map>

using namespace std;

map<string,string> m;

void addAlias(string s)
{
	string s1,s2;
	int i=0;
	for(i=0;i<s.length();i++)
	{
		if(s[i]=='=')
		{
			break;
		}
		s1+=s[i];
	}
	i++;
	for(;i<s.length();i++)
	{
		if(s[i]=='\"' || s[i]=='\'')
		{
			continue;
		}
		s2+=s[i];
	}
	//cout<<s2<<endl;
	if(m.find(s1)==m.end())
	{
		m.insert(make_pair(s1,s2));
	}
	else
	{
		m[s1]=s2;
	}
}

int checkAlias(string s1)
{
	if(m.find(s1)==m.end())
	{
		return 0;
	}
	return 1;
}

void printAlias()
{
	for(auto i=m.begin();i!=m.end();i++)
	{
		cout<<"alias "<<i->first<<"="<<i->second<<endl;
	}
}

string getAlias(string s1)
{
	return m[s1];
}


