# Source for make all line || true:
# https://piazza.com/class/k0n6ytspfa4571?cid=70
# comment from Mario Bocaletti about using || true to have make ignore 
# program exists with response != 0
CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
		gcc -o testme testme.c ${CFLAGS}

runTestme: testme
	./testme &> testmeresults.out
	gcov testme.c -f -b >> testmeresults.out
	cat testme.c.gcov >> testmeresults.out

testmeAll: testme.c
	gcc -o testme testme.c ${CFLAGS}
	./testme &> testmeresults.out || true
	gcov testme.c -b >> testmeresults.out
	cat testme.c.gcov >> testmeresults.out

clean: 
	rm -f *.o testme testmeresults.out *.gcda *gcno *.gcov
