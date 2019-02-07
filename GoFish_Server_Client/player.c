#include "player.h"
#include <stdio.h>

char choice;
int isValid;
//Add a new card to the player's hand.
//target: the target player
//new_card: pointer to the new card to add
//returns: return 0 if no error, non-zero otherwise
 int add_card(struct player* target, struct card* new_card)
 {

   struct hand* current = target->card_list;
   struct hand* nextHandPT;

   while( current != NULL && current->next !=NULL)
      current=current->next;

   nextHandPT = (struct hand*) malloc(sizeof(struct hand));
   nextHandPT->top.suit = new_card->suit;
   nextHandPT->top.rank[0] = new_card->rank[0];
   nextHandPT->top.rank[1] = new_card->rank[1];
   nextHandPT->next = NULL;

   if(current != NULL)
     current->next = nextHandPT;
   else
     target->card_list = nextHandPT;

   target->hand_size++;
   check_add_book(target);
  return 0;
 }


// Function: remove_card
//Remove a card from the player's hand.
//target: the target player
//old_card: pointer to the old card to remove
//returns: return 0 if no error, non-zero otherwise
int remove_card(struct player* target, struct card* old_card)
{
  struct hand* current = target->card_list;
  if(target->card_list != NULL)
  {

    if( current->top.suit == old_card->suit && current->top.rank[0] == old_card->rank[0] )
      {
        target->card_list=current->next;
        target->hand_size--;
        free(current);
        return 0;
      }
  }
  else
    return -1;

  while( current->next->top.suit != old_card->suit || current->next->top.rank[0] != old_card->rank[0] )
        {
          current = current->next;
          if(current->next == NULL)
              return -1;
        }
  struct hand* temp = current->next;
  current->next = temp->next;
  target->hand_size--;
  free(temp);

  return 0;

}

//Function: check_add_book
//Check if a player has all 4 cards of the same rank.
//If so, remove those cards from the hand, and add the rank to the book.
//Returns after finding one matching set of 4, so should be called after adding each a new card.
//target: pointer to the player to check
//Return: a char that indicates the book that was added; return 0 if no book added.
char check_add_book(struct player* target)
{
  struct hand* temp = target->card_list;
  char ranking = temp->top.rank[0];
  int b_count=0;
  while(temp->next != NULL)
    temp = temp->next;
                                  //increment to last card in linked list
  ranking = temp->top.rank[0];      //set card ranking, last card is most recently added

  temp = target->card_list;         //loop back from start, count if 4 of same card rank
while(temp != NULL)
{
  if(temp->top.rank[0] == ranking)
    b_count++;

  temp= temp->next;
}
if(b_count == 4)
{                   // 4 cards of same rank, loop back thru, remove them
  temp = target->card_list;
  while(temp!=NULL)
  {
    if(temp->top.rank[0] == ranking)
    {
      struct card* Card = &temp->top;
      remove_card(target, Card);
      int i = 0;
      while(target->book[i]!='\0' && target->book[i] != ranking)                // update book in nearest empty index
        i++;
      target->book[i]=ranking;
    }
    temp=temp->next;
  }
}

 return ranking;

}




//Function: search
//Search a player's hand for a requested rank.
//rank: the rank to search for
//target: the player (and their hand) to search
//Return: If the player has a card of that rank, return 1, else return 0
int search(struct player* target, char rank)
{
  struct hand* temp = target->card_list;
  while(temp !=NULL)
  {
    if(temp->top.rank[0] == rank)
      return 1;
    temp = temp->next;
  }
  return 0;
}

//Function: transfer_cards
//Transfer cards of a given rank from the source player's
//hand to the destination player's hand. Remove transferred
//cards from the source player's hand. Add transferred cards
//to the destination player's hand.
//src: a pointer to the source player
//dest: a pointer to the destination player
//rank: the rank to transfer
//Return: 0 if no cards found/transferred, <0 if error, otherwise
//return value indicates number of cards transferred
int transfer_cards(struct player* src, struct player* dest, char rank)
{
  struct hand* temp = src->card_list;
  struct card* cardPT;
  int count = 0;

  if (src==NULL || dest==NULL)
    return -1;
  if( search(src,rank) == 0)
    return 0;

  while( temp != NULL )
  {
    cardPT=(struct card*) malloc(sizeof(struct card));
    if(temp->top.rank[0] == rank)
      {
        cardPT->suit = temp->top.suit;
        cardPT->rank[0]= temp->top.rank[0];
        cardPT->rank[1]= '\0';
        add_card(dest,cardPT);
        remove_card(src,cardPT);
        count++;
      }
      temp = temp->next;
  }

  return count;
}

//Function: game_over
//Boolean function to check if a player has 7 books yet
//and the game is over
//target: the player to check
//Return: 1 if game is over, 0 if game is not over
int game_over(struct player* target)
{

  for(int i=0; i<7;i++)
    if(target->book[i] == '\0')
      return 0; // game is over

  return 1;
}


//Function: reset_player
//Reset player by free'ing any memory of cards remaining in hand,
//and re-initializes the book.  Used when playing a new game.
//target: player to reset
//Return: 0 if no error, and non-zero on error.
 int reset_player(struct player* target)
 {
   if(target==NULL)
    return -1;

  free(target->card_list);

  for(int i=0; i<7;i++)
    target->book[i]='\0';

  target->hand_size = 0;

  return 0;
 }


//Function: computer_play
//Select a rank randomly to play this turn. The player must have at least
//one card of the selected rank in their hand.
//target: the player's hand to select from
//Rank: return a valid selected rank
char computer_play(struct player* target)
{
  if(target == NULL)
    return -1;

  int whichCard = rand() % target->hand_size;
  struct hand* temp = target->card_list;

  int count = 0;
  while(count != whichCard)
    {
      temp = temp->next;
      count++;
    }

  return temp->top.rank[0];
}

//Function: user_play
//Read standard input to get rank user wishes to play.  Must perform error
//checking to make sure at least one card in the player's hand is of the
//requested rank.  If not, print out "Error - must have at least one card from rank to play"
//and then re-prompt the user.
//target: the player's hand to check
//returns: return a valid selected rank
char user_play(struct player* target)
{
  if(target == NULL)
    return -1;

  //char choice;
  isValid = 0;

  while(isValid == 0)
  {
    //printf("\nPlayer 1's turn, enter a Rank (CASE SENSITIVE):");
    //scanf("\n%c", &choice);
    struct hand* temp = target->card_list;
    while(temp!=NULL)
    {
      if(temp->top.rank[0] == choice)
          isValid = 1;
      temp=temp->next;
    }

    if(isValid == 0)
    {
      setChoice('.');
      return choice;
    }
  }
  check_add_book(target);
  return choice;
}

void setChoice(char newChoice)
{
  choice = newChoice;
}

char getChoice()
{
  return choice;
}

//Player.c ENDS HERE
