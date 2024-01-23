#include "mergesort.h"

void merge(int *a, int s1, int s2)
{
	int *tmp;
	int i = 0, j = 0, k = 0;
	tmp = new int[s1 + s2];

	while (i < s1)
	{
		tmp[i] = a[i];
		i++;
	}

	j = s1 + s2 - 1;

	while (i < s1 + s2)
	{
		tmp[i] = a[j];
		i++;
		j--;
	}

	i = 0;
	j = s1 + s2 - 1;

	for (k = 0; k < s1 + s2; k++)
	{
		a[k] = (tmp[i] < tmp[j]) ? tmp[i++] : tmp[j--];
	}

	delete[] tmp;
}

void mergesort2(int* a, unsigned p, unsigned r) {
    if (p < r) {
        int q = (p + r) / 2;
        mergesort2(a, p, q);
        mergesort2(a, q + 1, r);
        merge(a + p, q - p + 1, r - q);
    }
}

void mergesort(int* a, unsigned p, unsigned r) {
	mergesort2(a,p,r-1);
}
