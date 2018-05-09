#include "Profiny.h"

/*
 * f() is a normal function
 */
int f(int n)
{
	PROFINY_SCOPE
	int result = 1;
	for (int i=1; i<n; i++)
	{
		result *= i;
	}
	return result;
}

/*
 * g() is a recursive function. In FLAT mode, g(9) will be counted as one call
 * even though it calls itself recursively for multiple times.
 */
int g(int n)
{
	PROFINY_SCOPE
	if (n < 2)
		return 1;
	return g(n-1) * n;
}

/*
 * h1() and h2() calls each other to form recursive calls. In FLAT mode, the
 * call to h1(9) will yield call count 1 to h1() and h2().
 */
int h2(int n);

int h1(int n)
{
	PROFINY_SCOPE
	if (n < 2)
		return 1;
	return h2(n-1) * n;
}

int h2(int n)
{
	PROFINY_SCOPE
	if (n < 2)
		return 1;
	return h1(n-1) * n;
}

int main()
{
	PROFINY_SCOPE
	SET_OMIT_RECURSIVE_CALLS(false)

	// call f() two time. The call counting should be 2
	f(1000000000);
	f(100);

	g(9);
	h1(9);
	return 0;
}
