/*
 * echoclient.c - An echo client
 *:%i:/
* $begin echoclientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd, port;
    char *host, username[MAXLINE];
    rio_t rio;
    char password[MAXLINE];
    char inputString[MAXLINE];
    char tmp[MAXLINE];
//    char tmp2[MAXLINE];
    char tmp3[strlen("Cannot execute ")];
    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);


    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
	printf("UserName: ");
	Fgets(username, MAXLINE, stdin);
	Rio_writen(clientfd, username, strlen(username));
	Rio_readlineb(&rio, username, MAXLINE);
	printf("Password: ");
        Fgets(password, MAXLINE, stdin);
	Rio_writen(clientfd, password, strlen(password));
	Rio_readlineb(&rio, password, MAXLINE);
	
    	Fputs(password, stdout);
	if (strcmp("Login Failed\n",password)==0)
	{
		exit(0);
	}
	printf("rrsh > ");
    while (Fgets(inputString, MAXLINE, stdin) != NULL) {
	if ((strcmp("quit\n",inputString) == 0))
        {
	  exit(0);
        }
	/*strcpy(tmp2,inputString);
	tmp2[strlen(tmp2)-1] = '\0';
	strcat(tmp,"Cannot execute '");
	strcat(tmp,tmp2);
	strcat(tmp,"' on this server\n");
	*/
	Rio_writen(clientfd, inputString, strlen(inputString));
	Rio_readlineb(&rio, inputString, MAXLINE);
	Fputs(inputString, stdout);
	strncpy(tmp3,inputString,strlen("Cannot execute"));
	if ((strcmp(tmp3,"Cannot execute") == 0))
	{
		strcpy(tmp,"");
	}
	else if (strcmp("RRSH COMMAND COMPLETED\n",inputString) ==0)
         {
               
         }
	else
	{
	while ((Rio_readlineb(&rio, inputString, MAXLINE))!=0)
	{
	 
	 Fputs(inputString, stdout);
	 if (strcmp("RRSH COMMAND COMPLETED\n",inputString) ==0)
	 {
		break;
	 }

	}
	}
	printf("rrsh > ");
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
