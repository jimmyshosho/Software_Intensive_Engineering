/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
#include <stdio.h>
int STRCMP (const char *p1, const char *p2); // compares strings


int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 2)
    {
	     fprintf(stderr, "usage: %s <host> \n", argv[0]);
	     exit(0);
    }
    host = argv[1];
    port = "8080";// PORT IS HARDSET TO 8080
    clientfd = Open_clientfd(host, port);

    Rio_readinitb(&rio, clientfd);
    int flag = 0;
    printf("Shuffling Deck... \nWelcome to Go Fish! Press Enter to START\n");
      while (Fgets(buf, MAXLINE, stdin) != NULL)
      {
        if(flag==0)
        {
          flag=1;
          Rio_writen(clientfd, buf, strlen(buf));
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
        }
        else
        {
          Rio_writen(clientfd, buf, strlen(buf));
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
          Rio_readlineb(&rio, buf, MAXLINE);
          Fputs(buf, stdout);
        }

      }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
int STRCMP (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}
