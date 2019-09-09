#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string>
#include<fcntl.h>
#include<iostream>
#include<vector>
using namespace std;

vector<string> q;
int HISTSIZE=20;//default
int maxsize = HISTSIZE;

void setHistSize(int size)
{
	HISTSIZE=size;
}
void addhistory(string filename)
{
	if(q.size()<maxsize)
		q.push_back(filename);
	else
	{
		q.erase(q.begin()+0);
		q.push_back(filename);
	}	
}

void printhistory()
{
	int count=1;
	for(auto i=q.begin();i!=q.end();i++)
	{
		cout<<count<<" "<<*i<<endl;
		count++;
	}
}


void updatehistoryfile(char* input,int c_input)
{
	int fdh=open("history.txt",O_APPEND|O_RDWR,0777);
	write(fdh, input, c_input);
	close(fdh);
}

void printhistoryfile()
{
	char buff[1024];
	int fd1=open("history.txt",O_APPEND|O_RDWR,0777);
	int n1=read(fd1, buff, 1024);
	while(n1 > 0)
	{
		if(write(1, buff, n1) != n1)
		{
			cout<<"Error while writing file !!!"<<endl;
			break;
		}
		n1=read(fd1, buff, 1024);		
	}
	close(fd1);
}
