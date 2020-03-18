#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functionPeriodic.h"
#include "configuration.h"


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