#include "function.h"
#include "functionNonPeriodic.h"

// Utility function to find GCD of 'a' and 'b'. 
int
gcd(int a, int b) 
{
	if (b == 0) 
		return a;
	return gcd(b, a % b); 
}


/*
 * Finds and returns the CPU utilisation.
 * CPU utilisation = sum(ei/pi)
 */
float
calculateCpuUtilisation(Task *tasks, int numTasks)
{
	float sum = 0;
	for (int i = 0; i < numTasks; ++i)
	{
		sum += ((float)tasks[i].wcet / tasks[i].period);
	}

	return sum;
}


/*
 * Finding hyperperiod is the same as LCM for in-phase jobs.
 * This method is essentially to find the LCM.
 */
int
findHyperPeriod(Task *tasks, int n)
{
	int ans = tasks[0].period;
	for (int i = 1; i < n; i++) 
        	ans = (((tasks[i].period * ans)) / (gcd((int)tasks[i].period, ans))); 
       	return ans;
}


/*
 * Finds the total number of task-instances that are to be created
 * in the case of periodic tasks.
 */
int
findNumJobs(Task *tasks, int numTasks, int hyperperiod)
{
	int numJobs = 0;
	// Calculating the number of total jobs.
	for (int i = 0; i < numTasks; ++i)
	{
		if (tasks[i].numOfSplits == 0)
			numJobs += hyperperiod / tasks[i].period;
		else
			numJobs += (hyperperiod / tasks[i].period) * (tasks[i].numOfSplits + 1);
	}

	return numJobs;
}