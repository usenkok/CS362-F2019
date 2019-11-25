

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>


int main()
{
	printf("TESTING: MINION\n");
	// playMinion(choice1, choice2, state, handPos);


	// set your card array
	int k[10] = { adventurer, council_room, feast, gardens, mine
					, remodel, smithy, village, baron, great_hall };

	// declare the game state
	struct gameState G, g2;

	// declare the arrays
	/*int coppers[MAX_HAND];
	int silvers[MAX_HAND];
	int golds[MAX_HAND];*/

	int seed = 44;
	int r = -5;

	// initialize game
	r = initializeGame(2, k, seed, &G);
	
	if (r < 0)
	{
		printf("!!!GAME INITIALIZATION FAILED!!!\n");
	}
	else
	{
		/* minion choice1:  1 = +2 coin, 2 = redraw */
		/* choice2: discard hand, redraw 4, other players with 5+ cards discard hand and draw 4 */


		// set up variables for function to be called
		int handpos = 0;
		int curPlayer = 0;
		int choice1 = 1;
		int choice2 = 0;
		int bonus = 0;
		G.coins = 0;

		// first test choice of +2 coins
		// minionEffect(curPlayer, 0, choice1, choice2, &G, handpos, &bonus);
		// playMinion(choice1, choice2, state, handPos);
		playMinion(choice1, choice2, &G, handpos);

		if (G.coins == 2) {
			printf("ADD 2 COINS SUCCESS\n");
		}
		else {
			printf("ADD 2 COINS FAILURE\n");
		}

		printf("CURRENT PLAYER HANDCOUNT: %d\n", G.handCount[curPlayer]);
		printf("CURRENT PLAYER DISCARD COUNT: %d\n", G.discardCount[curPlayer]);
		
		// copy current state to different instance
		
		memcpy(&g2, &G, sizeof(struct gameState));
		

		// now test option choice 2: discard hand and draw 4
		// no other players have hand > 4
		choice1 = 0;
		choice2 = 1;


		int beforeDeckCount = G.deckCount[curPlayer];
		//printf("before deck count: %d\n", beforeDeckCount);
		int afterDeckCount = -1;
		
		G.handCount[curPlayer + 1] = 4;
		G.deckCount[curPlayer + 1] = 5;
		G.discardCount[curPlayer + 1] = 5;
		
		
		//minionEffect(curPlayer, 0, choice1, choice2, &G, handpos, &bonus);
		// playMinion(choice1, choice2, state, handPos);
		playMinion(choice1, choice2, &G, handpos);

		afterDeckCount = G.deckCount[curPlayer];

		if (afterDeckCount == beforeDeckCount-4)
		{
			printf("MINION +4 DECK SUCCESS\n");
		}
		else {
			printf("MINION +4 DECK !!FAILURE!! DECK COUNT: %d\n", afterDeckCount);
		}

		if (G.handCount[curPlayer + 1] == 4)
		{
			printf("HAND LESS THAN 4 SUCCESS\n");
		}
		else
		{
			printf("HAND LESS THAN 4 FAILURE\n HAND COUNT: %d\n", G.handCount[curPlayer+1]);

		}


		// now test option choice 2: discard hand and draw 4
		// other players DO have hand > 4

		g2.hand[1][0] = smithy;
		g2.hand[1][1] = village;
		g2.hand[1][2] = minion;
		g2.hand[1][3] = adventurer;
		g2.hand[1][4] = duchy;

		g2.handCount[1] = 5;

		int secondPlayerDiscardCount		= g2.discardCount[1];
		int secondPlayerDiscardCountAfter	= -1;
		

		//minionEffect(curPlayer, 0, choice1, choice2, &g2, handpos, &bonus);
		playMinion(choice1, choice2, &g2, handpos);

		secondPlayerDiscardCountAfter		= g2.discardCount[1];
		
		if (secondPlayerDiscardCount - secondPlayerDiscardCountAfter == 5)
		{
			printf("5 CARDS DISCARDED SUCCESS\n");
		}
		else
		{
			printf("5 CARDS DISCARDED !FAILURE!\n AMOUNT IN DISCARD: %d\n", secondPlayerDiscardCountAfter);
		}

	}
		

	return 0;
}