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
//	Fputs(buf, stdout);

	printf("Password: ");
        Fgets(buf2, MAXLINE, stdin);
	Rio_writen(clientfd, buf2, strlen(buf2));
//	Fputs(buf, stdout);
	Rio_readlineb(&rio, buf2, MAXLINE);
    	Fputs(buf2, stdout);
/*	if (strcmp("Yes\n",buf2)==0)
	{
	printf("YESSS\n");
	}*/
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
	Rio_writen(clientfd, buf, strlen(buf));
	Rio_readlineb(&rio, buf, MAXLINE);
	Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
