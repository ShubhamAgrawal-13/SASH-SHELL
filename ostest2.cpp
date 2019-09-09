#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<ctime>
#include<sys/wait.h>
#include<string>
#include<fcntl.h>
#include<pwd.h>
#include<signal.h>
#include"history.h"
#include"alias.h"
#include<map>
using namespace std;



map<string,string> menv;
char* user,*home,*shell,*path;
int childId;
int uid;
char host_name[100],cwd[100],ps1[100];
char fnn[100]="bigfile.txt"; //file in which script will be saved.
//char** environ;
void printPS1()
{
	getcwd(cwd,sizeof(cwd));
	sprintf(ps1,"%s@%s:%s$ ",user,host_name,cwd);
	cout<<ps1;
}
void initShell()
{
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
	gethostname(host_name,sizeof(host_name));
	getcwd(cwd,sizeof(cwd));
	sprintf(ps1,"%s@%s:%s$ ",user,host_name,cwd);
	path=getenv("PATH");
	string name=home;
	menv.insert(make_pair("HOME",name));
	name=path;
	menv.insert(make_pair("PATH",name));
	name=shell;
	menv.insert(make_pair("SHELL",name));
	name=cwd;
	menv.insert(make_pair("PWD",name));
	name=user;
	menv.insert(make_pair("USER",name));
	name=host_name;
	menv.insert(make_pair("HOSTNAME",name));
	name=ps1;
	menv.insert(make_pair("PS1",name));
	
	// menv.insert(make_pair("UID",to_string(uid)));

	// menv.insert(make_pair("HISTSIZE",to_string(30)));

	 // char* tempenv[100];
	 // int cenv=0;
	 // for(auto i=menv.begin();i!=menv.end();i++)
	 // {
	 // 	name=i->first+"="+i->second;
		// strcpy(tempenv[cenv++],name.c_str());
	 // }
	// memcpy(environ,tempenv,sizeof(tempenv));

	// while (*environ) 
	// {
 //        cout<<*environ<<endl;
 //         *environ++;
 //    }



	 //printPS1
	//cout<<user<<"@"<<host_name<<cwd;
	
	/*if(strcmp(user,"root")==0)
	{
		cout<<user<<"@"<<host_name<<cwd;
	}
	else
	{
		cout<<user<<"@"<<host_name<<":"<<cwd<<"$ ";
	}*/
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

bool  isFile(string name) 
{
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int match_extension(string s)
{
	int l= s.length();
	if(!isFile(s))
	{
		return -1;
	}
	//cout<<s<<endl;
	string temp ="";
	for(int i=l-1;s[i]!='.';i--)
		temp=s[i]+temp;
	temp='.'+temp;
	//temp+= s[l-4]+s[l-3]+s[l-2]+s[l-1];
	cout<<temp<<endl;
	if(temp == ".mp3")
		return 1;
	if(temp == ".mp4")
		return 1;
	if(temp == ".png" || temp == ".jpg")
		return 2;
	if(temp == ".pdf")
		return 3;
	return 0;
}

// void savefile(char* filename)
// {
// }

 
int main()
{
	//cout<<"history";
	//close(1);
	initShell();
	initializeAlias();
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
		//cout<<"hello";
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
		
		//cout<<c_token<<endl;
		if(token_words[0]=="exit")
		{
			break;
		}
		if(checkAlias(token_words[0]))
		{
			//cout<<"EEEE";
			if(getAlias(token_words[0])=="exit ")
				break;
			if(getAlias(token_words[0])=="history ")
			{
				printhistory();
				continue;
			}
			if(getAlias(token_words[0])=="cd ")
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
						if(chdir(temp)==-1)
						{
							cout<<"Error : Not a valid Directory\n";
						}
					}
				}
				continue;
			}
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
					if(chdir(temp)==-1)
					{
							cout<<"Error : Not a valid Directory\n";
					}
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
		if(checkAlias(token_words[0]))
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
		int openflag=0;
		if(token_words[0]=="open")
		{
			int ex1=match_extension(token_words[1]);
			if(ex1==0)
			{
				strcpy(parameters[0],"gedit"); // text file
			}
			if(ex1==1)
			{
				strcpy(parameters[0],"vlc");
			}
			if(ex1==2)
			{
				strcpy(parameters[0],"eog");
			}
			if(ex1==3)
			{
				strcpy(parameters[0],"evince");
			}
			if(ex1==-1)
			{
				cout<<"Could not open the file \n";
				continue;
			}
			openflag=1;
			//cout<<ex1<<" "<<parameters[0]<<endl;
		}

		if(token_words[0]=="record")
		{
			cout<<"Command not found... try record start or record stop\n";
			continue;
		}
		if(token_words[0]=="record" && token_words[1]=="start")
		{
			// childId=fork();
			// if(childId==0)
			// {	
			// 	string sc ="script";
			// 	parameters[0]=(char*)sc.c_str();
			// 	strcpy(parameters[1],fnn);
			// 	parameters[2]=NULL;
			// 	execvp(parameters[0],parameters);
			// 	cout<<"child";
			// 	exit(1);
			// }
		    continue;
		}
		if(token_words[0]=="record" && token_words[1]=="stop")
		{
			//kill(childId,SIGQUIT);
			continue;
		}
		
		int a=0;
		char* arrgs[20];	
		int fds[2];
		int sss1=dup(0);
		int sss2=dup(1);
		int flag1=0;
		int flag2=0;
		char* array;
		string si;
		//cout<<c_par<<endl;
		for(int i=0;i<c_par;i++)
		{
			//cout<<parameters[i]<<endl;
			pipe(fds);
			if(strcmp(parameters[i],"|")==0 || strcmp(parameters[i],">")==0 || strcmp(parameters[i],">>")==0)
			{
				if(fork()==0)
				{
					//cout<<arrgs[a-1]<<endl;
					dup2(fds[1],1);
					arrgs[a]=NULL;
					if(execvp(arrgs[0],arrgs)==-1)	
					{
						cout<<"command not found \n";
						exit(1);
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
			{
				if(checkAlias(token_words[i]))
				{
					si=getAlias(token_words[i]);
					//cout<<si<<" ";
					array=(char*)si.c_str();
					char* tok = strtok(array," ");
					while(tok!=NULL)
					{
						arrgs[a++]=tok;
						//strcpy(arrgs[a],tok);
						//cout<<tok<<endl;	
						tok=strtok(NULL," ");
					}
					//cout<<arrgs[0]<<endl;
					//cout<<"enter!!!\n";
				}
				else
				{
					arrgs[a++]=parameters[i];
				}
				//cout<<arrgs[0]<<endl;
			}
		}
		//cout<<a<<endl;
		//cout<<arrgs[0]<<endl;
		arrgs[a]=NULL;
		if(flag1==1 || flag2==1)
		{
			//cout<<parameters[c_par-1]<<endl;
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
			int pid=fork();	
			if(pid==0)
			{
				//dup2(fds[1],1);
				//cout<<arrgs[0]<<endl;
				if(execvp(arrgs[0],arrgs)==-1)	
				{
					cout<<"command not found 1 \n";
					exit(1);
				}
			}
			else
			{
				if(openflag!=1)
				{
				        waitpid(pid,0,0);
				}
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
