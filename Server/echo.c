/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
#define MAX_USR_PASS 42
#define MAXARGS   128
extern char **environ;
int parseline(char *buf, char **argv);
void echo(int connfd) 
{
    size_t n;
    char buf[MAXLINE];
    char buf2[MAXLINE];
    char buf3[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio, connfd);
    char *argv[128]; /* Argument list execve() */
    FILE *fp1;
    char oneword[100];
    char c;
	Rio_readlineb(&rio, buf, MAXLINE);
	Rio_writen(connfd,"Yes\n",4);
	Rio_readlineb(&rio, buf2, MAXLINE);
        fp1 = fopen("rrshusers.txt","r");
	int auth = 0;
   do {
      c = fscanf(fp1,"%s",oneword); /* got one word from the file */
	if (strcmp(oneword,strtok(buf,"\n"))==0)
	{	
		c = fscanf(fp1,"%s",oneword);
		if (strcmp(oneword,strtok(buf2,"\n"))==0)
		{
		   Rio_writen(connfd,"Login Approved\n",15);
		   auth = 1;
		}
		else
		{	
		  Rio_writen(connfd,"Login Failed\n",13);
		}
	}
   } while (c != EOF);   
           /* repeat until EOF           */
	if (auth == 0)
	{
		Rio_writen(connfd,"Login Failed\n",13);	
	}
    fclose(fp1);







    while((n = Rio_readlineb(&rio, buf3, MAXLINE)) != 0) { //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);
	// int bg = parseline(buf3, argv);
	 if (strcmp("/bin/ls -l\n",buf3) ==0 )
	 {
	   int bg = parseline(buf3, argv);
	    fp1 = fopen("rrshcommands.txt","r");
        int allowed = 0;
   do {
      c = fscanf(fp1,"%s",oneword); /* got one word from the file */
//      printf("%s\n",oneword);       /* display it on the monitor  */
        if (strcmp(oneword,argv[0])==0)
        {
	     allowed = 1;
		
        }

   } while (c != EOF);
           /* repeat until EOF           */
        printf("%d\n",allowed);
	fclose(fp1);
        if (allowed == 0)
        {
                Rio_writen(connfd,"Cannot execute program on this server\n",38);
        }
	else
	{
		int status;
                pid_t pid;
//		waitpid(pid, &status, 0);
  //          Rio_writen(connfd, "RRSH COMMAND COMPLETED\n", strlen("RRSH COMMAND COMPLETED\n"));
  if((pid = fork()) == 0) {
    dup2 (connfd, 1);
    Close(connfd);
//  char *argv[] = { "/bin/cat", "abc.c" , 0 };
	 if (execve(argv[0], argv, environ) < 0) {
		printf("%s: Command not found.\n", argv[0]);
		exit(0);
	    }  
// waitpid(pid, &status, 0);
  //          Rio_writen(connfd, "RRSH COMMAND COMPLETED\n", strlen("RRSH COMMAND COMPLETED\n"));  
  } else {
  }

	waitpid(pid, &status, 0);
            Rio_writen(connfd, "RRSH COMMAND COMPLETED\n", strlen("RRSH COMMAND COMPLETED\n"));

}
}
	else
	{
		printf("Cannot exec\n");
		Rio_writen(connfd,"Cannot execute program on this server\n",38);	
	}
	
}
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

