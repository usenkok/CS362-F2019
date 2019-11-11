#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int main()
{
	printf("TESTING MINEEFFECT()\n");

	// set your card array
	int k[10] = { adventurer, council_room, feast, gardens, mine
					, remodel, smithy, village, baron, great_hall };

	// declare the game state
	struct gameState G;
	// set up variables for function to be called
	int handpos = 0;
	int curPlayer = 0;
	int choice1 = 0;
	int choice2 = 0;
	int bonus = 0;
	int i, j, r, p;
	int seed = 1000;

	enum CARD card = mine;

	// set up random numbers
	SelectStream(2);
	PutSeed(3);

	// set r to known value
	r = -5;

	// initialize game
	r = initializeGame(2, k, seed, &G);
	//printf("value of r: %d\n", r);
	if (r < 0)
	{
		printf("!!!GAME INITIALIZATION FAILED!!!\n");
	}
	else
	{
		G.hand[curPlayer][0] = copper;
		G.hand[curPlayer][1] = village;
		G.hand[curPlayer][2] = silver;
		G.hand[curPlayer][3] = adventurer;
		G.hand[curPlayer][4] = duchy;

		G.handCount[curPlayer] = 5;

		/*Positive Tests*/
		// pass a valid card to trash and valid card to gain (within 3 of trashed card value)
		choice1 = 0; // index of copper within hand
		choice2 = silver; // 0+3 is not > 3 so should be able to make this swap


		// check that current player no longer has copper in hand
		int copperPresent = 0;
		for (i = 0; i < G.handCount[curPlayer]; i++)
		{
			if (G.hand[curPlayer][i] == copper)
			{
				copperPresent++;
			}
		}

		// check and print result
		if (copperPresent == 1)
		{
			printf("TRASH CARD FROM HAND FAILED\n");
		}
		else
		{
			printf("TRASH CARD FROM HAND PASSED\n");
		}


		// check that current player now has silver in their hand
		int silverPresent = 0;
		for (i = 0; i < G.handCount[curPlayer]; i++)
		{
			if (G.hand[curPlayer][i] == silver)
			{
				silverPresent++;
			}
		}
		if (silverPresent == 1)
		{
			printf("GAIN CARD IN HAND PASSED\n");
		}
		else
		{
			printf("GAIN CARD IN HAND FAILED\n    SILVER COUNT: %d",silverPresent);
		}


		/* Negative Tests */

		// test sending card to trash that is not a treasure card
		choice1 = 3; // adventurer = 7
		r = -5;
		r = mineEffect(curPlayer, card, choice1, choice2, &G, handpos, &bonus);

		// expect -1 returned
		if (r == -1)
		{
			printf("MINE TEST INVALID TREASURE CARD PASSED\n");
		}
		else
		{
			printf("MINE TEST INVALID TREASURE CARD FAILED\n    Returned result: %d\n", r);
		}

		// test passing invalid choice2
		choice1 = 0;  // choice1 is now valid, it is copper
		choice2 = 30; // largest card value is 26
		
		r = -5;
		r = mineEffect(curPlayer, card, choice1, choice2, &G, handpos, &bonus);
		// expect -1 returned
		if (r == -1)
		{
			printf("MINE TEST INVALID SUPPLY REQUEST CARD PASSED\n");
		}
		else
		{
			printf("MINE TEST INVALID SUPPLY REQUEST CARD FAILED\n    Returned result: %d\n", r);
		}

		// test choosing card whose value is > 3 more than trashed card
		choice1 = 0; // keep as copper, cost value of 0.

		G.hand[curPlayer][2] = gold;

		choice2 = gold; // value of gold is 6

		r = -5;
		r = mineEffect(curPlayer, card, choice1, choice2, &G, handpos, &bonus);
		// expect -1 returned
		if (r == -1)
		{
			printf("MINE TEST TOO LARGE TREASURE REQUEST PASSED\n");
		}
		else
		{
			printf("MINE TEST TOO LARGE TREASURE REQUEST FAILED\n    Returned result: %d\n", r);
		}


	}



	return 0;
}