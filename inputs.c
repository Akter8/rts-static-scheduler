/*
 * Author: Akhil Tarikere
 * Date: 8/3/20
 * 
 * Pre-Condition: Get the file to get data from.
 * 		
 *
 * Post-Condition: Fill the array of structures with data from
 *		the files along with the length of the array.
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"
#include "structNonPeriodic.h"


/*
 * Reads from the file given to it, and fills
 * the appropriate structure.
 */
Task
*periodicTaskInput(FILE *periodicTaskFile, int *n)
{
	// Taking data from the periodic job file.
	int numTasks;
	fscanf(periodicTaskFile, " %d", &numTasks);
	// printf("No. of lines = %d.\n", numTasks);

	Task *tasks = (Task *) malloc(sizeof(Task) * numTasks);
	int i;
	for (i = 0; i < numTasks; i++)
	{
		fscanf(periodicTaskFile, "%d %d %d", &tasks[i].period, &tasks[i].wcet, &tasks[i].deadline);
		// printf("%d %d %d\n", tasks[i].period, tasks[i].wcet, tasks[i].deadline);

		if (tasks[i].wcet > tasks[i].deadline)
		{
			fprintf(stderr, "WCET is greater than deadline. Please input valid data.\n" );
			fclose(periodicTaskFile);
			exit(0);
		}

		// Need to store this now as they will be sorted based on arrival time.
		tasks[i].taskNum = i;
	}
	fclose(periodicTaskFile);

	// Setting the value of number of lines, so that the function calling this function can use it.
	*n = numTasks;

	return tasks;
}


/*
 * Reads from the file given to it, and fills
 * the appropriate structure.
 */
AperiodicJob
*aperiodicJobsInput(FILE *aperiodicJobsFile, int *n)
{
	int numJobs;
	fscanf(aperiodicJobsFile, "%d", &numJobs);

	AperiodicJob *aperiodicJobs = (AperiodicJob *) malloc(sizeof(AperiodicJob) * numJobs);


	for (int i = 0; i < numJobs; ++i)
	{
		fscanf(aperiodicJobsFile, "%f %f", &aperiodicJobs[i].arrivalTime, &aperiodicJobs[i].wcet);

		// Need to store this now as they will be sorted based on arrival time.
		aperiodicJobs[i].jobNum = i;
		aperiodicJobs[i].timeLeft = aperiodicJobs[i].wcet;
		aperiodicJobs[i].alive = true;
	}

	fclose(aperiodicJobsFile);


	// Setting the value of number of lines, so that the function calling this function can use it.
	*n = numJobs;

	return aperiodicJobs;
}


/*
 * Reads from the file given to it, and fills
 * the appropriate structure.
 */
SporadicJob
*sporadicJobsInput(FILE *sporadicJobsFile, int *n)
{
	int numJobs;
	fscanf(sporadicJobsFile, "%d", &numJobs);

	SporadicJob *sporadicJobs = (SporadicJob *) malloc(sizeof(SporadicJob) * numJobs);

	for (int i = 0; i < numJobs; ++i)
	{
		fscanf(sporadicJobsFile, "%f %f %f", &sporadicJobs[i].arrivalTime, &sporadicJobs[i].wcet, &sporadicJobs[i].deadline);

		// Need to store this now as they will be sorted based on arrival time.
		sporadicJobs[i].jobNum = i;

		if (sporadicJobs[i].wcet > sporadicJobs[i].deadline)
		{
			fprintf(stderr, "Please enter valid data into sporadicJobs.txt\n");
			exit(0);
		}

		sporadicJobs[i].timeLeft = sporadicJobs[i].wcet;
		sporadicJobs[i].alive = true;
		sporadicJobs[i].accepted = false;
		sporadicJobs[i].rejected = false;
	}

	fclose(sporadicJobsFile);


	// Setting the value of number of lines, so that the function calling this function can use it.
	*n = numJobs;

	return sporadicJobs;
}

/*
 * Reads from the file given to it, and fills
 * the appropriate structure.
 */
ScheduleFrame
*framesInput(FILE *framesFile, int *nFrames, int *fSize)
{
	int numFrames, frameSize;
	fscanf(framesFile, "%d %d", &frameSize, &numFrames);
	printf("frameSize = %d, numFrames = %d\n", frameSize, numFrames);

	ScheduleFrame *framesData = (ScheduleFrame *) malloc(sizeof(ScheduleFrame) * numFrames);

	for (int i = 0; i < numFrames; ++i)
	{
		fscanf(framesFile, "%f %d\n", &framesData[i].slack, &framesData[i].numPeriodicJobs);
		framesData[i].periodicJobs = (PeriodicJob *) malloc(sizeof(PeriodicJob) * framesData[i].numPeriodicJobs);

		// Initialising the frames.
		framesData[i].numSporadicJobs = 0;
		framesData[i].sporadicJobs = (SporadicJob *) malloc(sizeof(SporadicJob) * 0);

		for (int j = 0; j < framesData[i].numPeriodicJobs; ++j)
		{
			char test;
			fscanf(framesFile, " %c", &test);
			// printf("%c ", test);

			if (test == 'N')
			{
				fscanf(framesFile, "%d %d %f", &framesData[i].periodicJobs[j].taskNum, &framesData[i].periodicJobs[j].instanceNum, &framesData[i].periodicJobs[j].wcet);
				framesData[i].periodicJobs[j].splitNum = -1;

				// printf("%d %d %f\n", framesData[i].periodicJobs[j].taskNum, framesData[i].periodicJobs[j].instanceNum, framesData[i].periodicJobs[j].wcet);
				// fflush(stdout);	
			}
			else
			{
				fscanf(framesFile, "%d %d %d %f", &framesData[i].periodicJobs[j].taskNum, &framesData[i].periodicJobs[j].splitNum, &framesData[i].periodicJobs[j].instanceNum, &framesData[i].periodicJobs[j].wcet);
				// printf("%d %d %d %f\n", framesData[i].periodicJobs[j].taskNum, framesData[i].periodicJobs[j].splitNum, framesData[i].periodicJobs[j].instanceNum, framesData[i].periodicJobs[j].wcet);
				// fflush(stdout);	
			}

			framesData[i].periodicJobs[j].alive = true;
		}
		
	}

	fclose(framesFile);

	*nFrames = numFrames;
	*fSize = frameSize;

	return framesData;
}