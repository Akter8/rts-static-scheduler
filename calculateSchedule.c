#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "function.h"

/*
 * Attempts to create a schedule using EDF algorithm.
 * 
 */
void
calculateSchedule(TaskInstance *jobs, int numJobs, int frameSize, int hyperperiod, Frame *frames)
{
	printf("Ima here\n");
	fflush(stdout);
	int numOfFrames = hyperperiod / frameSize;

	int aliveCount = numJobs;
	// printf("initial aliveCount=%d\n", aliveCount);
	// printf("hyperperiod=%d, frameSize=%d, numOfFrames=%d\n", hyperperiod, frameSize, numOfFrames);
	for (int f = 0; f < numOfFrames; ++f)
	{
		// printf("Frame No=%d\nJobs=", f);
		frames[f].numFrame = f;
		frames[f].numJobs = 0;
		fflush(stdout);
		frames[f].jobs = (TaskInstance *) malloc(sizeof(TaskInstance) * aliveCount);

		bool flag = true;
		int minIndex;
		int timeLeft = frameSize;
		while(flag)
		{
			flag = false;
			minIndex = 0;

			for (int i = 0; i < numJobs; ++i) // For finding the first valid entry in the array.
			{
				if (!(jobs[i].alive && jobs[i].maxFrame > f && jobs[i].startFrame <= f && jobs[i].wcet <= timeLeft))
					minIndex++;
				else
				{
					flag = true;
					break;
				}
			}

			// printf("minIndex=%d\n", minIndex);
			for (int i = minIndex+1; i < numJobs; ++i)
			{
				if (jobs[i].alive && jobs[i].startFrame <= f && jobs[i].maxFrame > f && jobs[i].maxFrame < jobs[minIndex].maxFrame && jobs[i].wcet <= timeLeft)
				{
					minIndex = i;
				}
				// else if(jobs[i].alive && jobs[i].startFrame <= f && jobs[i].maxFrame == jobs[minIndex].maxFrame && jobs[i].wcet <= timeLeft)
				// 	if (jobs[i].wcet > jobs[minIndex].wcet)
				// 	{
				// 		minIndex = i;
				// 		flag = true;
				// 	}
			}

			if (!flag)
				break;
			// printf("minIndex=%d\n", minIndex);
			// printf("J%d,%d; ", jobs[minIndex].taskNum, jobs[minIndex].instanceNum);
			// fflush(stdout);
			frames[f].jobs[frames[f].numJobs] = jobs[minIndex];
			frames[f].numJobs++;
			jobs[minIndex].alive = false;
			aliveCount--;
			timeLeft -= jobs[minIndex].wcet;
		}

		// printf("numJobs=%d, jobsAlive=%d\n", frames[f].numJobs, aliveCount);
		fflush(stdout);
		frames[f].jobs = (TaskInstance *) realloc(frames[f].jobs, sizeof(TaskInstance) * frames[f].numJobs);
		frames[f].slack = timeLeft;
		// printf("numJobs in Frame-%d is: %d", f, frames[f].numJobs);
		// printf("\n");
	}

	// printf("final aliveCount=%d\n", aliveCount);
	// if (aliveCount > 0)
	// {
	// 	printf("Could not schedule this set.\n");
	// }
	// else
	// {
	// 	printf("Schedule has been made!!\n");
	// }

	// Resetting the jobs so that they can be scheduled again.
	for (int i = 0; i < numJobs; i++)
		jobs[i].alive = true;

	return;
}