/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;
    char buf2[MAXLINE];
    char buf3[MAXLINE];
    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);


    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
    int auth = 0;
	printf("UserName: ");
	Fgets(buf, MAXLINE, stdin);
	Rio_writen(clientfd, buf, strlen(buf));
	Rio_readlineb(&rio, buf, MAXLINE);
	printf("Password: ");
        Fgets(buf2, MAXLINE, stdin);
	Rio_writen(clientfd, buf2, strlen(buf2));
//	Fputs(buf, stdout);
	Rio_readlineb(&rio, buf2, MAXLINE);
	
    	Fputs(buf2, stdout);
	if (strcmp("Login Failed\n",buf2)==0)
	{
		exit(0);
	}
printf("rrsh > ");
    while (Fgets(buf3, MAXLINE, stdin) != NULL) {
	if ((strcmp("quit\n",buf3) == 0))
        {
	  exit(0);
//        Fputs(buf3, stdout);
        }
	Rio_writen(clientfd, buf3, strlen(buf3));
	Rio_readlineb(&rio, buf3, MAXLINE);
	Fputs(buf3, stdout);
	//printf("rrsh > " );
	if ((strcmp("Cannot execute 'program' on this server\n",buf3) == 0))
	{
//	  Fputs(buf3, stdout);
	}
	else if (strcmp("RRSH COMMAND COMPLETED\n",buf3) ==0)
         {
               
         }
	else
	{
	while ((Rio_readlineb(&rio, buf3, MAXLINE))!=0)
	{
	 
	 Fputs(buf3, stdout);
	 if (strcmp("RRSH COMMAND COMPLETED\n",buf3) ==0)
	 {
		break;
	 }
	}
	}
	printf("rrsh > ");
//	Fputs(buf3, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
