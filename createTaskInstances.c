/*
 * Author: Akhil Tarikere
 * Date: 9/3/20
 * 
 * Pre-Condition: Given a set of tasks
 * 		
 *
 * Post-Condition: Creates and initialises the various jobs of that task that will be created for that hyperperiod.
 *	
 */

#include <stdio.h>
#include <stdbool.h>
#include "functionPeriodic.h"


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
		for (float j = 0; j < hyperperiod; j += tasks[i].period) // Iterates over the period of a task.
		{
			// Calculating the startFrame and maxFrame.
			int startFrame = (int)j / frameSize;
			int maxFrame = (int)(j + tasks[i].deadline) / frameSize;

			if ((int)j % frameSize != 0)
				startFrame++;

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

	return;
}