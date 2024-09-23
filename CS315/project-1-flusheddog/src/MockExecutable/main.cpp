#include "SimpleMath.h"
#include "stdio.h"
#include <stdlib.h> 

void Test_add(void)
{
	printf("Test_add\n");
	int r = simple_add(0, 1);
	printf("add(0, 1) = %d\n", r);
}

void Test_sub(void)
{
	printf("Test_sub\n");
	int r = simple_sub(4, 2);
	printf("sub(4, 2) = %d\n", r);
}
void Test_mul(void)
{
	printf("Test_mul\n");
	int r = simple_mul(0, 1);
	printf("mul(0, 1) = %d\n", r);
}
void Test_div(void)
{
	printf("Test_div\n");
	int r = simple_div(4, 2);
	printf("div(4, 2) = %d\n", r);
}

int main(int argc, char **argv)
{
	void (*fp[])(void) = {
		Test_add, //  1
		Test_sub, //  2
		Test_mul, //  3
		Test_div, //  4
	};

	unsigned count = sizeof(fp) / sizeof(*fp);

	unsigned test = 0;

	if (argc > 1)
	{
		test = atoi(argv[1]);
	}

	if (test > count)
	{
		test = 0;
	}

	if (test)
	{
		fp[test - 1]();
	}
	else
	{
		for (unsigned i = 0; i < count; i++)
		{
			fp[i]();
		}
	}

	return 0;
}
