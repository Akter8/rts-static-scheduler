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
#include "functionPeriodic.h"
#include "configuration.h"

// The global ADTs defined in driverMain.c
extern int numTasks;
extern Task* tasks;
extern TaskInstance *jobs;
extern Frame *frames;


int
periodicTaskDriver(int argc, char **argv)
{

	// Doing basic checks on the inputs.
	firstCheck(argc, argv);
	FILE *periodicJobFile = inputFileCheck(argv[1]);

	FILE *outputFile = fopen(OUTPUT_FILE, "a");
	

	// Taking in input for periodic tasks.
	tasks = periodicTaskInput(periodicJobFile, &numTasks);

	// Sorting the inputs based on their period.
	sortTasks(tasks, 0, numTasks - 1);

	// To print information about tasks.
	printTaskInfo(tasks, numTasks);


	fprintf(outputFile, "----------------------------\n");
	fflush(outputFile);
	// Checking the CPU utilisation.
	checkCpuUtilisation(tasks, numTasks);

	// Finding the hyperperiod.
	int hyperperiod = findHyperPeriod(tasks, numTasks);
	fprintf(outputFile, "Hyperperiod = %d\n", hyperperiod);
	fflush(outputFile);

	// Finding the frame sizes that satisfy the first condition.
	fprintf(outputFile, "\n");
	int condition1Size = frameSizeCondition1(tasks, numTasks);
	fprintf(outputFile, "Condition-1: %d\n", condition1Size);
	fflush(outputFile);

	// Finding the frame sizes that satisfy the second condition.
	int *condition2Sizes = (int *) malloc(sizeof(int) * hyperperiod);
	int reallocSize = frameSizeCondition2(hyperperiod, condition2Sizes);
	condition2Sizes = realloc(condition2Sizes, sizeof(int) * reallocSize);
	fprintf(outputFile, "Condition-2: ");
	int condition1Index1 = -1;
	for (int i = 0; i < reallocSize; ++i)
	{
		fprintf(outputFile, "%d ", condition2Sizes[i]);
		if(condition2Sizes[i] < condition1Size)
			condition1Index1++;
	}
	fprintf(outputFile, "\n");
	fflush(outputFile);


	// Finding the frame sizes that satisfy the third condition.
	int *condition3Sizes = (int *) malloc(sizeof(int) * reallocSize);
	reallocSize = frameSizeCondition3(condition2Sizes, tasks, reallocSize, numTasks, condition3Sizes);
	condition3Sizes = realloc(condition3Sizes, sizeof(int) * reallocSize);
	int condition1Index2 = -1;
	fprintf(outputFile, "Condition-3: ");
	for (int i = 0; i < reallocSize; ++i)
	{
		fprintf(outputFile, "%d ", condition3Sizes[i]);
		if(condition3Sizes[i] < condition1Size)
			condition1Index2++;
	}

	// Finding frame sizes that satisfy all 3 conditions.
	fprintf(outputFile, "\nPossible frame size(s) based on the three conditions (Might be empty if none of them are possible): ");
	for (int i = condition1Index2 + 1; i < reallocSize; ++i)
	{
		fprintf(outputFile, "%d ", condition3Sizes[i]);
	}
	fprintf(outputFile, "\n");
	fprintf(outputFile, "Disclaimer: If multiple frame sizes are possible in the end, the numerically largest one will be chosen as the frame size as that would in a pracital scenario be the frame size that would reduce the amount of scheduling time.\n");
	fflush(outputFile);
	

	// Even if multiple frame sizes are possible, the largest valid one will be chosen as that is the one which will lead to the least number of scheduling points. 
	// Though in this simulation the scheduling time is taken as zero, in all practical applications its not. 

	// INF part.
	// We care only about the condition3Sizes as condition3Sizes are a subset of condition2Sizes.
	// Condition1Size is the one that can be bent around a little if the task instances need to be split. This is automatically taken care of by the INF algorithm in findFrame in splitJobs().
	int frameSize = condition3Sizes[reallocSize - 1];
	int numFrames = hyperperiod / frameSize;

	// Creating task instances.
	int numJobs = findNumJobs(tasks, numTasks, hyperperiod);
	jobs = (TaskInstance *) malloc(sizeof(TaskInstance) * numJobs);
	createTaskInstances(tasks, jobs, frameSize, hyperperiod, numTasks, numJobs);

	// To print information about jobs.
	printJobInfo(jobs, numJobs);

	// Creating frame instances and finding which job goes into which frame.
	frames = (Frame *) malloc(sizeof(Frame) * numFrames);
	// Job splitting might occur if necessary in the below funcion call.
	findFrame(jobs, numJobs, frames, frameSize, numFrames);

	// To print information about frames.
	printFrameInfo(frames, numFrames, frameSize);

	free(jobs);
	// End of INF.
	
	// To store the information about frames into the output file.
	storeFrameInfo(frames, numFrames, frameSize);


	// Closing the file.
	fclose(outputFile);

	// Freeing data.
	free(condition2Sizes);
	free(condition3Sizes);

	for (int i = 0; i < numFrames; ++i)
	{
		free(frames[i].jobs);
	}
	free(frames);
	// Not freeing tasks (Task *) as it will be used later.
	
	return 0;
}