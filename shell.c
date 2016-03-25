#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define f(i,a,n) for(i=a;i<n;i++)
#define set(a) memset(a,0,sizeof(a))
char prompt[100],home[70],user[10],host[10],cmd[100];
char* getpath(char* a, char* b)
{
	int i;
	if(b[0]=='/')
	strcpy(a,b)	;
	else if(b[0]=='~' || b[0]=='\0')
	{	
		int l=strlen(home),m=strlen(b);
		f(i,0,l)
			a[i]=home[i];
		a[l]='/';
		f(i,0,m-2)
			a[i+l+1]=b[i+2];
	}
	else
	{	
		int l=strlen(prompt),m=strlen(b);
		f(i,0,l)
			a[i]=prompt[i];
			a[l]='/';
		if(b[0]=='.' && b[1]=='/')
			f(i,0,m-2)
				a[i+l+1]=b[i+2];	
		else
			f(i,0,m)
			a[i+l+1]=b[i];
	}
	return a;
}
int main()
{	
	getcwd(home,70);						// set home dir
	int hl=strlen(home);
	int i, fl=0,r=2;
	while(r)
	{	
		int i=0,j=0,fl=0,noc=i;
		char* arg;
		char str[1000]; char* space[100];			//parameters for execv
		char comm[100][100];					//store multiple commands
		set(prompt);
		getcwd(prompt,100);
		getlogin_r(user,10);					//prompt parameters
		gethostname(host,10);
		fl=0;
		f(i,0,hl)
			if(prompt[i]!=home[i])				//check if current dir is a subdir of home
				fl=1;
		printf("%s@%s:",user,host);
		if(fl)
			printf("%s$",prompt);
		else 
			printf("~/%s$",prompt+hl+1);
		
		memset(cmd,0,sizeof(cmd));	
		scanf(" %[^\n]s",cmd);
		memset(comm,0,sizeof(comm));	
		i=0;
		while(cmd[j]!='\0')
		{	
			int k=0;
			while(cmd[j]!=';' && cmd[j]!='\0')
				comm[i][k++]=cmd[j++];
			i++;
			while(cmd[j]==';' || cmd[j]==' ' || cmd[j]=='\t')
				j++;
		}
		noc=i;
		i=-1;
		while(++i<noc)
		{
			getcwd(prompt,100);					//update current directory, for next commands
			if(strncmp(arg,"cd",2)==0)
			{	
				memset(str,0,sizeof(str));
				if(chdir(str))
					perror("Could not change directory\n");
			}
			else if(strncmp(arg,"pwd",3)==0)
				fprintf(stdout,"%s\n",prompt);
			else if(strncmp(arg,"echo",4)==0)
				printf("%s\n",strtok(NULL,"\n"));
//			else if(strncmp(arg,"pinfo",5)==0);

			else 
			{	
				int i=1;space[0]=arg;
				do
				{
					space[i]=strtok(NULL," ");
				}while(space[i++]!=NULL);
			
				int pid=fork();
				if(pid<0)
					perror("Child process not created!");
				else if(pid==0)
				{	if(execvp(arg,space))
					exit(0);
				}
				wait();
			}
		}	
	}
}
