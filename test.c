/*
 * Author: Akhil Tarikere
 * Date: 6/3/20
 * 
 * Pre-Condition: CLI argument of Periodic job 
 * 		file name containing the test set.
 *
 * Post-Condition: The hyperperiods and the 
 *		frame sizes that fit this tast set.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct.h"
#include "function.h"


int
main(int argc, char const *argv[])
{
	// Test to see if user has entered the file name or not.
	if (argc != 2)
	{
		fprintf(stderr, "Please enter valid arguments. Program exiting.\n");
		goto end;
	}

	char *periodicJobFileName = argv[1];
	printf("The file entered is: %s\n", periodicJobFileName);

	FILE *periodicJobFile = fopen(periodicJobFileName, "r");
	// Checking if there is an error.
	if (!periodicJobFile)
	{
		fprintf(stderr, "Could not open file: %sProgram exiting.\n", periodicJobFileName);
		goto end;
	}

	// Taking data from the periodsic job file.
	int numLines;
	fscanf(periodicJobFile, " %d", &numLines);
	printf("No. of lines = %d.\n", numLines);

	Task *tasks = (Task *) malloc(sizeof(Task) * numLines);
	int i;
	for (i = 0; i < numLines; i++)
	{
		fscanf(periodicJobFile, "%d %d %d", &tasks[i].period, &tasks[i].wcet, &tasks[i].deadline);
		// printf("%d %d %d\n", tasks[i].period, tasks[i].wcet, tasks[i].deadline);

		if (tasks[i].wcet > tasks[i].deadline)
		{
			fprintf(stderr, "WCET is greater than deadline. Please input valid data.\n" );
			fclose(periodicJobFile);
			goto free;
		}
	}
	fclose(periodicJobFile);

	float cpuUtilisation = calculateCpuUtilisation(tasks, numLines);
	if (cpuUtilisation < 1.0f)
	{
		printf("CPU Utilisation < 1: might be schedulable.\n");
	}
	else
	{
		fprintf(stderr, "CPU Utilisation >= 1: cannot be scheduled\n");
		goto free;
	}


	int hyperperiod = findHyperPeriod(tasks, numLines);
	printf("The hyperperiod is: %d.\n", hyperperiod);


	int condition1Size = frameSizeCondition1(tasks, numLines);
	printf("Condition-1: %d\n", condition1Size);


	int *condition2Sizes = (int *) malloc(sizeof(int) * hyperperiod);
	int reallocSize = frameSizeCondition2(hyperperiod, condition2Sizes);
	condition2Sizes = realloc(condition2Sizes, sizeof(int) * reallocSize);
	printf("Condition-2: ");
	int condition1Index1 = -1;
	for (int i = 0; i < reallocSize; ++i)
	{
		printf("%d ", condition2Sizes[i]);
		if(condition2Sizes[i] <= condition1Size)
			condition1Index1++;
	}
	printf("\n");


	int *condition3Sizes = (int *) malloc(sizeof(int) * reallocSize);
	reallocSize = frameSizeCondition3(condition2Sizes, tasks, reallocSize, numLines, condition3Sizes);
	condition3Sizes = realloc(condition3Sizes, sizeof(int) * reallocSize);
	int condition1Index2 = -1;
	printf("Condition-3: ");
	for (int i = 0; i < reallocSize; ++i)
	{
		printf("%d ", condition3Sizes[i]);
		if(condition3Sizes[i] <= condition1Size)
			condition1Index2++;
	}
	printf("\n");

	int condition1Index = condition1Index1 < condition1Index2 ? condition1Index1 : condition1Index2;
	printf("Possible frame size(s) based on the three conditions: ");
	for (int i = condition1Index+1; i < reallocSize; ++i)
	{
		printf("%d ", condition3Sizes[i]);
	}
	printf("\n");


	int numTasks = numLines;

	// Check to see if jobs need to be sliced.
	printf("reallocSize=%d condition1Index=%d\n", reallocSize, condition1Index);
	if (reallocSize >= condition1Index+1)
	{
		printf("Have to slice the tasks as there is no frame size fitting the task set.\n");

		int numOfJobsToSlice = 0;
		int *indicesOfJobsToSlice = (int *) malloc(sizeof(int) * reallocSize + 1);

		// Finding the number of jobs that need to be sliced at min.
		for (int i = 0; i < numLines; ++i)
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
			
			printf("Task-%d with wcet=%d being split into=%d splits and to: ", indicesOfJobsToSlice[i], tasks[indicesOfJobsToSlice[i]].numOfSplits, tasks[indicesOfJobsToSlice[i]].wcet);
			
			for (int j = 0; j < tasks[indicesOfJobsToSlice[i]].numOfSplits + 1; ++j)
			{
				tasks[indicesOfJobsToSlice[i]].splits[j] = condition3Sizes[condition1Index];
				printf("%d ", condition3Sizes[condition1Index]);
			}

			int remOfSplit = tasks[indicesOfJobsToSlice[i]].wcet % condition3Sizes[condition1Index];

			if (remOfSplit != 0)
			{
				tasks[indicesOfJobsToSlice[i]].splits[tasks[indicesOfJobsToSlice[i]].numOfSplits] = remOfSplit;
				printf("%d", remOfSplit);
			}
			printf("\n");
		}
	}

	// printTaskInfo(tasks, numLines);

	int numJobs = 0;
	// Calculating the number of total jobs.
	for (int i = 0; i < numLines; ++i)
	{
		if (tasks[i].numOfSplits == 0)
			numJobs += hyperperiod / tasks[i].period;
		else
			numJobs += (hyperperiod / tasks[i].period) * (tasks[i].numOfSplits + 1);
	}

	// printf("numJobs=%d\n", numJobs);

	// Creating numJobs of Task Instances.
	TaskInstance *jobs = (TaskInstance *) malloc(sizeof(TaskInstance) * numJobs);
	createTaskInstances(tasks, jobs, condition3Sizes[condition1Index], hyperperiod, numLines, numJobs);

	printJobInfo(jobs, numJobs);

	int frameSize = condition3Sizes[condition1Index];
	printf("frameSize=%d. Trying to create a schedule\n", frameSize);
	Frame *frames = (Frame *) malloc(sizeof(Frame) * (hyperperiod / frameSize));
	calculateSchedule(jobs, numJobs, frameSize, hyperperiod, frames);

	// printFrameInfo(frames, hyperperiod / frameSize);


	free(condition2Sizes);
	free(condition3Sizes);
free:
	// Closing and freeing data.
	for (int i = 0; i < numLines; ++i)
	{
		free(tasks[i].splits);
	}
	free(tasks);

end:
	return 0;
}