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


typedef struct
{
	int period;
	int deadline;
	int wcet;
	int numOfSplits;
	int *splits;

}Task;

typedef struct
{
	int startFrame;
	int maxFrame;
	int wcet;
	int taskNum;
	int splitNum;
	int instanceNum;
}TaskInstance;


int findHyperPeriod(Task *, int);
int gcd(int, int);
int frameSizeCondition1(Task *, int);
int frameSizeCondition2(int, int *);
int frameSizeCondition3(int *, Task *, int, int, int *);
void createTaskInstances(Task *, TaskInstance *, int, int, int, int);
void calculateSchedule(TaskInstance *, int, int, int);


int
main(int argc, char const *argv[])
{
	// Test to see if user has entered the file name or not.
	if (argc != 2)
	{
		fprintf(stderr, "Please enter valid arguments. Program exiting.\n");
		goto end;
	}

	char *periodicJobFileName = argv[1];
	printf("The file entered is: %s\n", periodicJobFileName);

	FILE *periodicJobFile = fopen(periodicJobFileName, "r");
	// Checking if there is an error.
	if (!periodicJobFile)
	{
		fprintf(stderr, "Could not open file: %sProgram exiting.\n", periodicJobFileName);
		goto end;
	}

	// Taking data from the periodsic job file.
	int numLines;
	fscanf(periodicJobFile, " %d", &numLines);
	printf("No. of lines = %d.\n", numLines);
	int *periods, *deadlines, *wcet;

	Task *tasks = (Task *) malloc(sizeof(Task) * numLines);
	int i;
	for (i = 0; i < numLines; i++)
	{
		fscanf(periodicJobFile, "%d %d %d", &tasks[i].period, &tasks[i].wcet, &tasks[i].deadline);
		// printf("%d %d %d\n", tasks[i].period, tasks[i].wcet, tasks[i].deadline);

		if (tasks[i].wcet > tasks[i].deadline)
		{
			fprintf(stderr, "WCET is greater than deadline. Please input valid data.\n" );
			goto free;
		}
	}


	int hyperperiod = findHyperPeriod(tasks, numLines);
	printf("The hyperperiod is: %d.\n", hyperperiod);


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
		if(condition2Sizes[i] < condition1Size)
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
		if(condition3Sizes[i] < condition1Size)
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


	int numTasks = numLines;

	// Check to see if jobs need to be sliced.
	printf("reallocSize=%d condition1Index=%d\n", reallocSize, condition1Index);
	if (reallocSize == condition1Index+1)
	{
		printf("Have to slice the tasks as there is no frame size fitting the task set.\n");

		int numOfJobsToSlice = 0;
		int *indicesOfJobsToSlice = (int *) malloc(sizeof(int) * reallocSize + 1);

		// Finding the number of jobs that need to be sliced at min.
		for (int i = 0; i < numLines; ++i)
		{
			if(tasks[i].wcet > condition3Sizes[condition1Index])
			{
				indicesOfJobsToSlice[numOfJobsToSlice] = i;
				numOfJobsToSlice++;
			}
		}

		printf("Trying to slice %d task(s).\nTask(s) being split:", numOfJobsToSlice);
		for (int i = 0; i < numOfJobsToSlice; ++i)
		{
			printf(" %d", indicesOfJobsToSlice[i]);
		}
		printf("\n");


		// Calculating the splits for the jobs that need to be split.
		for (int i = 0; i < numOfJobsToSlice; ++i)
		{
			tasks[indicesOfJobsToSlice[i]].numOfSplits = (tasks[indicesOfJobsToSlice[i]].wcet -1) / condition3Sizes[condition1Index];

			tasks[indicesOfJobsToSlice[i]].splits = (int *) malloc(sizeof(int) * (tasks[indicesOfJobsToSlice[i]].numOfSplits + 1));
			
			printf("Task-%d with wcet=%d being split into=%d splits and to: ", indicesOfJobsToSlice[i], tasks[indicesOfJobsToSlice[i]].numOfSplits, tasks[indicesOfJobsToSlice[i]].wcet);
			
			for (int j = 0; j < tasks[indicesOfJobsToSlice[i]].numOfSplits + 1; ++j)
			{
				tasks[indicesOfJobsToSlice[i]].splits[j] = condition3Sizes[condition1Index];
				printf("%d ", condition3Sizes[condition1Index]);
			}

			int remOfSplit = tasks[indicesOfJobsToSlice[i]].wcet % condition3Sizes[condition1Index];

			if (remOfSplit != 0)
			{
				tasks[indicesOfJobsToSlice[i]].splits[tasks[indicesOfJobsToSlice[i]].numOfSplits] = remOfSplit;
				printf("%d", remOfSplit);
			}
			printf("\n");
		}
	}

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

	int numJobs = 0;
	// Calculating the number of total jobs.
	for (int i = 0; i < numLines; ++i)
	{
		if (tasks[i].numOfSplits == 0)
			numJobs += hyperperiod / tasks[i].period;
		else
			numJobs += (hyperperiod / tasks[i].period) * (tasks[i].numOfSplits + 1);
	}
	// printf("numJobs=%d\n", numJobs);
	// Creating numJobs of Task Instances.
	TaskInstance *jobs = (TaskInstance *) malloc(sizeof(TaskInstance) * numJobs);
	createTaskInstances(tasks, jobs, condition3Sizes[condition1Index], hyperperiod, numLines, numJobs);

	printf("----------------------------\n");
	printf("Job Info\n");
	for (int i = 0; i < numJobs; ++i)
	{
		if (jobs[i].splitNum != -1)
			printf("J%d,%d TaskInstance=%d: startFrame=%d, maxFrame=%d, wcet=%d\n", jobs[i].taskNum, jobs[i].splitNum, jobs[i].instanceNum, jobs[i].startFrame, jobs[i].maxFrame, jobs[i].wcet);
		else
			printf("J%d TaskInstance=%d: startFrame=%d, maxFrame=%d, wcet=%d\n", jobs[i].taskNum, jobs[i].instanceNum, jobs[i].startFrame, jobs[i].maxFrame, jobs[i].wcet);
	}

	int frameSize = condition2Sizes[condition1Index+1];
	calculateSchedule(jobs, numJobs, hyperperiod, frameSize);




	free(condition2Sizes);
	free(condition3Sizes);
free:
	// Closing and freeing data.
	for (int i = 0; i < numLines; ++i)
	{
		free(tasks[i].splits);
	}
	free(tasks);
	fclose(periodicJobFile);

end:
	return 0;
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
        	ans = (((tasks[i].period * ans)) / (gcd(tasks[i].period, ans))); 
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


/*
 * Fills in the details of the array of various instances of the tasks based on the task details.
 * Also, takes care of the tasks that have been split.
 */
void
createTaskInstances(Task *tasks, TaskInstance *jobs, int frameSize, int hyperperiod, int numTasks, int numJobs)
{
	int jobIndex = 0;
	for (int i = 0; i < numTasks; ++i) // Iterates of tasks
	{
		for (int j = 0; j < hyperperiod; j += tasks[i].period) // Iterattes over the period of a task.
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
				jobIndex++;
			}
		}
	}

	return;
}