/*
 * Author: Akhil Tarikere
 * Date: 9/3/20
 *
 * This module contains a few utility functions.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "configuration.h"
#include "functionPeriodic.h"
#include "functionNonPeriodic.h"


/*
 * Utility function to find GCD of 'a' and 'b'.
 */
int
gcd(int a, int b)
{
	if (b == 0)
		return a;
	return gcd(b, a % b);
}


/*
 * Utility function to find absolute value of 'x'.
 */
float
floatAbs(float x)
{
	if (x > 0)
		return x;
	else
		return -x;
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
		numJobs += hyperperiod / tasks[i].period;
	}

	return numJobs;
}


/*
 * Checks to see if the number of arguments given to the main driver
 * is equal to two. If not, it exits the program.
 */
void
firstCheck(int argc, char *argv[])
{
	// Test to see if user has entered the file name or not.
	if (argc != 2)
	{
		fprintf(stderr, "Please enter valid arguments. Program exiting.\n");
		exit(0);
	}
}


/*
 * Tries to open any files given to it.
 * Returns the file if it opened it successfully, else exits.
 */
FILE
*inputFileCheck(char *fileName)
{
	FILE *file = fopen(fileName, "r");

	// Checking if there is an error.
	if (!file)
	{
		fprintf(stderr, "Could not open file: %s\nProgram exiting.\n", fileName);
		exit(0);
	}

	return file;
}


/*
 * Finds the total CPU utilisation of the periodic tasks, if > 1
 * it exits the program.
 */
void
checkCpuUtilisation(Task *tasks, int numTasks)
{
	float cpuUtilisation = calculateCpuUtilisation(tasks, numTasks);
	if (cpuUtilisation <= 1.0f)
	{
		FILE *outputFile = fopen(OUTPUT_FILE, "a");
		fprintf(outputFile, "CPU Utilisation <= 1: might be schedulable.\n");
		fclose(outputFile);
	}
	else
	{
		fprintf(stderr, "CPU Utilisation > 1: cannot be scheduled\n");
		exit(0);
	}
}