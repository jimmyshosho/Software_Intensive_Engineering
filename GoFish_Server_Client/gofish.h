#ifndef GOFISH_H
#define GOFISH_H

/*
   Define any prototype functions
   for gofish.h here.
*/
void listHand(struct player* target); // lists the hand of the target in an easy to read format
void listBook(struct player* target); // lists the book of the target in an easy to read format
int bookTotal(struct player* target); // returns the book size for target

#endif
