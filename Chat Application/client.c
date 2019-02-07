/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

char* STRCAT(char* destination, const char* source);// used to concat strings
// SOURCE for strcat(): https://en.wikibooks.org/wiki/C_Programming/string.h/strcat
char*STRCPY(char *dest, const char *src); // copys/replaces strings
// SOURCE for strcpy(): https://en.wikibooks.org/wiki/C_Programming/string.h/strcpy
void* sendMSG();
void* readMSG();
char *host, *port, *username, buf[MAXLINE];
int clientfd;
rio_t rio;
pthread_t tid1, tid2;
char tempLine[MAXLINE];


int main(int argc, char **argv)
{
    if (argc != 4)
    {
	     fprintf(stderr, "usage: %s <host> <port> <username>\n", argv[0]);
	     exit(0);
    }
    host = argv[1];
    port = argv[2];
    username = argv[3];

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
    Pthread_create(&tid1,NULL,sendMSG,NULL);
    Pthread_create(&tid2,NULL,readMSG,NULL);
    Pthread_join(tid1,NULL);
    Pthread_join(tid2,NULL);
    Close(clientfd); //line:netp:echoclient:closes
    exit(0);
}
/* $end echoclientmain */


void* sendMSG()
{
  printf("Welcome %s! press ENTER to begin chatting!", username);
  int flag  = 0;
  while (Fgets(buf, MAXLINE, stdin) != NULL)
  {

    if(flag == 0)
    {
      STRCPY(buf,"@");
      STRCAT(buf,username);
      STRCAT(buf," joined\n");
      flag=1;
    }
    Rio_writen(clientfd, buf, strlen(buf));
    //Rio_readlineb(&rio, buf, MAXLINE);
    //puts(buf, stdout);

    printf("> ");
    STRCPY(tempLine,buf);
  }
  return NULL;
}
void* readMSG()
{
  while(1)
  {
    Rio_readlineb(&rio, buf, MAXLINE);
    if(strcmp(buf,tempLine) == 0)
        printf("");
    else if(strcmp(buf, "Goodbye!\n") == 0)// if client recieves exactly "Goodbye!" from server close connection
    {
      Fputs(buf,stdout);
      Close(clientfd); //line:netp:echoclient:closes
      exit(0);
    }
    else if(strcmp(buf,tempLine) != 0)
      printf("%s", buf);
  }

  return NULL;
}
char* STRCAT(char* destination, const char* source)
{
	// make ptr point to the end of destination string
	char* ptr = destination + strlen(destination);
	// Appends characters of source to the destination string
	while (*source != '\0')
		*ptr++ = *source++;
	// null terminate destination string
	*ptr = '\0';
	// destination is returned by standard STRCAT()
	return destination;
}
char*STRCPY(char *dest, const char *src)
{
  unsigned i;
 for (i=0; src[i] != '\0'; ++i)
   dest[i] = src[i];

 //Ensure trailing null byte is copied
 dest[i]= '\0';

 return dest;
}
