
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

int main()
{

	printf("TESTING AMBASSADOREFFECT\n");

	// set your card array
	int k[10] = { adventurer, council_room, feast, gardens, mine
					, remodel, smithy, village, baron, great_hall };

	// declare the game state
	struct gameState G, g1, g2;

	int i, p, r;
	int seed = 44;


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

		enum CARD card = ambassador;


		// positive tests
		/* 
		* Scenario: current player chooses to reveal a mine card in their hand. 
		* They have 2 total in their hand. Player 2 has no copies of this card
		* Expected results: 
		*				1. current player has no mine cards in their hand
		*				2. next player has 1 more card in their discard and it is of type 'mine'
						3. Supply of mine cards increases by 1 (cur player -2, +1 next player = +1 to put in supply
		*/

		/* choice1 = hand#, choice2 = number to return to supply */

		// negative test: test that number to return is 0 1 or 2

		// set up game state
		G.hand[curPlayer][0] = gardens;
		G.hand[curPlayer][1] = mine;
		G.hand[curPlayer][2] = duchy;
		G.hand[curPlayer][3] = copper;
		G.hand[curPlayer][4] = mine;

		if (G.handCount[curPlayer] != 5)
		{
			G.handCount[curPlayer] = 5;
		}

		int nextPlayer = curPlayer + 1;

		G.hand[nextPlayer][0] = duchy;
		G.hand[nextPlayer][1] = ambassador;
		G.hand[nextPlayer][2] = copper;
		G.hand[nextPlayer][3] = smithy;
		G.hand[nextPlayer][4] = embargo;

		if (G.handCount[nextPlayer] != 5)
		{
			G.handCount[nextPlayer] = 5;
		}

		// copy game state for further tests later
		memcpy(&g1, &G, sizeof(struct gameState));
		memcpy(&g2, &G, sizeof(struct gameState));


		// choice1 is set to 1
		// choice2 is how many to get rid of
		choice2 = 2;
		
		// set r to known value
		r = -5;


		r = ambassadorEffect(curPlayer, card, choice1, choice2, &G, handpos, &bonus);

		if (r != -1)
		{
			// check expected results
			int mineFound = 0;
			// check that next player has mine in their discard
			
			for (i = 0; i < G.discardCount[nextPlayer]; i++)
			{
				if (G.discard[nextPlayer][i] == mine)
				{
					mineFound = 1;
				}
			}
			if (mineFound == 1)
			{
				printf("AMBASSADOR TEST1 MINE FOUND IN NEXT PLAYER PASSED!\n");
			}
			else
			{
				printf("AMBASSADOR TEST1 MINE FOUND IN NEXT PLAYER !!FAILED!!!\n");
			}

			mineFound = 0;
			// check that current player does not have mine in hand or discard
			for (i = 0; i < G.discardCount[curPlayer]; i++)
			{
				if (G.discard[curPlayer][i] == mine)
				{
					mineFound = 1;
				}
			}
			if (mineFound == 1)
			{
				printf("AMBASSADOR TEST2 MINE FOUND IN CURRENT PLAYER HAND !!FAILED!!!\n");
			}
			else
			{
				printf("AMBASSADOR TEST2 MINE FOUND IN CURRENT PLAYER HAND PASSED\n");
			}

			printf("current player deckCount: %d\n", G.deckCount[curPlayer]);
			printf("current player handCount: %d\n", G.handCount[curPlayer]);
			printf("current player discardCount: %d\n", G.discardCount[curPlayer]);
		}
		else {
			// in its current state the code always lands here because the function is buggy and
			// not comparing the correct attributes
			printf("AMBASSADOR TEST1 DISCARD 2 RETURN VALUE FAILED\nr value: %d\n", r);
		}

		/* Test scenario: player choses a treasure card */
		// using g2
		int notFoundInCurPlayerPassed = 0;
		int notFoundInCurPlayerFailed = 0;
		int foundInNextPlayerPassed = 0;
		int foundInNextPlayerFailed = 0;

		int k;
		for (k = 0; k < 1000; k++)
		{

			int handCount = floor(Random() * MAX_HAND);
			int deckCount = floor(Random() * MAX_DECK);

			// set up hands
			g2.handCount[curPlayer] = handCount;
			int l;// = 0;
			for (l = 0; l < handCount; l++)
			{
				g2.hand[curPlayer][l] = floor(Random() * 26);
			}

			handCount = floor(Random() * MAX_HAND);
			g2.handCount[nextPlayer] = handCount;
			for (l = 0; l < handCount; l++)
			{
				g2.hand[nextPlayer][l] = floor(Random() * 26);

			}

			// set up decks
			g2.deckCount[curPlayer] = deckCount;
			for (l = 0; l < deckCount; l++)
			{
				g2.deck[curPlayer][l] = floor(Random() * 26);
			}

			deckCount = floor(Random() * MAX_DECK);
			g2.deckCount[nextPlayer] = deckCount;
			for (l = 0; l < deckCount; l++)
			{
				g2.deck[nextPlayer][l] = floor(Random() * 26);
			}

			// choose at random a valid card enum (betweeen 1 and 26) to be chosen
			p = floor(Random() * 26);

			// set two spots to have that card
			int spot1 = floor(Random() * g2.handCount[curPlayer]);
			int spot2 = floor(Random() * g2.handCount[curPlayer]);

		
			if (spot1 == spot2)
			{
				spot2++;
				if (spot2 > g2.handCount[curPlayer])
				{
					spot2 -= 3;
				}
			}

			g2.hand[curPlayer][spot1] = p;
			g2.hand[curPlayer][spot2] = p;

			// set second player to not have any of that card
			for (i = 0; i < g2.handCount[nextPlayer]; i++)
			{
				if (g2.hand[nextPlayer][i] == p)
				{
					if (p != 0) // keep within bounds
					{
						g2.hand[nextPlayer][i] = p - 1;
					}
					else
					{
						if (p != 26) // keep within bounds
						{
							g2.hand[nextPlayer][i] = p + 1;
						}
					}
				}
			}

			// set r to known value
			r = -5;
			int z;
			for (z = 0; z < 3; z++)
			{
				choice2 = z;
			
				r = ambassadorEffect(curPlayer, card, choice1, choice2, &g2, handpos, &bonus);

				int cardFound = 0;
			
				// check for amounts and supplies
				for (i = 0; i < g2.discardCount[nextPlayer]; i++)
				{
					if (g2.discard[nextPlayer][i] == p)
					{
						cardFound = 1;
					}
				}
				if (cardFound == 1)
				{
					foundInNextPlayerPassed++;
					//printf("AMBASSADOR TEST%d CARD FOUND IN NEXT PLAYER PASSED! P=%d\n", k, p);
				}
				else
				{
					foundInNextPlayerFailed++;
					//printf("AMBASSADOR TEST%d CARD FOUND IN NEXT PLAYER !!FAILED!!! P=%d\n",k,p);
				}

				cardFound = 0;
				// check that current player does not have mine in hand or discard
				for (i = 0; i < G.discardCount[curPlayer]; i++)
				{
					if (g2.discard[curPlayer][i] == p)
					{
						cardFound = 1;
					}
				}
				if (cardFound == 1)
				{
					notFoundInCurPlayerFailed++;
					//printf("AMBASSADOR TEST%d CARD NOT FOUND IN CURRENT PLAYER HAND !!FAILED!!! P=%d\n",k,p);
				}
				else
				{
					notFoundInCurPlayerPassed++;
					//printf("AMBASSADOR TEST%d CARD NOT FOUND IN CURRENT PLAYER HAND PASSED P=%d\n", k, p);
				}

			}

		}

		
		printf("OUTCOME OF RANDOM CARD CHOICE: \n");
		printf("CARD FOUND IN NEXT PLAYER PASSED: %d\n", foundInNextPlayerPassed);
		printf("CARD FOUND IN NEXT PLAYER FAILED: %d\n", foundInNextPlayerFailed);
		printf("CARD NOT FOUND IN CURRENT PLAYER PASSED: %d\n", notFoundInCurPlayerPassed);
		printf("CARD NOT FOUND IN CURRENT PLAYER FAILED: %d\n", notFoundInCurPlayerFailed);



		/* Negative tests */
		// invalid choice 2 amount


		choice2 = 4;
		r = ambassadorEffect(curPlayer, card, choice1, choice2, &G, handpos, &bonus);
		if (r == -1)
		{
			printf("AMBASSADOR TEST1: INVALID CHOICE2 PASSED\n");
		}
		else
		{
			printf("AMBASSADOR TEST1: INVALID CHOICE2 !!FAILED!!\n  RETURNED VALUE: %d", r);
		}

		ambassadorEffect(curPlayer, card, choice1, choice2, &G, handpos, &bonus);


		
		// pass handpos = choice1
		// expect to get -1 back
		choice1 = 0;

		// reset r to known value
		r = -5;

		r = ambassadorEffect(curPlayer, card, choice1, choice2, &G, handpos, &bonus);
		if (r == -1)
		{
			printf("AMBASSADOR TEST2: INVALID CHOICE1 PASSED\n");
		}
		else
		{
			printf("AMBASSADOR TEST2: INVALID CHOICE1 !!FAILED!!\n  RETURNED VALUE: %d", r);
		}

		ambassadorEffect(curPlayer, card, choice1, choice2, &G, handpos, &bonus);

		
		



	}


	return 0;
}