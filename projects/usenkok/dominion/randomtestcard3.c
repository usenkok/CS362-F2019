
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>

#define DEBUG 0


struct outcomeStruct
{
	int outcomes[3][1];
};

int assert(int one, int two, char mode)
{
	switch (mode) {
	case 'e': { if (one == two) return 1;
		return 0; }
		break;
	case 'g': {if (one > two) return 1;
		return 0; }
		break;
	case 'l': {if (one < two) return 1;
		return 0;
		break; }
	}
	// if we get here invalid mode sent
	return 0;
}

char getCardType(enum CARD c)
{
	int num = (int)c;
	if ((num >= 0 && num <= 3) // estate, duchy province
		|| (num == 10)       // gardens
		|| (num == 16)) {	   // great hall
		
		// victory
		return 'v';
	}
	else if (num >= 4 && num <= 6) {
		// treasure
		return 't';
	}
	else {
		return 'a';
	}

}

int assertTributeOutcome(char type, struct gameState *before, struct gameState *after, struct outcomeStruct *tracker)
{
	int returnVal = 0;

	switch (type)
	{
	case 'v': { returnVal = assert(after->handCount[before->whoseTurn], before->handCount[before->whoseTurn] + 2, 'e');
		tracker->outcomes[0][0] += returnVal; //tracker->outcomes[0][0]
		return returnVal;
		break;
		}
	case 't': { returnVal = assert(after->coins, before->coins + 2, 'e');
		tracker->outcomes[1][0] += returnVal;
		return returnVal; }
		break;
			
	case 'a': { returnVal = assert(after->numActions, before->numActions + 2, 'e');
		tracker->outcomes[2][0] += returnVal;
		return returnVal; }

	 break;

	}
	// invalid type sent if we get here so reply 0
	return 0;

}


int main()
{

	printf("\nTESTING TRIBUTE\n\n");

	// set your card array
	int k[10] = { adventurer, council_room, feast, gardens, mine
					, remodel, smithy, village, baron, great_hall };

	// declare the game state
	struct gameState G, g1;

	int i, j, r;
	int seed = 440;
	
	// set up random numbers
	SelectStream(2);

	PutSeed(-1);

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
		//int handpos = 0;
		int curPlayer = 0;
		/*int choice1 = 1;
		int choice2 = 0;*/
		int bonus = 0;
		int numRuns = 10000;

		
		struct outcomeStruct o;
		o.outcomes[0][0] = 0;
		o.outcomes[1][0] = 0;
		o.outcomes[2][0] = 0;

		//outcomes.outcomes[0] = 0;

		/*outcomes[0][0] = 0;
		outcomes[1][0] = 0;
		outcomes[3][0] = 0;*/
		
		

		int numSuccessOutcomes = 0;

		// set up random hand, deck and discard sizes for each player
		int k;
		for (k = 0; k < numRuns; k++)
		{
			// random number of players between 1 and 4
			int numPlayers = floor(Random() * 4) + 1;

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

			int tributeRevealedCards[2] = { -1, -1 };

			// randomize current and next player
			curPlayer = floor(Random() * 4) + 1;

			int nextPlayer = curPlayer + 1;
			
			if (nextPlayer > G.numPlayers) {
				nextPlayer = 0;
			}
			G.whoseTurn = curPlayer;

			// copy struct for comparison later
			memcpy(&g1, &G, sizeof(struct gameState));

			
			tributeEffect(curPlayer, nextPlayer, &tributeRevealedCards, NULL, NULL, NULL, &G, NULL, &bonus);

			char type1 = getCardType(tributeRevealedCards[0]);
			char type2 = getCardType(tributeRevealedCards[1]);

			if (type1 == type2) {
				numSuccessOutcomes += assertTributeOutcome(type1, &g1, &G, &o);
				if (DEBUG) printf("result: %d\n", assertTributeOutcome(type1, &g1, &G, &o));
			}
			else {
				numSuccessOutcomes += assertTributeOutcome(type1, &g1, &G, &o);
				numSuccessOutcomes += assertTributeOutcome(type2, &g1, &G, &o);
			}

			//printf("pause\n");
		}

		printf("Successful results: %d out of %d runs\n", numSuccessOutcomes, numRuns);
		printf("victory card success count: %d\n", o.outcomes[0][0]);
		printf("treasure card success count: %d\n", o.outcomes[1][0]);
		printf("action card success count: %d\n", o.outcomes[2][0]);

	}

	return 0;
}