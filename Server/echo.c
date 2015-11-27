/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
extern char **environ;
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
int parseline(char *buf, char **argv);
void echo(int connfd) 
{
    size_t n;
    size_t a;
    char buf[MAXLINE];
    char buf2[MAXLINE];
    char buf3[MAXLINE];
    rio_t rio;
    rio_t rio2;
    //pid_it pid;
    Rio_readinitb(&rio, connfd);
    char *argv[128]; /* Argument list execve() */
    FILE *fp1;
    char oneword[100];
    char c;
	Rio_readlineb(&rio, buf, MAXLINE);
	Rio_writen(connfd,"Yes\n",4);
	Rio_readlineb(&rio, buf2, MAXLINE);
      //  Rio_writen(connfd,"Yes\n",4);
        fp1 = fopen("rrshusers.txt","r");
	int auth = 0;
   do {
      c = fscanf(fp1,"%s",oneword); /* got one word from the file */
     // printf("%s\n",oneword);       /* display it on the monitor  */
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
	printf("asdasd\n");
	if (auth == 0)
	{
		Rio_writen(connfd,"Login Failed\n",13);	
	}
    fclose(fp1);


    while((n = Rio_readlineb(&rio, buf3, MAXLINE)) != 0) { //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);
	// int bg = parseline(buf3, argv);
	 if (strcmp("/bin/ls\n",buf3) ==0 )
	 {
	   int bg = parseline(buf3, argv);
	    fp1 = fopen("rrshcommands.txt","r");
        int allowed = 0;
   do {
      c = fscanf(fp1,"%s",oneword); /* got one word from the file */
      printf("%s\n",oneword);       /* display it on the monitor  */
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
                Rio_writen(connfd,"Cannot execute ’program’ on this server\n",41);
        }
	else
	{
		int link[2];
                pid_t pid;
  char foo[4096];

  if (pipe(link)==-1)
    die("pipe");
  if ((pid = fork()) == -1)
    die("fork");
  if(pid == 0) {
    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
//  char *argv[] = { "/bin/cat", "abc.c" , 0 };
    execve(argv[0], argv, environ);
          die("execve");
  } else {
    close(link[1]);
    int nbytes = read(link[0], foo, sizeof(foo));
    printf("%s", foo);
    Rio_writen(connfd,foo,sizeof(foo));
    wait(NULL);
  }
}
}
	else
	{
		Rio_writen(connfd,"No\n",3);	
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


