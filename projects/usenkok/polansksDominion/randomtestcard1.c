
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>



int getEstateCardCount(struct gameState *s, int player) {
	int c = 0;
	int i;
	for (i = 0; i < s->handCount[player]; i++) {
		if (s->hand[player][i] == estate) {
			c++;
		}
	}

	return c;
}

int main()
{

	printf("\nTESTING BARON\n\n");
	// int playBaron(int choice1, struct gameState *state)

	// set your card array
	int k[10] = { adventurer, council_room, feast, gardens, mine
					, remodel, smithy, village, baron, great_hall };

	// declare the game state
	struct gameState G, g1;

	int i, j, r;
	int seed = 440;

	int successPlayEstateCount = 0;
	int failurePlayEstateCount = 0;

	int playEstateSupplySuccess = 0;
	int playEstateSupplyFailure = 0;

	int playEstateHandSuccess = 0;
	int playEstateHandFailure = 0;

	int successNegativePlayEstateCount = 0;
	int failureNegativePlayEstateCount = 0;

	int successDontPlayEstateCount = 0;
	int failureDontPlayEstateCount = 0;

	int dontPlayEstateSupplySuccess = 0;
	int dontPlayEstateSupplyFailure = 0;


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

			int curPlayerEstates = getEstateCardCount(&G, curPlayer);
			//printf("Player %d has %d estate cards\n", curPlayer, curPlayerEstates);


			// randomize who we are playing and what choices we get
			curPlayer = floor(Random() * 2);
			handpos = floor(Random() * 2);
			choice1 = floor(Random() * 2);
			choice2 = floor(Random() * 2);

			/*printf("player: %d\n", curPlayer);
			printf("handPos: %d\n", handpos);
			printf("choice1: %d\n", choice1);
			printf("choice2: %d\n", choice2);
			*/


			// copy struct for comparison later
			memcpy(&g1, &G, sizeof(struct gameState));

			// play Baron and see what we get
			// baronEffect(curPlayer, choice1, choice2, &G, handpos, &bonus);
			playBaron(choice1, &G);

			if (curPlayerEstates > 0 && choice1 == 1)
			{	// play estate and gain 4 coins
				if (g1.coins + 4 == G.coins)
				{
					successPlayEstateCount++;
				}
				else
				{
					failurePlayEstateCount++;
				}
				// play estate card, expect supply to decrease (so previous supply larger)
				if (g1.supplyCount[estate] == (G.supplyCount[estate] + 1))
				{
					playEstateSupplySuccess++;
				}
				else
				{
					playEstateSupplyFailure++;
				}
				// play estate and expect hand decrease
				if (G.handCount[curPlayer] == g1.handCount[curPlayer] - 1)
				{
					playEstateHandSuccess++;
				}
				else
				{
					playEstateHandFailure++;
				}

			}
			// Handle case where player chooses to discard even though they don't have an estate card
			if (curPlayerEstates == 0 && choice1 == 1)
			{   // should not have changed coin amount
				if (G.coins == g1.coins)
				{
					successNegativePlayEstateCount++;
				}
				else
				{
					failureNegativePlayEstateCount++;
				}

			}
			// choice 0 is gain estate do not discard
			if (choice1 == 0)
			{
				// no exchange of coins should happen
				if (G.coins == g1.coins)
				{
					successDontPlayEstateCount++;
				}
				else
				{
					failureDontPlayEstateCount++;
				}
				// supply should go down since player choses to gain card
				if (G.supplyCount[estate] < g1.supplyCount[estate])
				{
					dontPlayEstateSupplySuccess++;
				}
				else
				{
					dontPlayEstateSupplyFailure++;
				}
			}
		}
		
		printf("Success play estate gain 4 coin count: %d\n", successPlayEstateCount);
		printf("Failure play estate gain 4 coin count: %d\n", failurePlayEstateCount);

		printf("Success play estate supply decrease count: %d\n", playEstateSupplySuccess);
		printf("Failure play estate supply decrease count: %d\n", playEstateSupplyFailure);

		printf("Success play estate hand decrease count: %d\n", playEstateHandSuccess++);
		printf("Failure play estate hand decrease count: %d\n", playEstateHandFailure++);

		printf("Success negative play estate count: %d\n", successNegativePlayEstateCount);
		printf("Failure negative play estate count: %d\n", failureNegativePlayEstateCount);


		printf("Success dont play estate count: %d\n", successDontPlayEstateCount);
		printf("Failure dont play estate count: %d\n", failureDontPlayEstateCount);


		printf("Success dont play estate supply decrement: %d\n", dontPlayEstateSupplySuccess);
		printf("Failure dont play estate supply decrement: %d\n", dontPlayEstateSupplyFailure);




	}

	return 0;
}