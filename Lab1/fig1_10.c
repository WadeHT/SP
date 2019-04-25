#include "apue.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void	sig_int(int);		/* our signal-catching function */
int main(void)
{
		char	buf[MAXLINE];	/* from apue.h */
		pid_t	pid;
		int		status;
		
		char cmd[10],dir[100],dst[100], workspace[100],bin[100];
		int i=0;
		
		strcpy(workspace,getcwd(NULL, NULL));

		if (signal(SIGINT, sig_int) == SIG_ERR)
				err_sys("signal error");

		printf("## ");	/* print prompt (printf requires %% to print %) */
		while (fgets(buf, MAXLINE, stdin) != NULL) 
		{
			sscanf(buf,"%s",cmd);
			if(strcmp(cmd,"cd") == 0)
			{
				sscanf(buf,"%s%s",cmd,dir);
				strcat(workspace,"/");
				strcat(workspace,dir);
				if(strcmp(dir,"..") == 0)chdir("OLDPWD");
				else chdir(dir);					
			}
			
			if (buf[strlen(buf) - 1] == '\n')buf[strlen(buf) - 1] = 0; /* replace newline with null */
			if ((pid = fork()) < 0)err_sys("fork error");
			else if (pid == 0)
			{		/* child */
				if(strcmp(cmd,"ls") == 0)
				{
					execlp(cmd, cmd,workspace,(char *)0);
					err_ret("couldn't execute: %s", buf);
				}
				else if(strcmp(cmd,"mkdir") == 0)
				{
					sscanf(buf,"%s%s",cmd,dir);
					strcat(workspace,"/");
					execlp(cmd, cmd,strcat(workspace,dir), (char *)0);
					err_ret("couldn't execute: %s", buf);
				}
				else if(strcmp(cmd,"cp") == 0)
				{
					sscanf(buf,"%s%s%s",cmd,dir,dst);
					strcat(workspace,"/");
					char temp[100];
					strcpy(temp,workspace);
					execlp(cmd, cmd, strcat(workspace,dir), strcat(temp,dst), (char *)0);		
					err_ret("couldn't execute: %s", buf);						
				}
				else if(strcmp(cmd,"cd") == 0);
				else printf("System error: invalid command\n");		
				exit(127);	
			}	
				/* parent */
				if ((pid = waitpid(pid, &status, 0)) < 0)err_sys("waitpid error");
				printf("## ");
		}	
		exit(0);
}

		void
sig_int(int signo)
{
		printf("interrupt\n## ");
}

