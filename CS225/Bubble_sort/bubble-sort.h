#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

template <typename ForwardIt>
void bubblesort(ForwardIt begin, ForwardIt end)
{
	for (ForwardIt sorted = begin; begin != end; end = sorted)
	{
		sorted = begin;
		for (ForwardIt current = begin, prev = begin; ++current != end; ++prev)
		{
			if (*current < *prev)
			{
				std::iter_swap(current, prev);
				sorted = current;
			}
		}
	}
}

#endif