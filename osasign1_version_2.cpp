#include<iostream>
#include "headerfiles.h"
#include "history.h"
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
			input[c_input]=ch;
			c_input++;
			if(ch=='\n')
			{
				break;
			}
	}
	input[c_input]='\0';
}

void history()
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
}

 
int main()
{
	while(1)
	{
		printPS1();	
		char input[100];
		string token_words[100];
		int c_input=0;
		int c_token=0;
		char com[10];
		char* parameters[100];
		getInput(input,c_input);
		//cout<<"hello1\n";
		if(c_input==1)				//empty line
			continue;
		
		int fdh=open("history.txt",O_APPEND|O_RDWR,0777);
		write(fdh, input, c_input);
		close(fdh);
		
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
		if(token_words[0]=="history")
		{
			history();
			continue;
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
		
		
		int flag_gt=0;
		char file1[100];
		if(token_words[1]!="" and token_words[2]!="")
		{
			if(token_words[1]==">")
			{
				flag_gt=1;
				token_words[1]="";
				strcpy(file1,token_words[2].c_str());
			}
			if(token_words[2]==">")
			{
				flag_gt=1;
				token_words[2]=="";
				strcpy(file1,token_words[3].c_str());
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
		int a=0;
		char* arrgs[10];
		int fds[2];
		int sss=dup(0);
		for(int i=0;i<c_par;i++)
		{
			pipe(fds);
			//cout<<parameters[i]<<endl;
			if(strcmp(parameters[i],"|")==0 || strcmp(parameters[i],">")==0)
			{
				arrgs[a]=NULL;
				if(fork()==0)
				{
					dup2(fds[1],1);
					execvp(arrgs[0],arrgs);
				}
				else
				{
					wait(0);
					dup2(fds[0],0);
					close(fds[1]);
					a=0;
					arrgs[a]=NULL;
					continue;
				}
			}
			else
				arrgs[a++]=parameters[i];
		}
		arrgs[a]=NULL;
		if(fork()==0)
		{
			//dup2(fds[1],1);
			execvp(arrgs[0],arrgs);
			
		}
		else
		{
			wait(0);
		}
		close(fds[0]);
		close(fds[1]);
		dup2(sss,0);
		//cout<<"hello\n";
		//execvp(arrgs[0],arrgs);
		/*int id = fork();
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
		}*/
	}
	return 0;
}
