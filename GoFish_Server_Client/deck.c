#include "deck.h"
#include "player.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

size_t deckSize = 52;
struct deck* Deck = &deck_instance;

void resetDeck();// creates a generic 52 card deck
void mixDeck(); // mixes and shuffles the deck

// Initializes deck_instance and shuffles it.
//Resets the deck if a prior game has been played.
//returns: 0 if no error, and non-zero on error
int shuffle()
{
  resetDeck();
  mixDeck();
  return 0;
}
// returns: 0 if no error, and non-zero on error
int deal_player_cards(struct player* target)
{
  if(target == NULL)
    return -1;

  for(int i =0; i<7; i++)
    add_card(target,next_card());

  return 0;
}

//Return a pointer to the top card on the deck.
//Removes that card from the deck.
//returns: pointer to the top card on the deck.
struct card* next_card( )
{
  struct card* temp = &Deck->list[Deck->top_card];
  Deck->top_card=Deck->top_card-1;
  deckSize = deckSize -1;
  return temp;
}

//Return the number of cards left in the current deck.
//returns: number of cards left in the deck.
size_t deck_size( )
{
  return deckSize;
}


void resetDeck()
{
  char suit[4] = {'C','D','H','S'};// array of all suits
  char ranks[13]={'2','3','4','5','6','7','8','9','0','J','Q','K','A'};
  int count = 0;
  for( int i = 0; i<4; i++)
  { for(int j = 0; j<13;j++)
      {
        Deck->list[count].suit = suit[i];
        Deck->list[count].rank[0] = ranks[j];
        Deck->list[count].rank[1]= '\0';
        count++;
      }
  }
    Deck->top_card = 51;
    deckSize = 52;
}

void mixDeck()
{
  int seed = time(NULL);
  for(int i =0; i<deck_size();i++)
  {
    srand(seed);
    int num = i + rand()/(RAND_MAX/(deck_size()-i)+1);
    struct card temp = Deck->list[num];
    Deck->list[num]= Deck->list[i];
    Deck->list[i] = temp;
  }

}
