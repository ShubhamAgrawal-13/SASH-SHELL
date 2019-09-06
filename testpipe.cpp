#include<iostream>
#include "headerfiles.h"
using namespace std;
int main()
{
	int fds[2];
	pipe(fds);
	if(fork()==0)
	{
		dup2(fds[1],1);
		char* ch[3];
		ch[0]="ls";
		ch[1]=NULL;
		execvp(ch[0],ch);
	}
	else
	{
		wait(NULL);
	}
	dup2(fds[0],0);
	close(fds[1]);
	char* ch[3];
	ch[0]="wc";
	ch[1]=NULL;
	execvp(ch[0],ch);
	close(fds[0]);
	return 0;
}
