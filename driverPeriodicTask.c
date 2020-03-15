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

#include "driverPeriodicTask.h"


int
periodTaskDriver(int argc, char *argv[])
{

	// Doing basic checks on the inputs.
	firstCheck(argc, argv);
	FILE *periodicJobFile = inputFileCheck(argv[1]);

	FILE *outputFile = fopen(OUTPUT_FILE, "a");
	

	int numTasks;
	// Taking in input for periodic tasks.
	Task *tasks = periodicTaskInput(periodicJobFile, &numTasks);

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
	
	// int condition1Size, *condition2Sizes, *condition3Sizes, condition1Index, reallocSize;
	// conditionChecker(tasks, numTasks, hyperperiod, &condition1Size, condition2Sizes, condition3Sizes, &condition1Index, &reallocSize);
	
	//
	fprintf(outputFile, "\n");
	int condition1Size = frameSizeCondition1(tasks, numTasks);
	fprintf(outputFile, "Condition-1: %d\n", condition1Size);
	fflush(outputFile);


	int *condition2Sizes = (int *) malloc(sizeof(int) * hyperperiod);
	int reallocSize = frameSizeCondition2(hyperperiod, condition2Sizes);
	condition2Sizes = realloc(condition2Sizes, sizeof(int) * reallocSize);
	fprintf(outputFile, "Condition-2: ");
	int condition1Index1 = -1;
	for (int i = 0; i < reallocSize; ++i)
	{
		fprintf(outputFile, "%d ", condition2Sizes[i]);
		if(condition2Sizes[i] <= condition1Size)
			condition1Index1++;
	}
	fprintf(outputFile, "\n");
	fflush(outputFile);


	int *condition3Sizes = (int *) malloc(sizeof(int) * reallocSize);
	reallocSize = frameSizeCondition3(condition2Sizes, tasks, reallocSize, numTasks, condition3Sizes);
	condition3Sizes = realloc(condition3Sizes, sizeof(int) * reallocSize);
	int condition1Index2 = -1;
	fprintf(outputFile, "Condition-3: ");
	for (int i = 0; i < reallocSize; ++i)
	{
		fprintf(outputFile, "%d ", condition3Sizes[i]);
		if(condition3Sizes[i] <= condition1Size)
			condition1Index2++;
	}

	int condition1Index = condition1Index1 < condition1Index2 ? condition1Index1 : condition1Index2;
	fprintf(outputFile, "Possible frame size(s) based on the three conditions: ");
	for (int i = condition1Index+1; i < reallocSize; ++i)
	{
		fprintf(outputFile, "%d ", condition3Sizes[i]);
	}
	fprintf(outputFile, "\n");
	fflush(outputFile);
	//

	
	// Splitting the periodic tasks if required.
	splitTasks(reallocSize, condition1Index, condition3Sizes, tasks, numTasks);

	// To print information about tasks.
	// printTaskInfo(tasks, numTasks);

	
	int numJobs = findNumJobs(tasks, numTasks, hyperperiod);
	

	// fprintf(outputFile, "numJobs=%d\n", numJobs);

	// Creating and initialising task instances.
	TaskInstance *jobs = (TaskInstance *) malloc(sizeof(TaskInstance) * numJobs);
	createTaskInstances(tasks, jobs, condition3Sizes[condition1Index], hyperperiod, numTasks, numJobs);

	// To print information about jobs.
	printJobInfo(jobs, numJobs);

	// Creating and initialising frames.
	int frameSize = condition3Sizes[condition1Index];
	int numFrames = hyperperiod / frameSize;
	fprintf(outputFile, "frameSize=%d. Trying to create a schedule\nnumFrames=%d\n", frameSize, (int)hyperperiod / frameSize);
	Frame *frames = (Frame *) malloc(sizeof(Frame) * (int)(hyperperiod / frameSize));
	fflush(outputFile);
	calculateSchedule(jobs, numJobs, frameSize, hyperperiod, frames);

	// To print information about frames.
	printFrameInfo(frames, numFrames, frameSize);

	// To store the information about frames.
	storeFrameInfo(frames, numFrames, frameSize);

	fprintf(outputFile, "hyperperiod = %d\n", hyperperiod);
	fflush(outputFile);


	fclose(outputFile);

	// Freeing data.
	free(condition2Sizes);
	free(condition3Sizes);

	for (int i = 0; i < numFrames; ++i)
	{
		free(frames[i].jobs);
	}
	free(frames);

	for (int i = 0; i < numTasks; ++i)
	{
		free(tasks[i].splits);
	}
	free(tasks);

	
	return 0;
}