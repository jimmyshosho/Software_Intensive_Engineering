/*
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include "csapp.h"
//********************************************
// DHIMITER SHOSHO
//IBRAHIM TAHOUN
//*******************************************
#include <stdio.h>
#include "deck.h"
#include "player.h"

// VARIAVLES DEFINES HERE
struct player* User = &user;
struct player* Computer= &computer;
// VARIABLES END HERE

// HELPER FUNCTIONS DEFINED HERE
char* listHand(struct player* target); // lists the hand of the target in an easy to read format
char* listBook(struct player* target); // lists the book of the target in an easy to read format
int bookTotal(struct player* target); // returns the book size for target

char* STRCAT(char* destination, const char* source);// used to concat strings
// SOURCE for strcat(): https://en.wikibooks.org/wiki/C_Programming/string.h/strcat

char*STRCPY(char *dest, const char *src); // copys/replaces strings
// SOURCE for strcpy(): https://en.wikibooks.org/wiki/C_Programming/string.h/strcpy

char* ITOA(int value, char* buffer, int base); // used to convert int to string
inline void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
// SOURCE for itoa, swap, and reverse: https://www.techiedelight.com/implement-itoa-function-in-c/
// HELPER FUNCTIONS END HERE
void *thread(void *vargp);

void echo(int connfd)
{
    int n=0;
    char buf[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE) != 0))
    {
        while(game_over(User)!=1 && game_over(Computer)!=1)
        {
          STRCPY(buf,listHand(User));
          STRCAT(buf,"\n");
          // code above lists players hand

          STRCAT(buf,listBook(User));
          STRCAT(buf, "\n");
          STRCAT(buf,listBook(Computer));
          STRCAT(buf, "\n");
          // code above lists both books

          STRCAT(buf, "Player 1's turn, enter a Rank (CASE SENSITIVE):\n");
          Rio_writen(connfd, buf, strlen(buf));

          Rio_readlineb(&rio, buf, MAXLINE);
          setChoice(buf[0]);

          int num = search(Computer,user_play(User));
          while(getChoice() == '.')// loops forever until user types in valid card
          {
            STRCPY(buf,"\n\n\n\n\n\n\n\n\n\nError - Enter new card that you HAVE!\n");
            Rio_writen(connfd, buf, strlen(buf));

            Rio_readlineb(&rio, buf, MAXLINE);
            setChoice(buf[0]);
            num = search(Computer,user_play(User));
          }
          if(num) //// reading in card input choice and searching if computer has it
        	  {
              STRCPY(buf, "Computer has the card ");
              char Choice[2];
              Choice[0] = getChoice();
              Choice[1] = '\0';
              STRCAT(buf, Choice);
              STRCAT(buf, "\n");
              transfer_cards(Computer,User,getChoice());
              STRCAT(buf, "Player 2's turn\n\n");
              Rio_writen(connfd, buf, strlen(buf));

            }
          else
          {
             STRCPY(buf, "Player 2 has no ");
             char Choice[2];
             Choice[0]= getChoice();
             Choice[1]= '\0';
             STRCAT(buf, Choice);
             STRCAT(buf, "\n");
             struct card* nextCard = next_card();
             add_card(User,nextCard);
             STRCAT(buf, "Go Fish, Player 1 draws ");
             char Card[3];
             Card[0]= nextCard->rank[0];
             Card[1]= nextCard->suit;
             Card[2]= '\0';
             STRCAT(buf, Card);
             STRCAT(buf, "\n");
             STRCAT(buf, "Player 2's turn");
             STRCAT(buf, "\n");
             Rio_writen(connfd, buf, strlen(buf));
          }

          if(game_over(Computer))
          {
            char Book[2];
            ITOA(bookTotal(Computer),Book, 10);
            STRCPY(buf,"------------------------Player 2 WINS!    ");
            STRCAT(buf, Book);
            ITOA(bookTotal(User),Book, 10);
            STRCAT(buf," - ");
            STRCAT(buf,Book);
            STRCAT(buf,"\n\n\n\n\n\n\n\n\n\n");
            Rio_writen(connfd, buf, strlen(buf));
            break;
           }
           if(game_over(User))
           {
             char Book[2];
             ITOA(bookTotal(User),Book, 10);
             STRCPY(buf,"------------------------Player 1 WINS!    ");
             STRCAT(buf, Book);
             ITOA(bookTotal(Computer),Book, 10);
             STRCAT(buf," - ");
             STRCAT(buf,Book);
             STRCAT(buf,"\n\n\n\n\n\n\n\n\n\n");
             Rio_writen(connfd, buf, strlen(buf));
             break;
           }

          setChoice(computer_play(Computer));
          STRCPY(buf, "Player 2 requests ");
          char Choice[2];
          Choice[0]= getChoice();
          Choice[1]= '\0';
          STRCAT(buf, Choice);
          Rio_writen(connfd, buf, strlen(buf));

          if(search(User,getChoice()))
           {
             transfer_cards(User,Computer,getChoice());
             STRCPY(buf, "\nPlayer 2 took your ");
             STRCAT(buf, Choice);
             STRCAT(buf, "'s\n\n\n");
             Rio_writen(connfd, buf, strlen(buf));
           }
          else
          {
            STRCPY(buf, "\nPlayer 1 has no ");
            char Choice[2];
            Choice[0]= getChoice();
            Choice[1]= '\0';
            STRCAT(buf, Choice);
            STRCAT(buf,"\nGo Fish Player 2!\n\n");
            add_card(Computer,next_card());
            Rio_writen(connfd, buf, strlen(buf));
          }

          if(game_over(Computer))
          {
            char Book[2];
            ITOA(bookTotal(Computer),Book, 10);
            STRCPY(buf,"------------------------Player 2 WINS!    ");
            STRCAT(buf, Book);
            ITOA(bookTotal(User),Book, 10);
            STRCAT(buf," - ");
            STRCAT(buf,Book);
            STRCAT(buf,"\n\n");
            Rio_writen(connfd, buf, strlen(buf));
            break;
           }
           if(game_over(User))
           {
             char Book[2];
             ITOA(bookTotal(User),Book, 10);
             STRCPY(buf,"------------------------Player 1 WINS!    ");
             STRCAT(buf, Book);
             ITOA(bookTotal(Computer),Book, 10);
             STRCAT(buf," - ");
             STRCAT(buf,Book);
             STRCAT(buf,"\n\n");
             Rio_writen(connfd, buf, strlen(buf));
             break;
           }
        }// end of game while

        STRCPY(buf, "Congrats! Reconnect to the server to play again!\n");
        Rio_writen(connfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        shuffle();
        reset_player(User);
        deal_player_cards(User);
        reset_player(Computer);
        deal_player_cards(Computer);
    }// end of outer while
}// end of echo function

int main(int argc, char **argv)
{
    shuffle();
    reset_player(User);
    deal_player_cards(User);
    reset_player(Computer);
    deal_player_cards(Computer);

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
      printf("@Player1 joined the game\nReady to start!\n");
    }
}

/* Thread routine */
void *thread(void *vargp) // the connection between server and client!
{
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert4:free
    echo(connfd); // stays in here forever until player leaves the game
    printf("@Player1 has left the game\nGAME OVER!\n");
    Close(connfd);

    return NULL;
}
/* $end echoservertmain */

char* listHand(struct player* target)
{
  char* hand = (char*) malloc(MAXLINE*sizeof(char));
  struct hand* temp = target->card_list;
  STRCAT(hand, "Player 1 Hand: ");

  while(temp!=NULL)
  {
    char Card[3];
    Card[0]= temp->top.rank[0];
    Card[1]= temp->top.suit;
    Card[2]= '\0';
    STRCAT(hand,Card);
    STRCAT(hand, " ");
    temp = temp->next;
  }
  return hand;
}

char* listBook(struct player* target)
{
  char* book =  (char*) malloc(MAXLINE*sizeof(char));
  if(target == User)
    STRCAT(book, "Player 1 Book: ");
  else
    STRCAT(book, "Player 2 Book: ");

  STRCAT(book, target->book);
  return book;
}

int bookTotal(struct player* target)
{
  int count = 0;
  for(int i =0; i<7;i++)
    if(target->book[i] != '\0')
      count++;
  return count;
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

char* ITOA(int value, char* buffer, int base)
{
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = abs(value);
	int i = 0;
	while (n)
	{
		int r = n % base;
		if (r >= 10)
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;
		n = n / base;
	}
	// if number is 0
	if (i == 0)
		buffer[i++] = '0';
	// If base is 10 and value is negative, the resulting string
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';
	buffer[i] = '\0'; // null terminate string
	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}
inline void swap(char *x, char *y)
 {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}
