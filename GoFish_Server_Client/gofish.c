//********************************************
// DHIMITER SHOSHO
//IBRAHIM TAHOUN
//*******************************************
#include <stdio.h>
#include "deck.h"
#include "player.h"
#include "gofish.h"
int main(int args, char* argv[])
{
  struct player* User = &user;
  struct player* Computer= &computer;
  shuffle();
  reset_player(User);
  deal_player_cards(User);
  reset_player(Computer);
  deal_player_cards(Computer);
  int isValid = 0;
  while(isValid != 1)
{
  while(game_over(User)!=1 && game_over(Computer)!=1)
  {
    printf("Player 1 Hand: ");
    listHand(User);

    printf("\nPlayer 1 Book: ");
    listBook(User);

    printf("\nPlayer 2 Book: ");
    listBook(Computer);

    char i = user_play(User);
    if(search(Computer,i))
  	   transfer_cards(Computer,User,i);
    else
    {
  	   printf("Player 2 has no %c, Go Fish Player 1!",i);
       add_card(User,next_card());
    }

    printf("\nPlayer 2's turn");

    i = computer_play(Computer);
    printf("\nPlayer 2 requests %c",i);
    if(search(User,i))
  	   transfer_cards(User,Computer,i);
    else
    {
      printf("\nPlayer 1 has no %c, Go Fish Player 2!",i);
      add_card(Computer,next_card());
    }

    printf("\n-----------------------------------------------\n");

    if(game_over(User))
      printf("\nPlayer 1 WINS! %i-%i",bookTotal(User),bookTotal(Computer));
    if(game_over(Computer))
      printf("\nPlayer 2 WINS! %i-%i",bookTotal(Computer),bookTotal(User));
  }
    char ans;
    printf("\nDo you want to play again? [y,n]:");
    scanf("\n%c", &ans);
    if( ans == 'n' || ans == 'N')
      isValid = 1;
    if( ans== 'y' || ans == 'Y')
    {
      shuffle();
      reset_player(User);
      deal_player_cards(User);
      reset_player(Computer);
      deal_player_cards(Computer);
    }

}

}

void listHand(struct player* target)
{
  struct hand* temp = target->card_list;
  while(temp!=NULL)
  {
    printf("%s%c ",temp->top.rank,temp->top.suit);
    temp = temp->next;
  }
}

void listBook(struct player* target)
{
  for(int i=0;i<7;i++)
     printf("%c ",target->book[i]);
}

int bookTotal(struct player* target)
{
  int count = 0;
  for(int i =0; i<7;i++)
    if(target->book[i] != '\0')
      count++;
  return count;
}
