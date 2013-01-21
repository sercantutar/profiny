#define PROFINY_CALL_GRAPH_PROFILER
//#define PROFINY_FLAT_PROFILER

#include "Profiny.h"

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

int g(int n)
{
	PROFINY_SCOPE
	if (n < 2)
		return 1;
	return g(n-1) * n;
}

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
	profiny::Profiler::setOmitRecursiveCalls(false);
	f(1000000000);
	g(9);
	h1(9);
	return 0;
}
