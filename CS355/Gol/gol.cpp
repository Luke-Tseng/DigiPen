#include "gol.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <pthread.h>

struct thread_data
{
	int x;
	int y;
	std::vector<std::tuple<int, int>> *population;
	bool state; // true if cell is alive, false if dead
	int max_x;
	int max_y;
	pthread_barrier_t* barrier;
};

// Function to count live neighbors for a given cell
int CountNeighbourCell(std::vector<std::tuple<int, int>> population, int x, int y, int max_x, int max_y)
{
	int count = 0;

	// Check all 8 neighbors
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			int new_x = x + i;
			int new_y = y + j;
			if (new_x >= 0 && new_x < max_x && new_y >= 0 && new_y < max_y)
			{
				for (int k = 0; k < static_cast<int>(population.size()); k++)
				{
					int x, y;
					std::tie(x, y) = population[k];
					if (x == new_x && y == new_y)
					{
						count++;
					}
				}
			}
		}
	}

	return count;
}

void *UpdateCell(void *args)
{
	struct thread_data *data;
	data = static_cast<struct thread_data *>(args);

	int count = CountNeighbourCell(*(data->population), data->x, data->y, data->max_x, data->max_y);

	if (data->state == false) // dead
	{
		if (count == 3)
		{
			data->state = true;
		}
	}
	else // alive
	{
		if (count < 2 || count > 3)
		{
			data->state = false;
		}
	}
	pthread_barrier_wait(data->barrier);

	// write new state to population
	if (data->state)
	{
		// check if cell is already in population
		bool found = false;
		for (int i = 0; i < static_cast<int>((*(data->population)).size()); i++)
		{
			int x, y;
			std::tie(x, y) = (*(data->population))[i];
			if (x == data->x && y == data->y)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			(*(data->population)).push_back(std::make_tuple(data->x, data->y));
		}
	}
	else
	{
		// remove cell from population
		for (int i = 0; i < static_cast<int>((*(data->population)).size()); i++)
		{
			int x, y;
			std::tie(x, y) = (*(data->population))[i];
			if (x == data->x && y == data->y)
			{
				(*(data->population)).erase((*(data->population)).begin() + i);
				break;
			}
		}
	}

	pthread_exit(NULL);
}

std::vector<std::tuple<int, int>> run(std::vector<std::tuple<int, int>> initial_population, int num_iter, int max_x, int max_y)
{
	int num_threads = max_x * max_y;
	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, num_threads);

	// Create threads for each cell
	std::vector<pthread_t> threads(num_threads);

	for (int it = 0; it < num_iter; it++)
	{
		// Initialize threads with the initial population
		for (int i = 0; i < num_threads; i++)
		{
			int x = i % max_x;
			int y = i / max_x;
			bool state = false;

			// Check if cell is alive
			for (int j = 0; j < static_cast<int>(initial_population.size()); j++)
			{
				int x_pop, y_pop;
				std::tie(x_pop, y_pop) = initial_population[j];
				if (x_pop == x && y_pop == y)
				{
					state = true;
					break;
				}
			}
			
			struct thread_data *data = new thread_data();
			data->x = x;
			data->y = y;
			data->population = &initial_population;
			data->state = state;
			data->max_x = max_x;
			data->max_y = max_y;
			data->barrier = &barrier;
			pthread_create(&threads[i], NULL, UpdateCell, static_cast<void *>(data));
		}

		// Wait for all threads to finish for this iteration
		for (int i = 0; i < num_threads; i++)
		{
			pthread_join(threads[i], NULL);
		}
	}

	pthread_barrier_destroy(&barrier);

	return initial_population;
}
