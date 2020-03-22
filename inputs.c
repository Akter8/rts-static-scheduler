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
#include "functionPeriodic.h"
#include "functionNonPeriodic.h"


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

	Task *tasks = (Task *) malloc(sizeof(Task) * numTasks);
	int i;
	for (i = 0; i < numTasks; i++) // One task at a time.
	{
		fscanf(periodicTaskFile, "%d %f %f", &tasks[i].period, &tasks[i].wcet, &tasks[i].deadline);

		// A couple of basic checks while taking input.
		if (tasks[i].period <= 0 || tasks[i].wcet <= 0 || tasks[i].deadline <= 0)
		{
			fprintf(stderr, "Encountered a negative task parameter. Please input only positive values.\n");
			free(tasks);
			fclose(periodicTaskFile);
			exit(0);
		}

		if (tasks[i].wcet > tasks[i].deadline)
		{
			fprintf(stderr, "WCET is greater than deadline. Please input valid data.\n" );
			free(tasks);
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

		// Basic check after taking input.
		if (aperiodicJobs[i].arrivalTime < 0 || aperiodicJobs[i].wcet <= 0)
		{
			fprintf(stderr, "Please enter positive parameters for aperiodic jobs.\n");
			exit(0);
		}

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

		// Basic checks after taking input.
		if (sporadicJobs[i].arrivalTime < 0 || sporadicJobs[i].wcet <= 0 || sporadicJobs[i].deadline <= 0)
		{
			fprintf(stderr, "Please enter only positive value in sporadic job parameters.\n");
			exit(0);
		}

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

	ScheduleFrame *framesData = (ScheduleFrame *) malloc(sizeof(ScheduleFrame) * numFrames);

	for (int i = 0; i < numFrames; ++i)
	{
		fscanf(framesFile, "%f %d\n", &framesData[i].slack, &framesData[i].numPeriodicJobs);
		framesData[i].periodicJobs = (PeriodicJob *) malloc(sizeof(PeriodicJob) * framesData[i].numPeriodicJobs);

		for (int j = 0; j < framesData[i].numPeriodicJobs; ++j)
		{
			char test;
			fscanf(framesFile, " %c", &test);
			// printf("%c ", test);

			if (test == 'N')
			{
				fscanf(framesFile, "%d %d %f", &framesData[i].periodicJobs[j].taskNum, &framesData[i].periodicJobs[j].instanceNum, &framesData[i].periodicJobs[j].wcet);
				framesData[i].periodicJobs[j].splitNum = -1;	
			}
			else
			{
				fscanf(framesFile, "%d %d %d %f", &framesData[i].periodicJobs[j].taskNum, &framesData[i].periodicJobs[j].splitNum, &framesData[i].periodicJobs[j].instanceNum, &framesData[i].periodicJobs[j].wcet);	
			}

			framesData[i].periodicJobs[j].alive = true;
		}
	}

	fclose(framesFile);

	// To return the values.
	*nFrames = numFrames;
	*fSize = frameSize;

	return framesData;
}