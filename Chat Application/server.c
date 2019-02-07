/*
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include "csapp.h"
#include "string.h"

//INITIALIZED VARIABLES START HERE
char users[100][100]; // server supports up to 100 users whos names can each be 100 characters long
int userIndex = 0;
int  userID[100]; // each user's custom ID
//INITIALIZED VARIABLES END HERE

//HELPER FUNCTIONS START HERE

char* STRCAT(char* destination, const char* source);// used to concat strings
// SOURCE for strcat(): https://en.wikibooks.org/wiki/C_Programming/string.h/strcat
char*STRCPY(char *dest, const char *src); // copys/replaces strings
// SOURCE for strcpy(): https://en.wikibooks.org/wiki/C_Programming/string.h/strcpy

// HELPER FUNCTIONS END HERE

void *thread(void *vargp);

void echo(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
      if(strstr(buf,"joined") !=NULL)
      {
        char userName[100];
        for(int i = 0; i< 100; i++)
        {
          if(buf[i] != ' ')
            userName[i] = buf[i];
          else
          {
            userName[i] = '\0';
            i = 100;
          }
        }
        STRCPY(users[userIndex], userName);
        userID[userIndex] = connfd; // gets client ID
        userIndex++;
        printf("%s", buf);
        STRCPY(buf,"\n");
        Rio_writen(connfd, buf, strlen(buf));
      }
      else if( strcmp(buf, "quit\n") == 0)
      {
        int ID = connfd;
        int index = 0;
        for(int i = 0; i<userIndex; i++)
        {
          if(userID[i] == ID)
          {
            index = i;
            break;
          }
        }
        printf("%s left\n", users[index]);

        STRCPY(users[index], ""); // clears the user in that index
        userID[index] = 999;      // removes user's id with 999

        STRCPY(buf, "Goodbye!\n");
        Rio_writen(connfd, buf, strlen(buf));
      }
      else if(strcmp(buf, "list-users\n") == 0)
      {
        for(int i = 0; i<userIndex; i++)
        {
          if(i == 0)
          {
            char user[100];
            STRCPY(user, users[i]);
            STRCPY(buf, user);
            STRCAT(buf," ");
          }
          else
          {
            char user[100];
            STRCPY(user, users[i]);
            STRCAT(buf,user);
            STRCAT(buf, " ");
          }
        }
        STRCAT(buf, "\n");
        Rio_writen(connfd, buf, strlen(buf));
      }// end of "list-users" if
      // if user doesn't enter valid command or doesn't @ anybody, prompt them to retype
      else if(strstr(buf, "@") == NULL)
      {
        STRCPY(buf, "Please enter a valid command (<quit> or <list-users>) or @ another user!\n");
        Rio_writen(connfd, buf, strlen(buf));
      }
      else if(strstr(buf, "@") != NULL) // if buf contains @, possible communication with other client
      {
        char sendTo[100];
        for(int i = 0; i<100; i++)
        {
          if(buf[i] != ' ')
            sendTo[i] = buf[i];
          if(buf[i] == ' ')
          {
            sendTo[i] = '\0';
            break;
          }
        }
        int index = -1;
        for(int j = 0; j< userIndex; j++)
        {
            if(strcmp(sendTo, users[j]) == 0)
            {
              index = j;
              break;
            }
        }
        int sendToID = userID[index];
        if(index == -1)
        {
            STRCPY(buf, "User does not exist or you did not type a message to the user!\n");
            Rio_writen(connfd, buf, strlen(buf));
        }
        else
        {
          char sender[100];
          for(int i = 0; i<userIndex; i++)
          {
            if(userID[i] == connfd)
              STRCPY(sender, users[i]);
          }
          // now sender has sender

          int index = 0;
          for(int i=0; i<MAXLINE;i++)
          {
            if(buf[i] == ' ')
            {
              index = i;
              break;
            }
          }
          char tempMessage[MAXLINE];
          int count = 0;
          for(int i = index; i<MAXLINE; i++)
          {
            tempMessage[count] = buf[i];
            count++;
          }
          STRCPY(buf, sender);
          STRCAT(buf, tempMessage);
          Rio_writen(sendToID, buf, strlen(buf));
        }

      }
    }// end of readline loop
}

int main(int argc, char **argv)
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2)
    {
	     fprintf(stderr, "usage: %s <port>\n", argv[0]);
	     exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    while (1)
    {
      clientlen=sizeof(struct sockaddr_storage);
	    connfdp = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc
	    *connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc
	    Pthread_create(&tid, NULL, thread, connfdp);
    }
}

/* Thread routine */
void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    echo(connfd);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */


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
