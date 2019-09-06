#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string>
#include<fcntl.h>
#include "history.h"
#include "alias.h"
using namespace std;

void printPS1()
{
	char* user;
	user=getlogin();
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



void savefile(char* filename)
{
}

 
int main()
{
	while(1)
	{
		printPS1();	
		char input[100];
		string token_words[100];
		string input_string;
		int c_input=0;
		int c_token=0;
		char com[10];
		char* parameters[100];
		getInput(input,c_input);
		//cout<<"hello1\n";
		if(c_input==1)				//empty line
			continue;
		
		updatehistoryfile(input,c_input);
		string s1(input);
		addhistory(s1);
		string s="";
		for(int i=0;;i++)
		{
			if(input[i]==' ' || input[i]=='\n')
			{
				if(input[i]==' ' && s=="")
					continue;
				token_words[c_token]=s;
				
				if(c_token>0)
					input_string+=s+" ";
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
			printhistory();
			//printhistoryfile();
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
		int flagalias=0;
		if(token_words[0]=="alias")
		{
			if(token_words[1]=="")
			{
				printAlias();
			}
			else
			{
				//cout<<input_string<<endl;
				addAlias(input_string);
			}
			continue;
		}
		if(checkAlias(token_words[0]) && token_words[0]!="alias" )
		{
			flagalias=1;
			//cout<<"enter!!!\n";
		}
		//int flag_gt=0;
		//char file1[100];
		/*if(token_words[1]!="" and token_words[2]!="")
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
		}*/

		//cout<<c_token<<endl;
		//token_words[0]="/bin/"+token_words[0];
		strcpy(com,token_words[0].c_str());
		//cout<<com<<endl;
		int c_par=0;
		for(int i=0;i<c_token;i++)
		{
			//char temp[100];  
			//strcpy(temp,);
			//if(token_words[i]=="")
			//{
			//	parameters[c_par]=NULL;
			//}
			//else
			//{
				parameters[c_par]=(char*)token_words[i].c_str();
			//}
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
		int sss1=dup(0);
		int sss2=dup(1);
		int flag1=0;
		int flag2=0;
		
		for(int i=0;i<c_par;i++)
		{
			//cout<<parameters[i]<<endl;
			pipe(fds);
			if(strcmp(parameters[i],"|")==0 || strcmp(parameters[i],">")==0 || strcmp(parameters[i],">>")==0)
			{
				arrgs[a]=NULL;
				if(fork()==0)
				{
					//cout<<arrgs[0]<<endl;
					dup2(fds[1],1);
					if(flagalias==1)
					{
						//cout<<"enter once again !!!";
						string temp13(arrgs[0]);
						char* ag[10];
						string si=getAlias(temp13);
						//cout<<ag[0]<<" ";
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
						//flagalias=0;
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
				}
				else
				{
					wait(0);
					dup2(fds[0],0);
					close(fds[1]);
					a=0;
					arrgs[a]=NULL;
					if(strcmp(parameters[i],">")==0) 
					{	
						flag1=1;
						i++;
						break;
					}
					if(strcmp(parameters[i],">>")==0)
					{
						flag2=1;
						i++;
						break;		
					}
					continue;
				}
			}
			else
				arrgs[a++]=parameters[i];
		}
		arrgs[a]=NULL;
		if(flag1==1 || flag2==1)
		{
			cout<<parameters[c_par-1]<<endl;
			char buff[1024];
			int fd1;
			if(flag1==1)
				fd1=open(parameters[c_par-1],O_CREAT |O_TRUNC| O_WRONLY,0777);
			else
				fd1=open(parameters[c_par-1],O_CREAT | O_APPEND | O_RDWR,0777);
			
			fsync(0);
			int n1=read(0, buff, 1024);
			while(n1 > 0)
			{
				if(write(fd1, buff, n1) != n1)
				{
					cout<<"Error while writing file !!!"<<endl;
					break;
				}
				n1=read(0, buff, 1024);		
			}
			
			close(fd1);
		}
		else
		{
			
			if(fork()==0)
			{
				//dup2(fds[1],1);
				if(flagalias==1)
				{
						cout<<"enter once again !!!";
						string temp13(arrgs[0]);
						char* ag[10];
						string si=getAlias(temp13);
						cout<<si<<" ";
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
				
			}
			else
			{
				wait(0);
			}
		}

		dup2(sss1,0);
		close(fds[0]);
		close(fds[1]);
		
		dup2(sss2,1);
		//close(sss1);
		//close(sss2);
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
