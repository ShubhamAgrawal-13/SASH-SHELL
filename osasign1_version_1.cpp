#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string>
#include<fcntl.h>

using namespace std;


void split_input(char* input)
{
	
	
}

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
 
int main()
{
	int fd=999;
	while(1)
	{
		//cout<<"shubham > ";
		
		printPS1();	
		char input[100];
		string token_words[100];
		int c_input=0;
		int c_token=0;
		char com[10];
		char* parameters[100];
		int t;
		while(1)
		{
			char ch=(char)fgetc(stdin);
			input[c_input]=ch;
			c_input++;
			if(ch=='\n')
			{
				break;
			}
		}
		input[c_input]='\0';
		//cout<<input;
		if(c_input==1)				//empty line
			continue;

		string s="";
		for(int i=0;;i++)
		{
			if(input[i]==' ' || input[i]=='\n')
			{
				token_words[c_token]=s;
				c_token++;
				//cout<<s<<"";	
				s="";
				
				if(input[i]=='\n')
				{
					break;
				}
				continue;
			}
			s=s+input[i];
		}
		/*for(int i=0;i<c_token;i++)
		{
			cout<<token_words[i].c_str()<<endl;
		}*/
		if(token_words[0]=="exit")
		{
			break;
		}
		if(token_words[0]=="cd")
		{
			char temp[100];  
			if(c_token==1)
			{
				strcpy(temp,getenv("HOME"));
				chdir(temp);
			}
			else
			{
				if(token_words[1] == "~")
				{
					strcpy(temp,getenv("HOME"));
					chdir(temp);	
				}
				else
				{
					strcpy(temp,token_words[1].c_str());
					chdir(temp);
				}
			}
			continue;
		}

		if(token_words[1]!="" and token_words[2]!="")
		{
			if(token_words[1]==">")
			{
				char file1[100];
				token_words[1]="";
				strcpy(file1,token_words[2].c_str());
				fd=open(file1,O_CREAT | O_APPEND | O_WRONLY | O_RDONLY);
				if(fd<0)
				{
					cout<<"Error in opening the file \n";
				}
				t=dup2(fd,1);
				if(t<0)
				{
					cout<<"Error in redirecting \n";
				}		
			}
			if(token_words[2]==">")
			{
				char file1[100];
				token_words[2]=="";
				strcpy(file1,token_words[3].c_str());
				fd=open(file1,O_CREAT | O_APPEND | O_WRONLY | O_RDONLY);
				if(fd<0)
				{
					cout<<"Error in opening the file \n";
				}
				t=dup2(fd,1);
				if(t<0)
				{
					cout<<"Error in redirecting \n";
				}		
			}
		}


		//cout<<c_token<<endl;
		//token_words[0]="/bin/"+token_words[0];
		strcpy(com,token_words[0].c_str());
		//cout<<com<<endl;
		int c_par=0;
		for(int i=0;i<c_token;i++)
		{
			//char temp[100];  
			//strcpy(temp,);
			if(token_words[i]=="")
			{
				parameters[c_par]=NULL;
			}
			else
			{
				parameters[c_par]=(char*)token_words[i].c_str();
			}
			//cout<<parameters[c_par]<<endl;
			c_par++;
		}
		/*for(int i=0;i<c_par;i++)
		{
			cout<<parameters[i]<<endl;
		}*/
		parameters[c_par]=NULL;
		//cout<<c_par<<endl;
		int id = fork();
		if(id!=0)
			wait(NULL);
		else
		{
			if(execvp(com,parameters)==-1)	
			{
				cout<<"command not found \n";
				exit(0);
			}
		}
		close(fd);
	}
	return 0;
}
