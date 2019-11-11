

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>


int main()
{
	printf("TESTING: BARON\n");

	// set your card array
	int k[10] = { adventurer, council_room, feast, gardens, mine
					, remodel, smithy, village, baron, great_hall };

	// declare the game state
	struct gameState G, g1;
	
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
		int choice1 = 1;
		int choice2 = 0;
		int bonus = 0;

		/* Positive Tests */

		// set player's values to known amounts
		G.coins = 0;

		// Scenario 1: where player has exactly 1 estate card
		int estateCount = 0;
		int i;
		for (i = 0; i < G.handCount[curPlayer]; i++)
		{
			if (G.hand[curPlayer][i] == estate)
			{
				if (estateCount == 0)
				{
					estateCount++;
				}
				else if (estateCount > 0)
				{
					G.hand[curPlayer][i] = duchy;
				}
			}
		}

		// copy state of game for later tests
		memcpy(&g1, &G, sizeof(struct gameState));

		// call function with set values
		baronEffect(curPlayer, choice1, choice2, &G, handpos, &bonus);

		// check if amount is what we expected
		if (G.coins == 4)
		{
			printf("BARON TEST: 4 COINS ADDED SUCCESS!\n");
		}
		else
		{
			printf("BARON TEST: 4 COINS ADDED !FAILED!\n Current coin amount: %d\n Expected: 4\n", G.coins);
		}


		// Sscenario 2: user has no estate cards but chooses to discard one anyway
		// (choice1 = 1)
		for (i = 0; i < G.handCount[curPlayer]; i++)
		{
			if (G.hand[curPlayer][i] == estate)
			{
					// change to something else
					G.hand[curPlayer][i] = duchy;
				
			}
		}
		
		// reset discard pile so we can check if estate added
		for (i = 0; i < G.discardCount[curPlayer]; i++)
		{
			G.discard[curPlayer][i] = 0;
		}

		// run baron effect again with new circumstance
		baronEffect(curPlayer, choice1, choice2, &G, handpos, &bonus);


		// check if current player now has estate and same amount of coins
		estateCount = 0;
		for (i = 0; i < G.discardCount[curPlayer]; i++)
		{
			if (G.discard[curPlayer][i] == estate)
			{
				estateCount++;

			}
		}
		// Expected outcome: gain estate
		if (estateCount == 1)
		{
			printf("GAIN ESTATE TEST PASSED!\n");
		}
		else
		{
			printf("GAIN ESTATE TEST FAILED. ESTATE COUNT: %d\n", estateCount);
		}

		// Test Scenario 3: test branch where user chooses to not discard an estate
		// (choice1 = 0)
		// expected result: gain an estate
		choice1 = 0;
		baronEffect(curPlayer, choice1, choice2, &G, handpos, &bonus);


		estateCount = 0;
		for (i = 0; i < G.discardCount[curPlayer]; i++)
		{
			if (G.discard[curPlayer][i] == estate)
			{
				estateCount++;

			}
		}

		if (estateCount == 2)
		{
			printf("GAIN ESTATE BY CHOICE TEST PASSED!\n");
		}
		else
		{
			printf("GAIN ESTATE BY CHOICE TEST FAILED. ESTATE COUNT: %d\n", estateCount);
		}



		// Test Scenario: Edge case - Supply has exactly 1 estate left
		g1.supplyCount[estate] = 1;
		g1.coins = 0;
		choice1 = 1; // choose to discard estate
		choice2 = 0;

		// actually does not have estate
		g1.hand[curPlayer][0] = copper;
		g1.hand[curPlayer][1] = ambassador;
		g1.hand[curPlayer][2] = province;
		g1.hand[curPlayer][3] = mine;
		g1.hand[curPlayer][4] = duchy;

		// call function with set values
		r = baronEffect(curPlayer, choice1, choice2, &g1, handpos, &bonus);

		if (g1.supplyCount[estate] == 0)
		{
			printf("BARON ESTATE SUPPLY DECREMENT PASSED\n");
		}
		else
		{
			printf("BARON ESTATE SUPPLY DECREMENT !!FAILED!!\n SUPPLY AMOUNT: %d\n", g1.supplyCount[estate]);

		}

		printf("END OF BARON TESTS\n");

	}
    
    return 0;
}