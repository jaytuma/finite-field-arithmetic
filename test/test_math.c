#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/lib_math.h"
#include "test_main.h"

int __test_min_max()
{
	srand(time(0));
	
	int a, b;

	a = rand();
	b = rand();

	if(max(a,b) < min(a,b))
	{
		return 0;
	}

	if(max(a,b) != a && max(a,b) != b)
	{
		return 0;	
	}

	if(min(a,b) != a && min(a,b) != b)
	{
		return 0;	
	}
	
	return 1;
}

int main()
{
	MCR_test_iteration_display(__test_min_max(), 1000000,
							   "min/max test:", "", stdout);
}