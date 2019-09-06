#include<iostream>
#include "headerfiles.h"
using namespace std;

void getInput(char* input,int& c_input)
{
	c_input=0;
	//string s="";
	while(1)
	{
			char ch=(char)fgetc(stdin);
			/*if(ch==' ' || ch=='\n' || ch=='>' || ch=='<' || ch=='|')
			{
				tokens[];
				s="";
			}*/
			input[c_input]=ch;
			c_input++;
			if(ch=='\n')
			{
				break;
			}
	}
	input[c_input]='\0';
}


int main()
{
	char input[1024];
	int c_input=0;
	getInput(input,c_input);
	//cout<<input<<endl;
	char* tokens[20];
	char* token = strtok(input, " "); 
	int count_tokens=0;
	while (token != NULL) 
	{ 
		tokens[count_tokens++] =token;
		token= strtok(NULL, " "); 
	} 
	tokens[count_tokens]=NULL;
	for(int i=0;i<count_tokens;i++)
	{
		printf("%s\n", tokens[i]); 
	}
	return 0;
}
