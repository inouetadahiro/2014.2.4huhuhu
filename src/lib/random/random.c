
#include <stdlib.h>

#define p 673
#define q 944

static long a;

void randomize(int n)
{
	a = n;
	a = labs(a % 100000);
}

int random(int n)
{
	a = labs((a * p + q) / 10 % 100000);
	return (int) (a % n);
}

