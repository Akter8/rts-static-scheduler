/*
 * Author: Akhil Tarikere
 * Date: 7/3/20
 * 
 * Pre-Condition: Takes in the required ADTs to be printed.
 * 		
 *
 * Post-Condition: Prints the data in the structure onto the output file.
 *		
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>

#include "functionPeriodic.h"
#include "functionNonPeriodic.h"
#include "configuration.h"


extern Task *tasks;
extern int numTasks;

extern int numPreemptions;
extern int numCacheImpactPoints;
extern int numContinuousPeriodicJobsOfSameTask;

extern FILE *outputFile;


/*
 * Prints Task information onto the output file.
 */
void
printTaskInfo(Task *tasks, int numTasks)
{
	fprintf(outputFile, "----------------------------------------------\n");
	fprintf(outputFile, "Task Info\nSorted based on period.\n");
	fprintf(outputFile, "Num Tasks: %d\n", numTasks);
	for (int i = 0; i < numTasks; ++i) // Iterates through every task.
	{
		fprintf(outputFile, "Task=T%d, period=%d, wcet=%0.1f, deadline=%0.1f\n", tasks[i].taskNum, tasks[i].period, tasks[i].wcet, tasks[i].deadline);
	}
}



/*
 * Prints the job information onto the output file.
 */
void
printJobInfo(TaskInstance * jobs, int numJobs)
{
	fprintf(outputFile, "----------------------------------------------\n");
	fprintf(outputFile, "Job Info\n");
	fprintf(outputFile, "Num Jobs: %d\n", numJobs);
	for (int i = 0; i < numJobs; ++i) // Iterates thorugh every job.
	{
		if (jobs[i].splitNum != -1) // If the job has been split.
			fprintf(outputFile, "J%d split=%d TaskInstance=%d: startFrame=%d, maxFrame=%d, wcet=%0.1f, alive=%d\n", jobs[i].taskNum, jobs[i].splitNum, jobs[i].instanceNum, jobs[i].startFrame, jobs[i].maxFrame, jobs[i].wcet, jobs[i].alive);
		else
			fprintf(outputFile, "J%d TaskInstance=%d: startFrame=%d, maxFrame=%d, wcet=%0.1f, alive=%d\n", jobs[i].taskNum, jobs[i].instanceNum, jobs[i].startFrame, jobs[i].maxFrame, jobs[i].wcet, jobs[i].alive);
	}
}



/*
 * Prints the frame information onto the outputfile.
 */
void
printFrameInfo(Frame * frames, int numFrames, int frameSize)
{
	fprintf(outputFile, "----------------------------------------------\n");
	fprintf(outputFile, "Frame Info\n");
	fprintf(outputFile, "Num Frames: %d\n", numFrames);
	fprintf(outputFile, "Frame size: %d\n", frameSize);
	for (int f = 0; f < numFrames; ++f) // Iterates through frames.
	{
		fprintf(outputFile, "Frame no.%d, No of jobs=%d, Jobs=\n", frames[f].frameNum, frames[f].numJobs);
		for (int i = 0; i < frames[f].numJobs; ++i) // Iterates through jobs in that frame.
		{
			if (frames[f].jobs[i].splitNum == -1) // If the job has not been split.
			{
				fprintf(outputFile, "\tJ%d instance=%d, wcet=%0.1f\n", frames[f].jobs[i].taskNum, frames[f].jobs[i].instanceNum, frames[f].jobs[i].wcet);
			}
			else
				fprintf(outputFile, "\tJ%d split=%d instance=%d, wcet=%0.1f\n", frames[f].jobs[i].taskNum, frames[f].jobs[i].splitNum, frames[f].jobs[i].instanceNum, frames[f].jobs[i].wcet);
		}
	}
}



/*
 * Stores the frame information into the frame info file.
 */
void
storeFrameInfo(Frame *frames, int numFrames, int frameSize)
{
	char *outputFileName = FRAME_INFO_FILE;
	FILE *outputFile;
	outputFile = fopen(outputFileName, "w");

	if(!outputFile)
	{
		fprintf(stderr, "The outputFile- periodicSchedule.txt did not open properly.\n");
		exit(0);
	}

	fprintf(outputFile, "%d\n", frameSize);
	fprintf(outputFile, "%d\n\n", numFrames);
	for (int f = 0; f < numFrames; ++f) // Iterates through all the frames.
	{
		fprintf(outputFile, "%0.1f\n", frames[f].slack);
		fprintf(outputFile, "%d\n", frames[f].numJobs);
		for (int i = 0; i < frames[f].numJobs; ++i) // Iterates through the jobs in that frame.
		{
			if (frames[f].jobs[i].splitNum != -1) // If the task has been split.
				fprintf(outputFile, "Y %d %d %d %0.1f\n", frames[f].jobs[i].taskNum, frames[f].jobs[i].splitNum, frames[f].jobs[i].instanceNum, frames[f].jobs[i].wcet);
			else
				fprintf(outputFile, "N %d %d %0.1f\n", frames[f].jobs[i].taskNum, frames[f].jobs[i].instanceNum, frames[f].jobs[i].wcet);
		}
		fprintf(outputFile, "\n");		
	}

	fclose(outputFile);

	return;
}



/*
 * Prints the aperiodic job info onto the output file.
 */
void
printAperiodicJobInfo(AperiodicJob *aperiodicJobs, int numJobs)
{
	fprintf(outputFile, "----------------------------------------------\n");
	fprintf(outputFile, "Aperiodic Job Info\nSorted based on arrival time.\n");

	for (int i = 0; i < numJobs; ++i) // Iterates through the various aperiodic jobs.
	{
		fprintf(outputFile, "Job=A%d, ArrivalTime=%0.1f, ExecutionTime=%0.1f\n", aperiodicJobs[i].jobNum, aperiodicJobs[i].arrivalTime, aperiodicJobs[i].wcet);
	}
}



/*
 * Prints the sporadic job info onto the output file.
 */
void
printSporadicJobInfo(SporadicJob *sporadicJobs, int numJobs)
{
	fprintf(outputFile, "----------------------------------------------\n");
	fprintf(outputFile, "Sporadic Job Info\nSorted based on arrival time.\n");

	for (int i = 0; i < numJobs; ++i) // Iterates through the various sporadic jobs.
	{
		fprintf(outputFile, "Job=S%d, ArrivalTime=%0.1f, ExecutionTime=%0.1f, Deadline=%0.1f, startFrame=%d, maxFrame=%d\n", sporadicJobs[i].jobNum, sporadicJobs[i].arrivalTime, sporadicJobs[i].wcet, sporadicJobs[i].deadline, sporadicJobs[i].startFrame, sporadicJobs[i].maxFrame);
	}
}



/*
 * Prints the frame info after reading from the file.
 */
void
printScheduleFrameInfo(ScheduleFrame *framesData, int numFrames, char *fileName)
{
	fprintf(outputFile, "----------------------------------------------\n");
	fprintf(outputFile, "Schedule Frame Info\n");
	fprintf(outputFile, "NumFrame: %d\n\n", numFrames);

	for (int f = 0; f < numFrames; ++f) // Iterates through the frames.
	{
		fprintf(outputFile, "Frame no: %d\n", f);
		fprintf(outputFile, "Slack: %0.1f\n", framesData[f].slack);
		fprintf(outputFile, "No of periodic jobs: %d\n", framesData[f].numPeriodicJobs);
		for (int i = 0; i < framesData[f].numPeriodicJobs; ++i) // Iterates through all the periodic jobs in that frame.
		{
			if (framesData[f].periodicJobs[i].splitNum != -1) // If the job has not been split.
				fprintf(outputFile, "J%d,%d: Instance: %d, wcet: %0.1f\n", framesData[f].periodicJobs[i].taskNum, framesData[f].periodicJobs[i].splitNum, framesData[f].periodicJobs[i].instanceNum, framesData[f].periodicJobs[i].wcet);
			else
				fprintf(outputFile, "J%d: Instance: %d, wcet: %0.1f\n", framesData[f].periodicJobs[i].taskNum, framesData[f].periodicJobs[i].instanceNum, framesData[f].periodicJobs[i].wcet);
		}

		fprintf(outputFile, "\n");
	}
}



/*
 * Prints the run time information about periodic, aperiodic and sporadic jobs
 * after the scheduler finished.
 */
void
printRunTimeSchedulingInfo(ScheduleFrame *framesData, int numFrames, int frameSize, AperiodicJob *aperiodicJobs, int numAperiodicJobs, SporadicJob *sporadicJobs, int numSporadicJobs)
{
	// Create and initialise the response time values.
	for (int i = 0; i < numTasks; ++i)
	{
		// Calloc initialises to 0.
		tasks[i].responseTimes = (float *) calloc(tasks[i].numInstances, sizeof(float));
		tasks[i].executionTimes = (float *) calloc(tasks[i].numInstances, sizeof(float));
	}

	// Find the values of the response times.
	for (int i = 0; i < numFrames; ++i) // Going through each frame.
	{
		for (int j = 0; j < framesData[i].numPeriodicJobs; ++j) // Going through each periodic job in that frame.
		{
			for (int k = 0; k < numTasks; ++k) // Going through all the tasks of the task set.
			{
				// Checking if the task instance corresponds to that task.
				if (framesData[i].periodicJobs[j].taskNum == tasks[k].taskNum)
				{
					framesData[i].periodicJobs[j].responseTime = framesData[i].periodicJobs[j].finishTime - tasks[k].period * framesData[i].periodicJobs[j].instanceNum;
					// printf("Job J%d,%d with finish time of %0.1f and a responseTime of %0.1f\n", framesData[i].periodicJobs[j].taskNum, framesData[i].periodicJobs[j].instanceNum, framesData[i].periodicJobs[j].finishTime, framesData[i].periodicJobs[j].responseTime);

					// The value will increase if that instance of the job had been split in to multiple jobs. The split job with the highest response time will become the response time for the overall task instance.
					if (tasks[k].responseTimes[framesData[i].periodicJobs[j].instanceNum] < framesData[i].periodicJobs[j].responseTime)
						tasks[k].responseTimes[framesData[i].periodicJobs[j].instanceNum] = framesData[i].periodicJobs[j].responseTime;

					// Updating the values of execution time.
					tasks[k].executionTimes[framesData[i].periodicJobs[j].instanceNum] += framesData[i].periodicJobs[j].executionTime;
					break;
				}
			}
		}
	}

	fprintf(outputFile, "\nJob Info after running the task set.\n");
	fprintf(outputFile, "\nPeriodic Task Schedule Info:\n");
	fprintf(outputFile, "\nResponse Times:\n");
	// Going through all the response times of the task instances of that job and also finding min, max and avg.
	for (int i = 0; i < numTasks; ++i)
	{
		fprintf(outputFile, "Response times of Task-%d: ", tasks[i].taskNum);
		// To find the min, max and avg.
		float min = FLT_MAX, max = -FLT_MAX, avg = 0;
		for (int j = 0; j < tasks[i].numInstances; ++j)
		{
			fprintf(outputFile, "%0.1f, ", tasks[i].responseTimes[j]);

			if (tasks[i].responseTimes[j] < min)
				min = tasks[i].responseTimes[j];

			if (tasks[i].responseTimes[j] > max)
				max = tasks[i].responseTimes[j];

			avg = avg + tasks[i].responseTimes[j];
		}
		fprintf(outputFile, "; Max: %0.1f, Min: %0.1f, Avg: %0.1f\n", max, min, avg / tasks[i].numInstances);
	}
	fprintf(outputFile, "\n");


	fprintf(outputFile, "Execution Times:\n");
	// Going through all the execution times of the task instances of that job and also finding min, max and avg.
	for (int i = 0; i < numTasks; ++i)
	{
		fprintf(outputFile, "Execution times of Task-%d with WCET=%0.1f: ", tasks[i].taskNum, tasks[i].wcet);
		// To find the min, max and avg.
		float min = FLT_MAX, max = 0, avg = 0;
		for (int j = 0; j < tasks[i].numInstances; ++j)
		{
			fprintf(outputFile, "%0.1f, ", tasks[i].executionTimes[j]);

			if (tasks[i].executionTimes[j] < min)
				min = tasks[i].executionTimes[j];

			if (tasks[i].executionTimes[j] > max)
				max = tasks[i].executionTimes[j];

			avg = avg + tasks[i].executionTimes[j];
		}
		fprintf(outputFile, "; Max: %0.1f, Min: %0.1f, Avg: %0.1f\n", max, min, avg / tasks[i].numInstances);
	}
	fprintf(outputFile, "\n");


	fprintf(outputFile, "Waiting Times:\n");
	// Going through all the waiting times of the task instances of that job and also finding min, max and avg.
	for (int i = 0; i < numTasks; ++i)
	{
		fprintf(outputFile, "Waiting times of Task-%d: ", tasks[i].taskNum);
		// To find the min, max and avg.
		float min = FLT_MAX, max = 0, avg = 0;
		for (int j = 0; j < tasks[i].numInstances; ++j)
		{
			fprintf(outputFile, "%0.1f, ", tasks[i].responseTimes[j] - tasks[i].executionTimes[j]);

			if (tasks[i].responseTimes[j] - tasks[i].executionTimes[j] < min)
				min = tasks[i].responseTimes[j] - tasks[i].executionTimes[j];

			if (tasks[i].responseTimes[j] - tasks[i].executionTimes[j] > max)
				max = tasks[i].responseTimes[j] - tasks[i].executionTimes[j];

			avg = avg + tasks[i].responseTimes[j] - tasks[i].executionTimes[j];
		}
		fprintf(outputFile, "; Max: %0.1f, Min: %0.1f, Avg: %0.1f\n", max, min, avg / tasks[i].numInstances);
	}
	fprintf(outputFile, "\n");


	fprintf(outputFile, "Disclaimer: Relative Response Jitter time will be the same as hyperperiod=%d as number of jobs of this task is one. Can only compare with the next job which is in the next hyperperiod.\n", frameSize * numFrames);

	// To calculate the jitters of the same response times.
	fprintf(outputFile, "Response Time Jitters:\n");
	for (int i = 0; i < numTasks; ++i)
	{
		fprintf(outputFile, "T%d: ", tasks[i].taskNum);
		float min = FLT_MAX, max = -FLT_MAX, avg = 0;
		for (int j = 0; j < tasks[i].numInstances; ++j)
		{
			if (tasks[i].responseTimes[j] < min)
				min = tasks[i].responseTimes[j];

			if (tasks[i].responseTimes[j] > max)
				max = tasks[i].responseTimes[j];
		}
		fprintf(outputFile, "Absolute RTJ: %0.1f, Relative RTJ: ", max - min);

		// Relative response time jitter cannot be defined for a task set with only one job.
		if (tasks[i].numInstances == 1)
		{
			fprintf(outputFile, "%d\n", numFrames * frameSize);
			continue;
		}

		// For finding the relative response time jitter.
		max = 0;
		for (int j = 0; j < tasks[i].numInstances - 1; ++j)
		{
			// Since we have to consider both cases of negative and positive difference.
			if (floatAbs(tasks[i].responseTimes[j+1] - tasks[i].responseTimes[j]) > max)
				// floatAbs of x returns the absolute value of x.
				max = floatAbs(tasks[i].responseTimes[j+1] - tasks[i].responseTimes[j]);
		}

		// For comparision between J0 and Jn (where n = numInstances - 1)
		// This is because after the hyperperiod, when the first job continues again, it will be right after the last job.
		int j = tasks[i].numInstances - 1;
		if (floatAbs(tasks[i].responseTimes[j] - tasks[i].responseTimes[0]) > max)
			// floatAbs of x returns the absolute value of x.
			max = floatAbs(tasks[i].responseTimes[j] - tasks[i].responseTimes[0]);

		fprintf(outputFile, "%0.1f\n", max);
	}
	fprintf(outputFile, "\n");



	// Response times for sporadic jobs that were accepted.
	fprintf(outputFile, "\n\nSporadic Job Schedule Info:\n");

	fprintf(outputFile, "\nDisclaimer: Sporadic jobs and Aperiodic jobs do not show execution time as the time they execute for is the same as the inputted value.\n");
	fprintf(outputFile, "Sporadic jobs that were accepted (list may be empty):\n");
	for (int i = 0; i < numSporadicJobs; ++i)
	{
		if (sporadicJobs[i].accepted  && !sporadicJobs[i].rejected)
			fprintf(outputFile, "S%d completed with response time: %0.1f and waiting time: %0.1f\n", sporadicJobs[i].jobNum, sporadicJobs[i].responseTime, sporadicJobs[i].arrivalTime + sporadicJobs[i].responseTime - sporadicJobs[i].wcet);
	}
	fprintf(outputFile, "\n");

	// Lists the sporadic jobs that were rejected.
	fprintf(outputFile, "Sporadic jobs that were rejected (list may be empty): ");
	for (int i = 0; i < numSporadicJobs; ++i)
	{
		if (!sporadicJobs[i].accepted  && sporadicJobs[i].rejected)
			fprintf(outputFile, "S%d ", sporadicJobs[i].jobNum);
	}
	fprintf(outputFile, "\n");


	// Response times for aperiodic jobs that finished.
	// Also lists aperiodic jobs that couldn't finish.
	fprintf(outputFile, "\n\nAperiodic Job Schedule Info:\n");
	for (int i = 0; i < numAperiodicJobs; ++i)
	{
		if (!aperiodicJobs[i].alive && aperiodicJobs[i].timeLeft == 0)
			fprintf(outputFile, "A%d has finished. Response time: %0.1f, Waiting time: %0.1f\n", aperiodicJobs[i].jobNum, aperiodicJobs[i].responseTime, aperiodicJobs[i].responseTime + aperiodicJobs[i].arrivalTime - aperiodicJobs[i].wcet);
		else
			fprintf(outputFile, "A%d could NOT finish.\n", aperiodicJobs[i].jobNum);
	}

	fprintf(outputFile, "\n");
	return;
}



/*
 * Prints the data about the number of preemption points and cache impact points.
 */
void
printPreemptionInfo()
{
	fprintf(outputFile, "------------------------------------------------\n");
	fprintf(outputFile, "\nPreemption Data\n\n");

	fprintf(outputFile, "Disclaimer: Number of preemption points will only calculate the non-periodic jobs that were preempted as periodic jobs are not preempted.\n\n");
	fprintf(outputFile, "Number of preemptions while executing sporadic and aperiodic jobs: %d\n\n", numPreemptions);
	fprintf(outputFile, "Disclaimer: The number of cache impact points counts the number of points in the schedule where a preemption/context swtich resulted in cache impact. It will not count a point in the schedule in which two periodic jobs of the same jobs run one after the other as the instr cache and most of the data cache will remain the same.\n");
	fprintf(outputFile, "Disclaimer: The number of cache impact points also contains the number of preemption points of non-periodic jobs.\n");
	fprintf(outputFile, "Number of cache impact points: %d\n", numCacheImpactPoints);
	fprintf(outputFile, "Number of points where the periodic job of the same task is running one after the other: %d\n\n", numContinuousPeriodicJobsOfSameTask);

	return;
}