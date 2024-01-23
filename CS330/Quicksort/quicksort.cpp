#include "quicksort.h"
#include <algorithm>

int partition(int *a, unsigned left, unsigned right)
{
	int pivot = a[left];
	unsigned i = left - 1;
	unsigned j = right + 1;
	while (true) {
		do {
			--j;
		} while ( a[j] > pivot );
		do {
			++i;
		} while ( a[i] < pivot );
		if ( i < j ) {
			std::swap(a[i], a[j]);
		} else {
			return j;
		}
	}
}

void quicksort2(int *a, unsigned left, unsigned right)
{
	if ( left < right ) {
		unsigned pivot = partition(a, left, right);
		quicksort2(a, left, pivot);
		quicksort2(a, pivot+1, right);
	}
}

void quicksort(int *a, unsigned left, unsigned right)
{
	quicksort2(a, left, right-1);
}