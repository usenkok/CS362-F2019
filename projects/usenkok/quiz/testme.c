#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	// create pre-set array of valid and invalid characters that testme() is looking for
	char *charsForTesting = "[]1()b{} ax.Z";

	int charIndex = 0;

	int minNbr = 0;
	int maxNbr = 14;
	
	// return a character that is a random index of the pre-defined set to choose from;
	charIndex = (rand() % (maxNbr - minNbr + 1)) + minNbr;
	
    return charsForTesting[charIndex];;
}

char *inputString()
{
	char randomString[5]; // create string of fixed size so we can edit each character

	char* randomStrPtr = randomString; // create pointer to return value

	char* inputPool = "rrstteerrsteeet"; // pre-defined set of characters to choose from

	int randomStringSize = 6;
	int randomNbr = 0;
	int i;
	int minNbr = 0;
	int maxNbr = 15;

	for (i = 0; i < randomStringSize - 1; i++)
	{
		randomNbr = (rand() % (maxNbr - minNbr + 1)) + minNbr;
		
		randomString[i] = inputPool[randomNbr];
	}

	randomString[randomStringSize] = '\0';

    return randomStrPtr;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
	testme();
    return 0;
}
