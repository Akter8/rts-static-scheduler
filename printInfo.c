/*
 * Author: Akhil Tarikere
 * Date: 7/3/20
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
#include "function.h"
#include "functionNonPeriodic.h"

void
printTaskInfo(Task *tasks, int numTasks)
{
	printf("----------------------------\n");
	printf("Task Info\n");
	printf("Num Tasks: %d\n", numTasks);
	for (int i = 0; i < numTasks; ++i)
	{
		printf("Task=T%d, period=%d, wcet=%d, deadline=%d, numOfSplits=%d", tasks[i].taskNum, tasks[i].period, tasks[i].wcet, tasks[i].deadline, tasks[i].numOfSplits);
		if (tasks[i].numOfSplits != 0)
		{
			printf(", splits=");
			for (int j = 0; j < tasks[i].numOfSplits + 1; ++j)
			{
				printf("%d ", tasks[i].splits[j]);
			}
		}
		printf("\n");
	}
}


void
printJobInfo(TaskInstance * jobs, int numJobs)
{
	printf("----------------------------\n");
	printf("Job Info\n");
	printf("Num Jobs: %d\n", numJobs);
	for (int i = 0; i < numJobs; ++i)
	{
		if (jobs[i].splitNum != -1)
			printf("J%d,%d TaskInstance=%d: startFrame=%d, maxFrame=%d, wcet=%d, alive=%d\n", jobs[i].taskNum, jobs[i].splitNum, jobs[i].instanceNum, jobs[i].startFrame, jobs[i].maxFrame, jobs[i].wcet, jobs[i].alive);
		else
			printf("J%d TaskInstance=%d: startFrame=%d, maxFrame=%d, wcet=%d, alive=%d\n", jobs[i].taskNum, jobs[i].instanceNum, jobs[i].startFrame, jobs[i].maxFrame, jobs[i].wcet, jobs[i].alive);
	}
}


void
printFrameInfo(Frame * frames, int numFrames)
{
	printf("----------------------------\n");
	printf("Frame Info\n");
	printf("Num Frames: %d\n", numFrames);
	for (int f = 0; f < numFrames; ++f)
	{
		printf("Frame no.%d, Jobs=\n", f);
		for (int i = 0; i < frames[f].numJobs; ++i)
		{
			if (frames[f].jobs[i].splitNum == -1)
			{
				printf("\tJob%d Instance=%d\n", frames[f].jobs[i].taskNum, frames[f].jobs[i].instanceNum);
			}
			else
				printf("\tJob%d,%d Instance=%d\n", frames[f].jobs[i].taskNum, frames[f].jobs[i].splitNum, frames[f].jobs[i].instanceNum);
		}
	}
}


void
storeFrameInfo(Frame *frames, int numFrames, int frameSize)
{
	char *outputFileName = "periodicSchedule.txt";
	FILE *outputFile;
	outputFile = fopen(outputFileName, "w");

	if(!outputFile)
	{
		fprintf(stderr, "The outputFile- periodicSchedule.txt did not open properly.\n");
		exit(0);
	}

	fprintf(outputFile, "%d\n", frameSize);
	fprintf(outputFile, "%d\n\n", numFrames);
	for (int f = 0; f < numFrames; ++f) // Iterates through all the frames.
	{
		// fprintf(outputFile, "%d\n", f);
		fprintf(outputFile, "%0.1f\n", frames[f].slack);
		fprintf(outputFile, "%d\n", frames[f].numJobs);
		for (int i = 0; i < frames[f].numJobs; ++i)
		{
			if (frames[f].jobs[i].splitNum != -1) // If the task has been split.
				fprintf(outputFile, "Y %d %d %d %d\n", frames[f].jobs[i].taskNum, frames[f].jobs[i].splitNum, frames[f].jobs[i].instanceNum, frames[f].jobs[i].wcet);
			else
				fprintf(outputFile, "N %d %d %d\n", frames[f].jobs[i].taskNum, frames[f].jobs[i].instanceNum, frames[f].jobs[i].wcet);
		}
		fprintf(outputFile, "\n");		
	}

	fclose(outputFile);

	return;
}


void
printAperiodicJobInfo(AperiodicJob *aperiodicJobs, int numJobs)
{
	printf("----------------------------\n");
	printf("Aperiodic Job Info\n");

	for (int i = 0; i < numJobs; ++i)
	{
		printf("Job=A%d, ArrivalTime=%0.1f, ExecutionTime=%0.1f\n", aperiodicJobs[i].jobNum, aperiodicJobs[i].arrivalTime, aperiodicJobs[i].wcet);
	}
}



void
printSporadicJobInfo(SporadicJob *sporadicJobs, int numJobs)
{
	printf("----------------------------\n");
	printf("Sporadic Job Info\n");

	for (int i = 0; i < numJobs; ++i)
	{
		printf("Job=S%d, ArrivalTime=%0.1f, ExecutionTime=%0.1f, Deadline=%0.1f, startFrame=%d, maxFrame=%d\n", sporadicJobs[i].jobNum, sporadicJobs[i].arrivalTime, sporadicJobs[i].wcet, sporadicJobs[i].deadline, sporadicJobs[i].startFrame, sporadicJobs[i].maxFrame);
	}
}


void
printScheduleFrameInfo(ScheduleFrame *framesData, int numFrames)
{
	printf("----------------------------\n");
	printf("Schedule Frame Info\n");

	for (int f = 0; f < numFrames; ++f)
	{
		printf("Frame no: %d\n", f);
		printf("Slack: %0.1f\n", framesData[f].slack);
		printf("No of periodic jobs: %d\n", framesData[f].numPeriodicJobs);
		for (int i = 0; i < framesData[f].numPeriodicJobs; ++i)
		{
			if (framesData[f].periodicJobs[i].splitNum != -1)
				printf("J%d,%d: Instance: %d, wcet: %0.1f\n", framesData[f].periodicJobs[i].taskNum, framesData[f].periodicJobs[i].splitNum, framesData[f].periodicJobs[i].instanceNum, framesData[f].periodicJobs[i].wcet);
			else
				printf("J%d: Instance: %d, wcet: %0.1f\n", framesData[f].periodicJobs[i].taskNum, framesData[f].periodicJobs[i].instanceNum, framesData[f].periodicJobs[i].wcet);
		}

		printf("\n");
	}
}