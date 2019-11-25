#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>


int main()
{
	printf("TESTING TRIBUTEEFFECT FUNCTION\n");
	// playTribute(state)



	// set your card array
	int k[10] = { adventurer, council_room, feast, gardens, mine
					, remodel, smithy, village, baron, great_hall };

	// declare the game state
	struct gameState G, g1, g2;

	// declare the arrays
	int coppers[MAX_HAND];
	int silvers[MAX_HAND];
	int golds[MAX_HAND];

	int seed = 44;
	int r = -5;
	// initialize game
	r = initializeGame(2, k, seed, &G);
	printf("value of r: %d\n", r);
	if (r < 0)
	{
		printf("!!!GAME INITIALIZATION FAILED!!!\n");
	}
	else
	{
		// set up variables for function to be called
		int handpos = 0;
		int curPlayer = 0;
		int nextPlayer = 1;
		int choice1 = 1;
		int choice2 = 0;
		int bonus = 0;
		int tributeRevealedCards[2] = { -1, -1 };
		enum CARD card = tribute;

		// copy the game state for easy testing same setup
		memcpy(&g1, &G, sizeof(struct gameState));

		memcpy(&g2, &G, sizeof(struct gameState));

		printf("nextPlayer deck count: %d\n", G.deckCount[nextPlayer]);

		// set up next player's cards to drive tributeEffect behavior
		G.deck[nextPlayer][G.deckCount[nextPlayer] - 1] = copper;
		G.deck[nextPlayer][G.deckCount[nextPlayer] - 2] = silver;

		// set coins to known value
		G.coins = 0;

		//tributeEffect(curPlayer, nextPlayer, tributeRevealedCards, card, choice1, choice2, &G, handpos, &bonus);
		playTribute(&G);

		// check for expected outcome -- two treasure = +2 coins
		if (G.coins == 2)
		{
			printf("TRIBUTE TEST1: 2X TREASURE PASSED\n");
		}
		else
		{
			printf("TRIBUTE TEST1: 2X TREASURE !!FAILED!!\n Previous amount: 0\nExpected amount: 2\n Current amount: %d\n", G.coins);
		}


		// set value cards of next player to be victory cards and check for card gain of 2
		g1.deck[nextPlayer][g1.deckCount[nextPlayer] - 1] = duchy;
		g1.deck[nextPlayer][g1.deckCount[nextPlayer] - 2] = estate;

		int currentPlayerHandCount = g1.handCount[curPlayer];

		//tributeEffect(curPlayer, nextPlayer, tributeRevealedCards, card, choice1, choice2, &g1, handpos, &bonus);
		playTribute(&G);

		if (g1.handCount[curPlayer] == currentPlayerHandCount + 2)
		{
			printf("TRIBUTE TEST2: +2 CARD VICTORY CARD TEST PASSED\n");
		}
		else
		{
			printf("TRIBUTE TEST2: +2 CARD VICTORY CARD TEST !!FAILED!!\n Previous Amount: %d\nCurrent card amount: %d\nExpected: %d\n", currentPlayerHandCount, g1.handCount[curPlayer], currentPlayerHandCount+2);
		}

		// test that appropriate amount is awarded for action cards
		// set value cards of next player to be victory cards and check for card gain of 2
		g2.deck[nextPlayer][g2.deckCount[nextPlayer] - 1] = minion;
		g2.deck[nextPlayer][g2.deckCount[nextPlayer] - 2] = ambassador;

		int curActions = g2.numActions;
		//tributeEffect(curPlayer, nextPlayer, tributeRevealedCards, card, choice1, choice2, &g2, handpos, &bonus);
		playTribute(&G);

		if (g2.numActions == curActions + 2)
		{
			printf("TRIBUTE TEST3: ACTION CARD INCREASE +2 PASSED\n");
		}
		else
		{
			printf("TRIBUTE TEST3: ACTION CARD INCREASE +2 !!FAILED!!\n        Prev number of actions: %d\n        Current number of actions: %d\n        Expected: %d\n", curActions, g2.numActions, curActions+2);
		}


		// test scenario the next player has no cards to reveal
		g2.discardCount[nextPlayer] = 0;
		g2.deckCount[nextPlayer] = 0;

		int curActionNum	= g2.numActions;
		int curCoins		= g2.coins;
		int curPlayerCards	= g2.deckCount[curPlayer];

		//tributeEffect(curPlayer, nextPlayer, tributeRevealedCards, card, choice1, choice2, &g2, handpos, &bonus);
		playTribute(&G);

		// make sure numbers did not change

		if (curActionNum == g2.numActions
			&& curCoins == g2.coins
			&& curPlayerCards == g2.deckCount[curPlayer])
		{
			printf("TRIBUTE TEST4: NO TRIBUTE CARDS TO REVEAL TEST PASSED\n");
		}
		else
		{
			printf("TRIBUTE TEST4: NO TRIBUTE CARDS TO REVEAL TEST !!FAILED!!\n");
			if (curCoins == g2.coins)
			{
				printf("    COIN AMOUNT UNCHANGED\n");
			}
			else
			{
				printf("        COIN AMOUNT CHANGED\n");
				printf("          PREVIOUS: %d\n", curCoins);
				printf("          CURRENT: %d\n", g2.coins);
			}
			if (curActionNum == g2.numActions)
			{
				printf("        ACTION AMOUNT UNCHANGED\n");
			}
			else
			{
				printf("        ACTION AMOUNT CHANGED\n");
				printf("          PREVIOUS: %d\n", curActionNum);
				printf("          CURRENT: %d\n", g2.numActions);
			}
			
			if (curPlayerCards == g2.deckCount[curPlayer])
			{
				printf("        CARD AMOUNT UNCHANGED\n");
			}
			else
			{
				printf("       CARD AMOUNT CHANGED\n");
				printf("         PREVIOUS: %d\n", curPlayerCards);
				printf("         CURRENT: %d\n", g2.deckCount[curPlayer]);
			}
		}
			
			
			
		
	}

	return 0;
}