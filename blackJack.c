/****
  Programming Concepts - Assignment Two, sp2 2023
  File:  blackJack.c

  DO NOT MODIFY THIS FILE!
****/

#include "blackJack.h"

/* In this case, the keyword static indicates that the variable
   cardsDealt is known only to functions in the file in which it is defined.
   Although it is a global variable, it has file scope. */
static int cards_dealt = MAX_CARDS;

/* Flag to determine whether we have created the deck. */
static int empty_deck = 1;

/* Determines whether string contains only digits */
static int is_number(char *number);

/* Fill deck of cards. Deitel & Deitel - C How to Program. Pearson Education. */
static void fill_deck(Card *deck)
{

  int i;

  for (i = 0; i < MAX_CARDS; i++)
  {
    deck[i].face = card_face[i % 13];
    deck[i].suit = card_suit[i / 13];
    deck[i].value = card_value[i % 13];
  }
}

/* Shuffle deck of cards. Deitel & Deitel - C How to Program. Pearson Education. */
void shuffle_cards(Card *deck)
{
  int i;
  int j;
  Card temp;

  srand(time(NULL));

  /* check for emptyDeck - if empty, then fill deck. */
  if (empty_deck)
  {
    fill_deck(deck);
    empty_deck = 0;
  }

  for (i = 0; i < MAX_CARDS; i++)
  {
    j = rand() % MAX_CARDS;
    temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
}

/* Deal a card from the top of the deck. Modified from: Deitel & Deitel - C How to Program. Fifth Edition. Pearson Education. */
void deal_one_card(Card deck[], Card *one_card)
{

  /* if we have run out of cards (i.e. dealt all cards) then shuffle deck. */
  if (cards_dealt >= MAX_CARDS)
  {
    shuffle_cards(deck);
    cards_dealt = 0;
  }

  /* deal next card */
  *one_card = deck[cards_dealt];
  cards_dealt++;
}

/* Function that determines the point value of a hand. */
static int check_hand(Card deck[], int no_cards)
{
  int i = 0;
  int value = 0;
  int ace_value = 0;

  for (i = 0; i < no_cards; i++)
  {

    /* if ace is found - treat it as a one (but only once). */
    if ((strcmp(deck[i].face, "A") == 0) && (value == ace_value))
      ace_value += 1;
    else
      ace_value += deck[i].value;

    value += deck[i].value;
  }

  /* determine best hand and return value */
  if (value > SCORE)
    return ace_value;

  return value;
}

/* Function that displays the value and content of a hand. */
static void display_hand(char *string, Card deck[], int no_cards)
{

  int i = 0;
  int hand_value = 0;

  hand_value = check_hand(deck, no_cards);
  printf("%s %2d: ", string, hand_value);

  for (i = 0; i < no_cards; i++)
  {
    printf("%s", i == 0 ? " " : "| ");
    printf("%-s of %-s ", deck[i].face, deck[i].suit);
  }
  printf("\n");
}

/* Function to play one game of Blackjack adhering to the rules provided in the assignment
   specification.  The function takes the deck of cards as a parameter and
   returns the result of the player's game against the dealer (computer).

   The function returns 3 if the player wins, 1 if the player draws with the dealer and
   0 if the player loses. */
int play_one_game(Card deck[])
{

  int dealer_value = 0; /* point value of dealer's hand */
  int player_value = 0; /* point value of player's hand */

  Card player_hand[MAX_IN_HAND]; /* cards player is holding */
  Card dealer_hand[MAX_IN_HAND]; /* cards dealer is holding */
  int player_cards_dealt = 0;    /* number of cards player has been dealt */
  int dealer_cards_dealt = 0;    /* number of cards dealer has been dealt */

  char option = 0; /* stores hit / stand option entered by player */
  int quit = 0;    /* flag used by dealer to signal end of hand   */
  int winner = 0;  /* stores winner of game                       */

  printf("\n\n---------------------- START GAME ----------------------\n");

  /* deal first round of cards */
  deal_one_card(deck, &player_hand[player_cards_dealt++]);
  deal_one_card(deck, &dealer_hand[dealer_cards_dealt++]);

  /* deal second round of cards */
  deal_one_card(deck, &player_hand[player_cards_dealt++]);
  deal_one_card(deck, &dealer_hand[dealer_cards_dealt++]);

  /* display cards */
  display_hand("| Dealer's hand is", dealer_hand, dealer_cards_dealt - 1);
  display_hand("| Player's hand is", player_hand, player_cards_dealt);

  printf("|\n");

  /*** check for Blackjack ***/
  player_value = check_hand(player_hand, player_cards_dealt);
  dealer_value = check_hand(dealer_hand, dealer_cards_dealt);
  if ((player_value == SCORE) && (dealer_value == SCORE))
  {
    printf("| *** Blackjack --");
    display_hand("| Dealer's hand is", dealer_hand, dealer_cards_dealt);
    display_hand("| Player's hand is", player_hand, player_cards_dealt);

    printf("| *** Blackjack! Push - no winners! ***\n");
    winner = DRAW;
  }
  else if (player_value == SCORE)
  {
    printf("| *** Blackjack! Player Wins! ***\n");
    winner = PLAYER;
  }
  else if (dealer_value == SCORE)
  {
    printf("| *** Blackjack! Dealer Wins! ***\n");
    display_hand("| Dealer's hand is", dealer_hand, dealer_cards_dealt);
    winner = DEALER;
  }

  /*** else if neither player or dealer have Blackjack then play out hand ***/
  else
  {
    printf("| Please enter h or s (h = Hit, s = Stand): ");
    scanf("%c", &option);
    while (option == '\n')
      scanf("%c", &option);
    getchar();

    while (option != 'h' && option != 's')
    {
      printf("| Please enter h or s (h = Hit, s = Stand): ");
      scanf("%c", &option);
      while (option == '\n')
        scanf("%c", &option);
      getchar();
    }

    if (player_value < PLAYER_MIN_SCORE && option == 's')
    {
      printf("|\n| Cannot stand on value less than %d!\n", PLAYER_MIN_SCORE);
      option = 'h';
    }

    /*** deal player's hand ***/
    while (player_value <= SCORE && option == 'h' && player_cards_dealt < MAX_IN_HAND)
    {
      deal_one_card(deck, &player_hand[player_cards_dealt++]);
      display_hand("|\n| Player's hand is", player_hand, player_cards_dealt);
      player_value = check_hand(player_hand, player_cards_dealt);

      if (player_value <= SCORE)
      {
        printf("| Please enter h or s (h = Hit, s = Stand): ");
        scanf("%c", &option);
        while (option == '\n')
          scanf("%c", &option);
        getchar();

        while (option != 'h' && option != 's')
        {
          printf("| Please enter h or s (h = Hit, s = Stand): ");
          scanf("%c", &option);
          while (option == '\n')
            scanf("%c", &option);
          getchar();
        }
      }

      if (player_value < PLAYER_MIN_SCORE && option == 's')
      {
        printf("|\n| Cannot stand on value less than %d!\n", PLAYER_MIN_SCORE);
        option = 'h';
      }
    }

    if (player_value > SCORE)
      printf("| *** Player busts!\n");

    /*** deal dealer's hand ***/
    dealer_value = 0;
    display_hand("|\n| Dealer's hand is", dealer_hand, dealer_cards_dealt);
    dealer_value = check_hand(dealer_hand, dealer_cards_dealt);
    if (dealer_value >= DEALER_MIN_SCORE && dealer_value <= SCORE)
      quit = 1;

    while (!quit && dealer_value < SCORE && dealer_cards_dealt < MAX_IN_HAND)
    {

      deal_one_card(deck, &dealer_hand[dealer_cards_dealt++]);
      display_hand("| Dealer's hand is", dealer_hand, dealer_cards_dealt);
      dealer_value = check_hand(dealer_hand, dealer_cards_dealt);

      if (dealer_value >= DEALER_MIN_SCORE && dealer_value <= SCORE)
        quit = 1;
    }

    if (dealer_value > SCORE)
      printf("| *** Dealer busts!\n");

    /*** determine winner ***/
    printf("|\n| *** Dealer: %d  Player: %d  - ", dealer_value, player_value);

    /* if the player busts, or
       if the dealer is 21 or under and the dealers hand is greater than the players hand,
       then dealer wins the hand */
    if ((player_value > SCORE) || (dealer_value > player_value && dealer_value <= SCORE))
    {
      printf("Dealer Wins! ***\n");
      winner = DEALER;
    }

    /* if dealer and player hold the same point value, no one wins */
    else if (dealer_value == player_value)
    {
      printf("Push - no winners! ***\n");
      winner = DRAW;
    }

    /* otherwise, the player wins */
    else
    {
      printf("Player Wins! ***\n");
      winner = PLAYER;
    }

  } /* end else if */

  printf("---------------------- END GAME ------------------------\n");

  return winner;
}

/* Determines whether string contains only digits */
static int is_number(char *number)
{
  int digit = 1;

  if (strlen(number) == 0)
    return 0;

  while (*number != '\0')
  {
    if (!isdigit(*number))
      digit = 0;

    number++;
  }
  return digit;
}
