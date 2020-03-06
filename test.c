/*
 * Author: Akhil Tarikere
 * Date: 6/3/20
 * 
 * Pre-Condition: CLI argument of Periodic job 
 * 		file name containing the test set.
 *
 * Post-Condition: The hyperperiods and the 
 *		frame sizes that fit this tast set.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int findHyperPeriod(int *, int);
int gcd(int, int);
int frameSizeCondition1(int *, int);
int frameSizeCondition2(int, int *, int);
int frameSizeCondition3(int *, int *, int *, int, int, int *);


int
main(int argc, char const *argv[])
{
	// Test to see if user has entered the file name or not.
	if (argc != 2) {
		fprintf(stderr, "Please enter valid arguments. Program exiting.\n");
		goto end;
	}

	char *periodsicJobFileName = argv[1];
	printf("The file entered is: %s\n", periodsicJobFileName);

	FILE *periodsicJobFile = fopen(periodsicJobFileName, "r");
	// Checking if there is an error.
	if (!periodsicJobFile) {
		fprintf(stderr, "Could not open file: %sProgram exiting.\n", periodsicJobFileName);
		goto end;
	}

	// Taking data from the periodsic job file.
	int nLines;
	fscanf(periodsicJobFile, " %d", &nLines);
	printf("No. of lines = %d.\n", nLines);
	int *periods, *deadlines, *wcet;

	periods = (int *) malloc(sizeof(int) * nLines);
	deadlines = (int *) malloc(sizeof(int) * nLines);
	wcet = (int *) malloc(sizeof(int) * nLines);


	int i;
	for (i = 0; i < nLines; i++) {
		fscanf(periodsicJobFile, "%d %d %d", &periods[i], &wcet[i], &deadlines[i]);
		// printf("%d %d %d\n", periods[i], wcet[i], deadlines[i]);

		if ((float)wcet[i] * 0.2 > deadlines[i]) {
			fprintf(stderr, "WCET*0.2 is greater than deadline. Please input valid data.\n" );
			goto free;
		}
	}


	int hyperperiod = findHyperPeriod(periods, nLines);
	printf("The hyperperiod is: %d.\n", hyperperiod);


	int condition1Size = frameSizeCondition1(wcet, nLines);
	printf("Condition-1 returns minimum size of the frame as: %d.\n", condition1Size);


	int *condition2Sizes = (int *) malloc(sizeof(int) * hyperperiod);
	int reallocSize = frameSizeCondition2(hyperperiod, condition2Sizes, condition1Size);
	condition2Sizes = realloc(condition2Sizes, sizeof(int) * reallocSize);
	printf("Condition-2 returns the various sizes of the frame size possible: ");
	for (int i = 0; i < reallocSize; ++i)
	{
		printf("%d ", condition2Sizes[i]);
	}
	printf("\n");


	int *condition3Sizes = (int *) malloc(sizeof(int) * reallocSize);
	reallocSize = frameSizeCondition3(condition2Sizes, periods, deadlines, reallocSize, nLines, condition3Sizes);
	condition3Sizes = realloc(condition3Sizes, sizeof(int) * reallocSize);
	printf("Condition-3 returns the various sizes of the frame size possible: ");
	for (int i = 0; i < reallocSize; ++i)
	{
		printf("%d ", condition3Sizes[i]);
	}
	printf("\n");


	// Check to see if jobs need to be sliced.
	if (reallocSize == 0)
	{
		printf("Have to slice the tasks as there is no frame size fitting the task set.\n");
	}


	free(condition2Sizes);
	free(condition3Sizes);
free:
	// Closing and freeing data.
	free(periods);
	free(deadlines);
	free(wcet);
	fclose(periodsicJobFile);

end:
	return 0;
}

/*
 * Finding hyperperiod is the same as LCM for in-phase jobs.
 * This method is essentially to find the LCM.
 */
int
findHyperPeriod(int *periods, int n)
{
	int ans = periods[0];
	for (int i = 1; i < n; i++) 
        	ans = (((periods[i] * ans)) / (gcd(periods[i], ans))); 
       	return ans;
}


// Utility function to find GCD of 'a' and 'b'. 
int
gcd(int a, int b) 
{
	if (b == 0) 
		return a;
	return gcd(b, a % b); 
}


/*
 * Returns the minimum size of the frame according to condition-1.
 * Condition-1: Frame size should be greater than the maximum of the worst-case execution times of the task set.
 */
int
frameSizeCondition1(int *wcet, int n)
{
	int maxSize = wcet[0];
	for (int i = 1; i < n; i++) {
		if (maxSize < wcet[i])
			maxSize = wcet[i];
	}

	return maxSize;
}


/*
 *
 *
 */
int
frameSizeCondition2(int hyperperiod, int *answer, int condition1Size)
{
	
	int j = 0;
	for (int i = condition1Size; i <= hyperperiod; i++) {
		if (hyperperiod % i == 0)
			answer[j++] = i;
	}

	return j;
}


/*
 *
 *
 */
int
frameSizeCondition3(int *frames, int *periods, int *deadlines, int numFrames, int numPeriods, int *answers)
{
	int k = 0;
	for (int i = 0; i < numFrames; ++i)
	{
		// printf("%d\n", frames[i]);
		bool flag = true;
		for (int j = 0; j < numPeriods; ++j)
		{
			if (!(2*frames[i] - gcd(frames[i], periods[j]) <= deadlines[j]))
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