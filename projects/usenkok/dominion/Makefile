CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testBaron: randomcardtest1.c dominion.o rngs.o
	gcc -o testBaron -g randomcardtest1.c dominion.o rngs.o $(CFLAGS)

testMinion: randomtestcard2.c dominion.o rngs.o
	gcc -o testMinion -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)

testTribute: randomtestcard3.c dominion.o rngs.o
	gcc -o testTribute -g randomtestcard3.c dominion.o rngs.o $(CFLAGS)


interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)


runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

runBaronTests: testBaron
	./testBaron &> randomtestresults1.out
	gcov dominion.c -f -b >> randomtestresults1.out
	cat dominion.c.gcov >> randomtestresults1.out

runMinionTests: testMinion
	./testMinion &> randomtestresults2.out
	gcov dominion.c -f -b >> randomtestresults2.out
	cat dominion.c.gcov >> randomtestresults2.out


runTributeTests: testTribute
	./testTribute &> randomtestresults3.out
	gcov dominion.c -f -b >> randomtestresults3.out
	cat dominion.c.gcov >> randomtestresults3.out

randomtestresults: testBaron testMinion testTribute
	./testBaron &> randomtestresults.out
	./testMinion >> randomtestresults.out
	./testTribute >> randomtestresults.out
	gcov dominion.c -f -b >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
