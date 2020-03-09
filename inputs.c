#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"

Task
*periodicTaskInput(FILE *periodicTaskFile, int *n)
{
	// Taking data from the periodsic job file.
	int numLines;
	fscanf(periodicTaskFile, " %d", &numLines);
	// printf("No. of lines = %d.\n", numLines);

	Task *tasks = (Task *) malloc(sizeof(Task) * numLines);
	int i;
	for (i = 0; i < numLines; i++)
	{
		fscanf(periodicTaskFile, "%d %d %d", &tasks[i].period, &tasks[i].wcet, &tasks[i].deadline);
		// printf("%d %d %d\n", tasks[i].period, tasks[i].wcet, tasks[i].deadline);

		if (tasks[i].wcet > tasks[i].deadline)
		{
			fprintf(stderr, "WCET is greater than deadline. Please input valid data.\n" );
			fclose(periodicTaskFile);
			exit(0);
		}
	}
	fclose(periodicTaskFile);
	*n = numLines;

	return tasks;
}