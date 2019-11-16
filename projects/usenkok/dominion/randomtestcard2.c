
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>



int main()
{

	printf("\nTESTING MINION\n\n");

	// set your card array
	int k[10] = { adventurer, council_room, feast, gardens, mine
					, remodel, smithy, village, baron, great_hall };

	// declare the game state
	struct gameState G, g1;

	int i, j, r;
	int seed = 440;


	int addTwoActionsSuccessCount = 0;
	int addTwoActionsFailureCount = 0;

	int discard4SuccessCount = 0;
	int discard4FailureCount = 0;

	int curPlayerRedrawHandSize4SuccessCount = 0;
	int curPlayerRedrawHandSize4FailureCount = 0;

	int nextPlayerRedrawHandSize4SuccessCount = 0;
	int nextPlayerRedrawHandSize4FailureCount = 0;

	// set up random numbers
	SelectStream(2);

	PutSeed(3);

	// set r to known value
	r = -5;

	// initialize game
	r = initializeGame(2, k, seed, &G);
	
	if (r < 0)
	{
		printf("!!!GAME INITIALIZATION FAILED!!!\n");
	}
	else
	{
		// set up variables for function to be called
		int handpos = 0;
		int curPlayer = 0;
		int choice1 = 1;
		int choice2 = 0;
		int bonus = 0;

		// randomized tests
		// set up random hand, deck and discard sizes for each player
		int numPlayers = 2;
		int k;
		for (k = 0; k < 1000; k++)
		{
			for (i = 0; i < numPlayers; i++)
			{
				G.deckCount[i] = floor(Random() * MAX_DECK);
				G.handCount[i] = floor(Random() * MAX_HAND);
				G.discardCount[i] = floor(Random() * MAX_DECK);
			}

			// now generate random cards for each set
			for (i = 0; i < numPlayers; i++) {
				for (j = 0; j < G.deckCount[i]; j++)
				{
					G.deck[i][j] = floor(Random() * 26);
				}
				for (j = 0; j < G.handCount[i]; j++)
				{
					G.hand[i][j] = floor(Random() * 26);
				}
				for (j = 0; j < G.discardCount[i]; j++)
				{
					G.discard[i][j] = floor(Random() * 26);
				}
			}

			G.coins = floor(Random() * 10);
			G.numActions = floor(Random() * 10); 


			//printf("Player %d has %d estate cards\n", curPlayer, curPlayerEstates);


			// randomize who we are playing and what choices we get
			curPlayer = floor(Random() * 2);
			handpos = floor(Random() * 2);
			choice1 = floor(Random() * 2);
			choice2 = floor(Random() * 2);
			int nextPlayer;

			if (curPlayer == 0)
			{
				nextPlayer = 1;
			}
			else 
			{
				nextPlayer = 0;
			}

			/*printf("player: %d\n", curPlayer);
			printf("handPos: %d\n", handpos);
			printf("choice1: %d\n", choice1);
			printf("choice2: %d\n", choice2);
			*/


			// copy struct for comparison later
			memcpy(&g1, &G, sizeof(struct gameState));

			// play Minion and see what we get
			minionEffect(curPlayer, 0, choice1, choice2, &G, handpos, &bonus);


			if (choice1 == 1)
			{	// if first choice selected, increase actions by 2
				if (G.numActions - 2 == g1.numActions)
				{
					addTwoActionsSuccessCount++;
				}
				else
				{
					addTwoActionsFailureCount++;
				}
			}
			else if (choice2 == 1)
			{
				// check that all cards were discarded and only 4 currently in hand
				if (G.handCount[curPlayer] == 4)
				{
					curPlayerRedrawHandSize4SuccessCount++;
				}
				else
				{
					curPlayerRedrawHandSize4FailureCount++;
				}

				// check if not current player had a hand size > 4
				if (g1.handCount[nextPlayer] > 4)
				{
					// check if all cards were discarded and only 4 currently in hand
					if (G.handCount[nextPlayer] == 4)
					{
						nextPlayerRedrawHandSize4SuccessCount++;
					}
					else

					{
						nextPlayerRedrawHandSize4FailureCount++;
					}
					
				}
			}
		}
		

		printf("*******results*******\n\n");

		printf("addTwoActionsSuccessCount: %d\n", addTwoActionsSuccessCount);
		printf("addTwoActionsFailureCount: %d\n", addTwoActionsFailureCount);

		printf("discard4SuccessCount: %d\n", discard4SuccessCount);
		printf("discard4FailureCount: %d\n", discard4FailureCount);

		printf("addTwoActionsSuccessCount: %d\n", curPlayerRedrawHandSize4SuccessCount);
		printf("addTwoActionsFailureCount: %d\n", curPlayerRedrawHandSize4FailureCount);

		printf("nextPlayerRedrawHandSize4SuccessCount: %d\n", nextPlayerRedrawHandSize4SuccessCount);
		printf("nextPlayerRedrawHandSize4FailureCount: %d\n", nextPlayerRedrawHandSize4FailureCount);


	}

	return 0;
}