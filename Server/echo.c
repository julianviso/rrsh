
/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
#define MAX_USR_PASS 42
#define MAXARGS   128
#include <errno.h>
extern char **environ;
int parseline(char *buf, char **argv);
int authUser(char * username, char * password);
void echo(int connfd,unsigned short client_port,char * haddrp) 
{
    size_t n;
    char username[MAXLINE];
    char password[MAXLINE];
    char inputString[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio, connfd);
    char *argv[128]; /* Argument list execve() */
    FILE *fp1;
    char oneword[100];
    char c;
	Rio_readlineb(&rio, username, MAXLINE);
	Rio_writen(connfd,"Yes\n",4);
	Rio_readlineb(&rio, password, MAXLINE);
	printf("User %s logging in from %s at TCP port %u.\n", username, haddrp, client_port);
	int auth = authUser(username,password);
	if (auth == 0 )
	{
		printf("user %s denied access\n",username);
		Rio_writen(connfd,"Login Failed\n",13);
	}
	else
	{
		printf("User %s successfully logged in.\n", username);
		Rio_writen(connfd,"Login Approved\n",15);
	}







    while((n = Rio_readlineb(&rio, inputString, MAXLINE)) != 0) { //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);
int allowed = 0;
parseline(inputString, argv);
fp1 = fopen("rrshcommands.txt","r");
if (argv[0] != NULL) {
	printf("User %s sent the command '%s' to be executed.\n", username, inputString);
   do {
      c = fscanf(fp1,"%s",oneword); /* got one word from the file */
        if (strcmp(oneword,argv[0])==0)

        {
	     allowed = 1;
		
        }
   } while (c != EOF);
	}
	fclose(fp1);
        if (allowed == 0)
        {
		printf("The command '%s' is not allowed.\n", inputString);
                    printf("Cannot execute ’%s’ on this server\n", inputString);
		    char error[MAXLINE];
              sprintf(error,"Cannot execute 'program' on this server\n");
                Rio_writen(connfd,error,strlen(error));
        }
	else
	{
		int status;
                pid_t pid;
  if((pid = fork()) == 0) 
	{
	printf("Forking/Execing the command ’%s’ on behalf of %s.\n", inputString, username);	
    dup2 (connfd, 1);
    Close(connfd);
	 if (execve(argv[0], argv, environ) < 0) {
		exit(0);
	    }  
  } else {
  }
	waitpid(pid, &status, 0);
            Rio_writen(connfd, "RRSH COMMAND COMPLETED\n", strlen("RRSH COMMAND COMPLETED\n"));
	
}

	
}
printf("User %s disconnected\n",username);
}
/* $end echo */

int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
	       buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */
int authUser(char * username, char * password)
{
 FILE *fp1;
    char oneword[100];
    char c;
 fp1 = fopen("rrshusers.txt","r");
        int auth = 0;
	username[strlen(username)-1] = '\0';
		
	password[strlen(password)-1] = '\0';
   do {
      c = fscanf(fp1,"%s",oneword);
        if (strcmp(oneword,username)==0)
        {
                c = fscanf(fp1,"%s",oneword);
                if ((strcmp(oneword,password)==0) && (c!=EOF))
                {
                   auth = 1;
		   return auth;
                }
                else
              {
		auth = 0;
		return auth;
                }
        }
   } while (c != EOF);
           /* repeat until EOF           */
    fclose(fp1);


return auth;
} 
