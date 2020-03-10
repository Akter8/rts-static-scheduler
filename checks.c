#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct.h"
#include "function.h"

FILE
*firstCheck(int argc, char *argv[])
{
	// Test to see if user has entered the file name or not.
	if (argc != 2)
	{
		fprintf(stderr, "Please enter valid arguments. Program exiting.\n");
		exit(0);
	}

	char *periodicJobFileName = argv[1];
	printf("The file entered is: %s\n", periodicJobFileName);

	FILE *periodicJobFile = fopen(periodicJobFileName, "r");
	// Checking if there is an error.
	if (!periodicJobFile)
	{
		fprintf(stderr, "Could not open file: %sProgram exiting.\n", periodicJobFileName);
		exit(0);
	}

	return periodicJobFile;
}


void
checkCpuUtilisation(Task *tasks, int numLines)
{
	float cpuUtilisation = calculateCpuUtilisation(tasks, numLines);
	if (cpuUtilisation < 1.0f)
	{
		printf("CPU Utilisation < 1: might be schedulable.\n");
	}
	else
	{
		fprintf(stderr, "CPU Utilisation >= 1: cannot be scheduled\n");
		exit(0);
	}
}


void
conditionChecker(Task *tasks, int numLines, int hyperperiod, int *c1, int *c2, int *c3, int *c1Index, int *reallocS)
{
	int condition1Size = frameSizeCondition1(tasks, numLines);
	printf("Condition-1: %d\n", condition1Size);


	int *condition2Sizes = (int *) malloc(sizeof(int) * hyperperiod);
	int reallocSize = frameSizeCondition2(hyperperiod, condition2Sizes);
	condition2Sizes = realloc(condition2Sizes, sizeof(int) * reallocSize);
	printf("Condition-2: ");
	int condition1Index1 = -1;
	for (int i = 0; i < reallocSize; ++i)
	{
		printf("%d ", condition2Sizes[i]);
		if(condition2Sizes[i] <= condition1Size)
			condition1Index1++;
	}
	printf("\n");


	int *condition3Sizes = (int *) malloc(sizeof(int) * reallocSize);
	reallocSize = frameSizeCondition3(condition2Sizes, tasks, reallocSize, numLines, condition3Sizes);
	condition3Sizes = realloc(condition3Sizes, sizeof(int) * reallocSize);
	int condition1Index2 = -1;
	printf("Condition-3: ");
	for (int i = 0; i < reallocSize; ++i)
	{
		printf("%d ", condition3Sizes[i]);
		if(condition3Sizes[i] <= condition1Size)
			condition1Index2++;
	}
	printf("\n");

	int condition1Index = condition1Index1 < condition1Index2 ? condition1Index1 : condition1Index2;
	printf("Possible frame size(s) based on the three conditions: ");
	for (int i = condition1Index+1; i < reallocSize; ++i)
	{
		printf("%d ", condition3Sizes[i]);
	}
	printf("\n");


	*c1 = condition1Size;
	*c1Index = condition1Index;
	*reallocS = reallocSize;
	c2 = condition2Sizes;
	c3 = condition3Sizes;
}