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
periodTaskDriver(int argc, char *argv[])
{

	// Doing basic checks on the inputs.
	FILE *periodicJobFile = firstCheck(argc, argv);
	

	int numLines;
	// Taking in input for periodic tasks.
	Task *tasks = periodicTaskInput(periodicJobFile, &numLines);

	// Checking the CPU utilisation.
	checkCpuUtilisation(tasks, numLines);

	// Finding the hyperperiod.
	int hyperperiod = findHyperPeriod(tasks, numLines);

	
	// int condition1Size, *condition2Sizes, *condition3Sizes, condition1Index, reallocSize;
	// conditionChecker(tasks, numLines, hyperperiod, &condition1Size, condition2Sizes, condition3Sizes, &condition1Index, &reallocSize);
	
	//
	printf("\n");
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

	int condition1Index = condition1Index1 < condition1Index2 ? condition1Index1 : condition1Index2;
	printf("Possible frame size(s) based on the three conditions: ");
	for (int i = condition1Index+1; i < reallocSize; ++i)
	{
		printf("%d ", condition3Sizes[i]);
	}
	printf("\n");
	//

	
	// Splitting the periodic tasks if required.
	splitTasks(reallocSize, condition1Index, condition3Sizes, tasks, numLines);

	// To print information about tasks.
	// printTaskInfo(tasks, numLines);

	
	int numJobs = findNumJobs(tasks, numLines, hyperperiod);
	

	// printf("numJobs=%d\n", numJobs);

	// Creating and initialising task instances.
	TaskInstance *jobs = (TaskInstance *) malloc(sizeof(TaskInstance) * numJobs);
	createTaskInstances(tasks, jobs, condition3Sizes[condition1Index], hyperperiod, numLines, numJobs);

	// To print information about jobs.
	// printJobInfo(jobs, numJobs);

	// Creating and initialising frames.
	int frameSize = condition3Sizes[condition1Index];
	int numFrames = hyperperiod / frameSize;
	printf("frameSize=%d. Trying to create a schedule\nnumFrames=%d\n", frameSize, hyperperiod / frameSize);
	Frame *frames = (Frame *) malloc(sizeof(Frame) * (hyperperiod / frameSize));
	calculateSchedule(jobs, numJobs, frameSize, hyperperiod, frames);

	// To print information about frames.
	// printFrameInfo(frames, numFrames);

	// To store the information about frames.
	storeFrameInfo(frames, numFrames, frameSize);


	// Freeing data.
	free(condition2Sizes);
	free(condition3Sizes);

	for (int i = 0; i < numFrames; ++i)
	{
		free(frames[i].jobs);
	}
	free(frames);

	for (int i = 0; i < numLines; ++i)
	{
		free(tasks[i].splits);
	}
	free(tasks);

	
	return 0;
}