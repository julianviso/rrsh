/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
void echo(int connfd) 
{
    size_t n;
    size_t a;
    char buf[MAXLINE];
    char buf2[MAXLINE];
    char ab[4] = "YOLO"; 
    rio_t rio;
    rio_t rio2;
    pid_t pid;
    Rio_readinitb(&rio, connfd);

    FILE *fp1;
    char oneword[100];
    char c;
	Rio_readlineb(&rio, buf, MAXLINE);
	Rio_writen(connfd,"Yes\n",4);
	Rio_readlineb(&rio, buf2, MAXLINE);
      //  Rio_writen(connfd,"Yes\n",4);
        fp1 = fopen("rrshusers.txt","r");

   do {
      c = fscanf(fp1,"%s",oneword); /* got one word from the file */
      printf("%s\n",oneword);       /* display it on the monitor  */
	if (strcmp(oneword,strtok(buf,"\n"))==0)
	{	
		c = fscanf(fp1,"%s",oneword);
		if (strcmp(oneword,strtok(buf2,"\n"))==0)
		{
		   Rio_writen(connfd,"Yessss\n",7);
		}
	}
   } while (c != EOF);              /* repeat until EOF           */

    fclose(fp1);


    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);
	 if (strcmp("abc\n",buf) ==0 )
	{
	  Rio_writen(connfd,"Yes\n",4);
	}
	else
	{
		Rio_writen(connfd,"No\n",3);	
	}
	}
}
/* $end echo */
