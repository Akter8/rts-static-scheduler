#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "function.h"


int
splitTasks(int reallocSize, int condition1Index, int *condition3Sizes, Task *tasks, int numTasks)
{
	printf("reallocSize=%d condition1Index=%d\n", reallocSize, condition1Index);
	if (reallocSize >= condition1Index+1)
	{
		printf("Have to slice the tasks as there is no frame size fitting the task set.\n");

		int numOfJobsToSlice = 0;
		int *indicesOfJobsToSlice = (int *) malloc(sizeof(int) * reallocSize + 1);

		// Finding the number of jobs that need to be sliced at min.
		for (int i = 0; i < numTasks; ++i)
		{
			if(tasks[i].wcet > condition3Sizes[condition1Index])
			{
				indicesOfJobsToSlice[numOfJobsToSlice] = i;
				numOfJobsToSlice++;
			}
		}

		printf("Trying to slice %d task(s).\nTask(s) being split:", numOfJobsToSlice);
		for (int i = 0; i < numOfJobsToSlice; ++i)
		{
			printf(" %d", indicesOfJobsToSlice[i]);
		}
		printf("\n");


		// Calculating the splits for the jobs that need to be split.
		for (int i = 0; i < numOfJobsToSlice; ++i)
		{
			tasks[indicesOfJobsToSlice[i]].numOfSplits = (tasks[indicesOfJobsToSlice[i]].wcet -1) / condition3Sizes[condition1Index];

			tasks[indicesOfJobsToSlice[i]].splits = (int *) malloc(sizeof(int) * (tasks[indicesOfJobsToSlice[i]].numOfSplits + 1));
			
			printf("Task-%d with wcet=%0.1f being split into=%d splits and to: ", indicesOfJobsToSlice[i], tasks[indicesOfJobsToSlice[i]].wcet, tasks[indicesOfJobsToSlice[i]].numOfSplits);
			
			for (int j = 0; j < tasks[indicesOfJobsToSlice[i]].numOfSplits + 1; ++j)
			{
				tasks[indicesOfJobsToSlice[i]].splits[j] = condition3Sizes[condition1Index];
				printf("%d ", condition3Sizes[condition1Index]);
			}

			int remOfSplit = (int)tasks[indicesOfJobsToSlice[i]].wcet % condition3Sizes[condition1Index];

			if (remOfSplit != 0)
			{
				tasks[indicesOfJobsToSlice[i]].splits[tasks[indicesOfJobsToSlice[i]].numOfSplits] = remOfSplit;
				printf("%d", remOfSplit);
			}
			printf("\n");
		}
	}

	// return the frame size.
	return -1; // placeholder.
}