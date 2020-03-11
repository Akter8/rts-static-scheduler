#include <stdbool.h>
#include "struct.h"

/*
 * Fills in the details of the array of various instances of the tasks based on the task details.
 * Also, takes care of the tasks that have been split.
 */
void
createTaskInstances(Task *tasks, TaskInstance *jobs, int frameSize, int hyperperiod, int numTasks, int numJobs)
{
	int jobIndex = 0;
	for (int i = 0; i < numTasks; ++i) // Iterates over tasks.
	{
		for (int j = 0; j < hyperperiod; j += tasks[i].period) // Iterates over the period of a task.
		{
			int startFrame = j / frameSize;
			int maxFrame = (j + tasks[i].deadline) / frameSize;
			if (j % frameSize != 0)
				startFrame++;

			if (tasks[i].numOfSplits != 0) // If the task has been split.
			{
				for (int k = 0; k < tasks[i].numOfSplits + 1; k++) // Iterates over the various splits of a task.
				{
					jobs[jobIndex].startFrame = startFrame;
					jobs[jobIndex].maxFrame = maxFrame;
					jobs[jobIndex].wcet = tasks[i].splits[k];
					jobs[jobIndex].splitNum = k;
					jobs[jobIndex].taskNum = i;
					jobs[jobIndex].instanceNum = j / tasks[i].period;
					jobs[jobIndex].alive = true;

					jobIndex++;
				}
			}
			else // If the task has not been split.
			{
				jobs[jobIndex].startFrame = startFrame;
				jobs[jobIndex].maxFrame = maxFrame;
				jobs[jobIndex].taskNum = i;
				jobs[jobIndex].splitNum = -1;
				jobs[jobIndex].wcet = tasks[i].wcet;
				jobs[jobIndex].instanceNum = j / tasks[i].period;
				jobs[jobIndex].alive = true;
				jobIndex++;
			}
		}
	}

	return;
}