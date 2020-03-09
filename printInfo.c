#include <stdio.h>
#include <stdbool.h>
#include "struct.h"

void
printTaskInfo(Task *tasks, int numLines)
{
	printf("----------------------------\n");
	printf("Task Info\n");
	for (int i = 0; i < numLines; ++i)
	{
		printf("Task=%d, period=%d, wcet=%d, deadline=%d, numOfSplits=%d", i, tasks[i].period, tasks[i].wcet, tasks[i].deadline, tasks[i].numOfSplits);
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