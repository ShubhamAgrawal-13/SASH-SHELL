#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<pwd.h>
#include<sys/wait.h>
#include<string>
#include<fcntl.h>
#include<signal.h>
#include<ctime>

using namespace std;


void initShell()
{
	int uid;
	char* user,*home,*shell;
	uid=getuid();
	struct passwd * passwd1;
	passwd1 = getpwuid(uid);
	if(passwd1 == NULL)
	{
			cout<<"Error: !!!! please login first\n";
	}
	else
	{
		user=passwd1->pw_name;
		home=passwd1->pw_dir;
		shell=passwd1->pw_shell;
	}
	char host_name[100],cwd[100],ps1[100];
	gethostname(host_name,sizeof(host_name));
	getcwd(cwd,sizeof(cwd));
	sprintf(ps1,"%s@%s:%s$ ",user,host_name,cwd);
	cout<<ps1<<endl;


	FILE *fp;
	fp=fopen("shubhshellrc","r");
	
	char line[1000];
	int linenum=0;
	while(fgets(line, 1000, fp) != NULL)
	{
	        char key[100], value[500];
	        if(line[0] == '/' || line[0]=='\n') continue;
	        linenum++;
	        char* token = strtok(line,"=");
	        strcpy(key,token);
	        token = strtok(NULL,"=");
	        strcpy(value,token);
	        printf("Line %d:   Key = %s   Value = %s\n", linenum, key, value);
	}
	fclose(fp);
}

int main()
{
	time_t now = time(0);
	char* time=ctime(&now);
	cout<<time<<endl;
	initShell();
	return 0;
}