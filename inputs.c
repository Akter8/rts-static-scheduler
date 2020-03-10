/*
 * Author: Akhil Tarikere
 * Date: 8/3/20
 * 
 * Pre-Condition: 
 * 		
 *
 * Post-Condition:
 *	
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"
#include "structNonPeriodic.h"


Task
*periodicTaskInput(FILE *periodicTaskFile, int *n)
{
	// Taking data from the periodsic job file.
	int numLines;
	fscanf(periodicTaskFile, " %d", &numLines);
	// printf("No. of lines = %d.\n", numLines);

	Task *tasks = (Task *) malloc(sizeof(Task) * numLines);
	int i;
	for (i = 0; i < numLines; i++)
	{
		fscanf(periodicTaskFile, "%d %d %d", &tasks[i].period, &tasks[i].wcet, &tasks[i].deadline);
		// printf("%d %d %d\n", tasks[i].period, tasks[i].wcet, tasks[i].deadline);

		if (tasks[i].wcet > tasks[i].deadline)
		{
			fprintf(stderr, "WCET is greater than deadline. Please input valid data.\n" );
			fclose(periodicTaskFile);
			exit(0);
		}
	}
	fclose(periodicTaskFile);

	// Setting the value of number of lines, so that the function calling this function can use it.
	*n = numLines;

	return tasks;
}


AperiodicJob
*aperiodicJobsInput(FILE *aperiodicJobsFile, int *n)
{
	int numJobs;
	fscanf(aperiodicJobsFile, "%d", &numJobs);

	AperiodicJob *aperiodicJobs = (AperiodicJob *) malloc(sizeof(AperiodicJob) * numJobs);


	for (int i = 0; i < numJobs; ++i)
	{
		fscanf(aperiodicJobsFile, "%f %f", &aperiodicJobs[i].arrivalTime, &aperiodicJobs[i].wcet);
	}

	fclose(aperiodicJobsFile);


	// Setting the value of number of lines, so that the function calling this function can use it.
	*n = numJobs;

	return aperiodicJobs;
}


SporadicJob
*sporadicJobsInput(FILE *sporadicJobsFile, int *n)
{
	int numJobs;
	fscanf(sporadicJobsFile, "%d", &numJobs);

	SporadicJob *sporadicJobs = (SporadicJob *) malloc(sizeof(SporadicJob) * numJobs);

	for (int i = 0; i < numJobs; ++i)
	{
		fscanf(sporadicJobsFile, "%f %f %f", &sporadicJobs[i].arrivalTime, &sporadicJobs[i].wcet, &sporadicJobs[i].deadline);
	}

	fclose(sporadicJobsFile);


	// Setting the value of number of lines, so that the function calling this function can use it.
	*n = numJobs;

	return sporadicJobs;
}