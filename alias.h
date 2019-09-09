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
void initializeAlias()
{
	m.insert(make_pair("ll","ls -alF"));
	m.insert(make_pair("la","ls -A"));
	m.insert(make_pair("c","clear"));
	m.insert(make_pair("~","/home/shubham"));
	m.insert(make_pair("lss","ls --color=auto"));
	m.insert(make_pair("greph","grep --color=auto"));
}
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



/*
if(flagalias==1)
				{
						//cout<<"enter once again !!!";
						string temp13(arrgs[0]);
						char* ag[10];
						string si=getAlias(temp13);
						//cout<<si<<" ";
						int ac=0;
						char* array=(char*)si.c_str();
						char* tok = strtok(array," ");
						while(tok!=NULL)
						{
							ag[ac++]=tok;
							tok=strtok(NULL," ");
						}
						ag[ac]=NULL;
						
						//cout<<ag[0]<<" "<<ag[1]<<" "<<ac<<" "; 
						if(execvp(ag[0],ag)==-1)	
						{
							cout<<"command not found \n";
							exit(1);
						}		
				}
				else
				{	
					if(execvp(arrgs[0],arrgs)==-1)	
					{
						cout<<"command not found \n";
						exit(1);
					}
				}
*/
