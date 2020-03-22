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
#include "functionPeriodic.h"
#include "functionNonPeriodic.h"
#include "configuration.h"


/*
 * Prints Task information onto the output file.
 */
void
printTaskInfo(Task *tasks, int numTasks)
{
	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "----------------------------\n");
	fprintf(outputFile, "Task Info\nSorted based on period.\n");
	fprintf(outputFile, "Num Tasks: %d\n", numTasks);
	for (int i = 0; i < numTasks; ++i) // Iterates through every task.
	{
		fprintf(outputFile, "Task=T%d, period=%d, wcet=%0.1f, deadline=%0.1f\n", tasks[i].taskNum, tasks[i].period, tasks[i].wcet, tasks[i].deadline);
	}

	fclose(outputFile);
}



/*
 * Prints the job information onto the output file.
 */
void
printJobInfo(TaskInstance * jobs, int numJobs)
{
	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "----------------------------\n");
	fprintf(outputFile, "Job Info\n");
	fprintf(outputFile, "Num Jobs: %d\n", numJobs);
	for (int i = 0; i < numJobs; ++i) // Iterates thorugh every job.
	{
		if (jobs[i].splitNum != -1) // If the job has been split.
			fprintf(outputFile, "J%d split=%d TaskInstance=%d: startFrame=%d, maxFrame=%d, wcet=%0.1f, alive=%d\n", jobs[i].taskNum, jobs[i].splitNum, jobs[i].instanceNum, jobs[i].startFrame, jobs[i].maxFrame, jobs[i].wcet, jobs[i].alive);
		else
			fprintf(outputFile, "J%d TaskInstance=%d: startFrame=%d, maxFrame=%d, wcet=%0.1f, alive=%d\n", jobs[i].taskNum, jobs[i].instanceNum, jobs[i].startFrame, jobs[i].maxFrame, jobs[i].wcet, jobs[i].alive);
	}

	fclose(outputFile);
}



/*
 * Prints the frame information onto the outputfile.
 */
void
printFrameInfo(Frame * frames, int numFrames, int frameSize)
{
	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "----------------------------\n");
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
	fclose(outputFile);
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
	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "----------------------------\n");
	fprintf(outputFile, "Aperiodic Job Info\nSorted based on arrival time.\n");

	for (int i = 0; i < numJobs; ++i) // Iterates through the various aperiodic jobs.
	{
		fprintf(outputFile, "Job=A%d, ArrivalTime=%0.1f, ExecutionTime=%0.1f\n", aperiodicJobs[i].jobNum, aperiodicJobs[i].arrivalTime, aperiodicJobs[i].wcet);
	}

	fclose(outputFile);
}



/*
 * Prints the sporadic job info onto the output file.
 */
void
printSporadicJobInfo(SporadicJob *sporadicJobs, int numJobs)
{
	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "----------------------------\n");
	fprintf(outputFile, "Sporadic Job Info\nSorted based on arrival time.\n");

	for (int i = 0; i < numJobs; ++i) // Iterates through the various sporadic jobs.
	{
		fprintf(outputFile, "Job=S%d, ArrivalTime=%0.1f, ExecutionTime=%0.1f, Deadline=%0.1f, startFrame=%d, maxFrame=%d\n", sporadicJobs[i].jobNum, sporadicJobs[i].arrivalTime, sporadicJobs[i].wcet, sporadicJobs[i].deadline, sporadicJobs[i].startFrame, sporadicJobs[i].maxFrame);
	}

	fclose(outputFile);
}



/*
 * Prints the frame info after reading from the file.
 */
void
printScheduleFrameInfo(ScheduleFrame *framesData, int numFrames)
{
	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "----------------------------\n");
	fprintf(outputFile, "Schedule Frame Info\n");

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

	fclose(outputFile);
}