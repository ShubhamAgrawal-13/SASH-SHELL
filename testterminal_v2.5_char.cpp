#include<iostream>
#include "headerfiles.h"
using namespace std;

void printPS1()
{
	char* user;
	user=getenv("USER");
	char host_name[100],cwd[100];
	gethostname(host_name,sizeof(host_name));
	getcwd(cwd,sizeof(cwd));
	//cout<<user<<"@"<<host_name<<cwd;
	
	if(strcmp(user,"root")==0)
	{
		cout<<user<<"@"<<host_name<<cwd;
	}
	else
	{
		cout<<user<<"@"<<host_name<<":"<<cwd<<"$ ";
	}
}

void getInput(char* input,int& c_input)
{
	c_input=0;
	while(1)
	{
			char ch=(char)fgetc(stdin);
			if(ch=='\n')
			{
				break;
			}
			input[c_input]=ch;
			c_input++;
	}
	input[c_input]='\0';
}



 
int main()
{
	while(1)
	{
		printPS1();	
		char input[100];
		int c_input=0;
		getInput(input,c_input);
		char* tokens[20];
		char* token=NULL;
		if(c_input!=0)
			token = strtok(input, " "); 
		int count_tokens=0;
		while (token != NULL) 
		{ 
			tokens[count_tokens++] =token;
			//cout<<token<<endl;
			//if()
			token= strtok(NULL, " "); 	
		} 
		cout<<count_tokens<<endl;
		tokens[count_tokens]=NULL;
		/*for(int i=0;i<count_tokens;i++)
		{
			printf("%s\n", tokens[i]); 
		}*/
		//cout<<"hello";		
		if(c_input==0)				//empty line
			continue;
		
		int fdh=open("history.txt",O_APPEND|O_RDWR);
		write(fdh, input, c_input);
		close(fdh);
		
		
		if(strcmp(tokens[0],"exit")==0)
		{
			break;
		}
		if(strcmp(tokens[0],"history")==0)
		{
			char buff[1024];
			int fd1=open("history.txt",O_APPEND|O_RDWR,0777);
			int n1=read(fd1, buff, 1024);
			while(n1 > 0)
			{
				if(write(1, buff, n1) != n1)
				{
					cout<<"Error while executing file !!!"<<endl;
					break;
				}
				n1=read(fd1, buff, 1024);		
			}
			close(fd1);
			continue;
		}
		if(strcmp(tokens[0],"cd")==0)
		{
			char temp[100];  
			if(count_tokens==1)
			{
				strcpy(temp,getenv("HOME"));
				chdir(temp);
			}
			else
			{
				if(strcmp(tokens[1],"~")==0)
				{
					strcpy(temp,getenv("HOME"));
					chdir(temp);	
				}
				else
				{
					strcpy(temp,tokens[1]);
					chdir(temp);
				}
			}
			continue;
		}
		
		
		char com[10];
		char* parameters[100];

		
		
		int flag_gt=0;
		char file1[100];
		if(count_tokens>=2)
		{
			if(strcmp(tokens[1],">")==0)
			{
				flag_gt=1;
				strcpy(file1,tokens[2]);
			}
			if(count_tokens>=3)
			{
				if(strcmp(tokens[2],">")==0)
				{
					flag_gt=1;
					strcpy(file1,tokens[3]);
				}
			}
		}

		strcpy(com,tokens[0]);
		//cout<<com<<endl;
		int c_par=0;
		for(int i=0;i<count_tokens-1;i++)
		{
				if(strcmp(tokens[i],">")==0  || strcmp(tokens[i],"|")==0 || strcmp(tokens[i],"<")==0 )
				{
					break;
				}
				strcpy(parameters[c_par],tokens[i]);
				//cout<<parameters[c_par]<<endl;
				c_par++;
		}
		/*for(int i=0;i<c_par;i++)
		{
			cout<<parameters[i]<<endl;
		}*/
		
		strcpy(parameters[c_par++],tokens[count_tokens-1]);
		parameters[c_par]=NULL;
		//cout<<c_par<<endl;
		
		
		int id = fork();
		if(id!=0)
			wait(NULL);
		else
		{
			if(flag_gt)
			{
				int fd,t;
				fd=open(file1,O_CREAT | O_APPEND |  O_RDWR,0700);
				if(fd<0)
				{
					cout<<"Error in opening the file \n";
				}
				t=dup2(fd,1);
				if(t<0)
				{
					cout<<"Error in redirecting \n";
				}		

				if(execvp(com,parameters)==-1)	
				{
					cout<<"command not found \n";
					exit(0);
				}
				close(fd);
			}
			else
			{
				if(execvp(com,parameters)==-1)	
				{
					cout<<"command not found \n";
					exit(0);
				}
			}
		}
	}
	return 0;
}
