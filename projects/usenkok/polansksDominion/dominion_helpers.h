#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state,
                int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3,
               struct gameState *state, int handPos, int *bonus);

int isTreasureCard(int card);
int isVictoryCard(int card);

// Ambassador functions
int playAmbassador(int choice1, int choice2, struct gameState *state, int handPos);
int validateAmbassadorChoices(int choice1, int choice2, struct gameState *state, int handPos);

// Mine functions
int playMine(int choice1, int choice2, struct gameState *state, int handPos);
int validateMineChoices(int choice1, int choice2, struct gameState *state);

// Minion functions
int playMinion(int choice1, int choice2, struct gameState *state, int handPos);
void discardHandAndRedraw(struct gameState *state, int handPos, int numCardsToRedraw, int player);

// Tribute functions
int playTribute(struct gameState *state);
void setTributeRevealedCards(struct gameState *state, int *tributeRevealedCards);

// Baron functions
int playBaron(int choice1, struct gameState *state);
void discardEstateForBaron(struct gameState *state, int cardPos);
void gainEstateForBaron(struct gameState *state);




#endif
