/****
  Programming Concepts - Assignment Two, sp2 2023
  File:  blackJack.h

  DO NOT MODIFY THIS FILE!
****/

#ifndef BLACKJACK_H
#define BLACKJACK_H

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CARDS 52
#define SCORE 21
#define MAX_IN_HAND 10
#define PLAYER_MIN_SCORE 15
#define DEALER_MIN_SCORE 17

#define PLAYER 3
#define DEALER 0
#define DRAW 1

#define MAX_STRING 128

static char *card_face[] = {"A", "2", "3", "4", "5", "6", "7", "8",
                            "9", "10", "J", "Q", "K"};
static char *card_suit[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
static int card_value[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

struct card
{
  char *face;
  char *suit;
  int value;
};
typedef struct card Card;

/* Shuffle deck of cards.  Deitel & Deitel - C How to Program. Pearson Education. */
void shuffle_Cards(Card *deck);

/* Deal a card from the top of the deck. Modified from: Deitel & Deitel - C How to Program. Pearson Education. */
void deal_one_card(Card deck[], Card *one_card);

/* Function to play one game of Blackjack adhering to the rules provided in the assignment
   specification.  The function takes the deck of cards as a parameter and
   returns the result of the player's game against the dealer (computer).

   The function returns 3 if the player wins, 1 if the player draws with the dealer and
   0 if the player loses. */
int play_one_game(Card deck[]);

#endif