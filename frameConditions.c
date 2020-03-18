/*
 * Author: Akhil Tarikere
 * Date: 6/3/20
 * 
 * Pre-Condition: Given set of tasks along with its hyperperiod.
 *
 * Post-Condition: The frame sizes that meet the various conditions.
 */


#include <stdio.h>
#include <stdbool.h>
#include "functionPeriodic.h"

/*
 * Returns the minimum size of the frame according to condition-1.
 * Condition-1: Frame size should be greater than the maximum of the worst-case execution times of the task set.
 */
int
frameSizeCondition1(Task *tasks, int n)
{
	int maxSize = tasks[0].wcet;
	for (int i = 1; i < n; i++)
	{
		if (maxSize < tasks[i].wcet)
			maxSize = tasks[i].wcet;
	}

	return maxSize;
}


/*
 * Finds all the frame sizes possible based on condition-2.
 * Condition-2: [pi/f] - pi/f = 0 for at least one pi.
 */
int
frameSizeCondition2(int hyperperiod, int *answer)
{
	
	int j = 0;
	for (int i = 1; i <= hyperperiod; i++)
	{
		if (hyperperiod % i == 0)
			answer[j++] = i;
	}

	return j;
}


/*
 * Finds all the frame sizes possible based on condition-2.
 * Condition-3: 2*f - gcd(pi,f) <= Di for all i for f to be a valid frame size.
 */
int
frameSizeCondition3(int *frames, Task *tasks, int numFrames, int numTasks, int *answers)
{
	int k = 0;
	for (int i = 0; i < numFrames; ++i)
	{
		bool flag = true;
		for (int j = 0; j < numTasks; ++j)
		{
			if (2*frames[i] - gcd(frames[i], tasks[j].period) > tasks[j].deadline)
			{
				flag = false;
				break;
			}
		}
		if (flag)
			answers[k++] = frames[i];
	}

	return k;
}