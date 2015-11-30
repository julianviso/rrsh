/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
#define MAX_USR_PASS 42
#define MAXARGS   128
extern char **environ;
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
int parseline(char *buf, char **argv);
void run_commands(char *username, rio_t rio, int connfd);
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
  //  char username[MAX_USR_PASS];
//    Rio_readlineb(&rio, username, MAX_USR_PASS);
   // Rio_writen(connfd, "Login Approved\n", strlen("Login Approved\n"));
   // Rio_readlineb(&rio, username, MAX_USR_PASS);
   // Rio_writen(connfd, "Login Approved\n", strlen("Login Approved\n"));
   // run_commands("aasd", rio, connfd);
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

void run_commands(char *username, rio_t rio, int connfd) {

    FILE *fp;
    char *argv[MAXARGS];
    char buf[MAXLINE];
    size_t valid = 0;
    pid_t pid;
    int status; 
    char f_cmd[MAXLINE];

    fp = fopen("rrshcommands.txt", "r");
    // Read a line from the client
    while (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
	printf("%s",buf);                
        // Remove the new line character transferred over the network
//        buf[strlen(buf)-2] = '\0';
        // Parse the command line
        parseline(buf,argv);

        if (argv != NULL) { 
            printf("User %s sent the command '%s' to be executed.\n", username, buf);
            // Open the file and determine if the input is a valid command
            while (fgets(&f_cmd[0], MAXLINE, fp) != NULL && !valid) {
                f_cmd[strlen(f_cmd)-1] = '\0';
                if (strncmp(argv[0], f_cmd, MAXLINE) == 0)
                    valid = 1;
            }
            // Fork a child and dup the output to the client
            if ((pid = fork()) == 0) {   /* Child runs user job */
                // If its a valid command, log and display output to client
                if (valid) {
                    printf("Forking/Execing the command ’%s’ on behalf of %s.\n", buf, username);
                    dup2(connfd, 1);
                    if (execve(argv[0], argv, environ) < 0) 
                        exit(0);
                }
                // If it is not, log and display error to the client
                else {
                    printf("The command '%s' is not allowed.\n", buf);
                    dup2(connfd, 1);
                    printf("Cannot execute ’%s’ on this server\n", buf);
                    exit(0); 
                }
            }
            // Wait for the child process to complete; Output finished command
            waitpid(pid, &status, 0);
            Rio_writen(connfd, "RRSH COMMAND COMPLETED\n", strlen("RRSH COMMAND COMPLETED\n"));            
            // Start back at the top of the file 
            rewind(fp);
            valid = 0;
        }
    }
    close(fp);
}
