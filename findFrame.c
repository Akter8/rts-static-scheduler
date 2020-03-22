/*
 * Author: Akhil Tarikere
 * Date: 16/3/20
 * 
 * Pre-Condition: Takes in the inputs of task instances.
 * 		
 * Post-Condition: Calculates which frames the jobs will have to execute in.
 * And if required, it also slices jobs to fit them into the frames using the INF algorithm.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "functionPeriodic.h"
#include "configuration.h"



/*
 * Given a job that has to be split and the slacks in each frame.
 * Finds and returns in which frames and how many splits the job will undergo.
 */
void
splitJobs(TaskInstance job, Frame *frames, int numFrames)
{

	/* 
	Algorithm for splitting jobs:
		Sort frames based on the decreasing amount of slack they have.
		Find the number of frames required for the job.
		Update the meta data for the job and the frames.
	
	End
	*/

	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	float wcet = job.wcet;

	int numValidFrames = job.maxFrame - job.startFrame;
	Frame *sortedFrames = (Frame *) malloc(sizeof(Frame) * numValidFrames);

	// Finding the set of valid frames-- the set of frames in which a job can execute in.
	for (int i = job.startFrame; i < job.maxFrame; ++i)
	{
		sortedFrames[i - job.startFrame] = frames[i];
	}


	// Sorting the frames based on the amount of slack in them.
	sortFramesOnSlack(sortedFrames, 0, numValidFrames - 1);

	fprintf(outputFile, "\nSplitting job J%d instance=%d\n", job.taskNum, job.instanceNum);
	fprintf(outputFile, "Initally has a wcet of: %0.1f\n", job.wcet);
	fprintf(outputFile, "With these execution times: \n");

	int i;
	for (i = 0; i < numValidFrames; ++i) // Iterating through the sorted frames.
	{
		if (sortedFrames[i].slack <= 0)
		{
			fprintf(stderr, "Cannot schedule task set.\n");
			exit(0);
		}

		if (wcet <= 0) // Check for break.
		{
			// fprintf(outputFile, "wcet = %0.1f\n", wcet);
			break;
		}

		// Creating a new task instance and initialising it.
		TaskInstance newJob;
		newJob.instanceNum = job.instanceNum;
		newJob.taskNum = job.taskNum;
		newJob.splitNum = i;
		newJob.alive = true;
		newJob.startFrame = job.startFrame;
		newJob.maxFrame = job.maxFrame;


		// Need to have another split.
		if (wcet > sortedFrames[i].slack)
		{
			newJob.wcet = sortedFrames[i].slack;
			wcet -= sortedFrames[i].slack;
			sortedFrames[i].slack -= newJob.wcet;
		}
		else // This is the last split.
		{
			newJob.wcet = wcet;
			wcet = 0;
			sortedFrames[i].slack = 0;
		}

		sortedFrames[i].jobs[sortedFrames[i].numJobs] = newJob;
		sortedFrames[i].numJobs++;

		// Updating the main copy of frames data.
		for (int f = job.startFrame; f < job.maxFrame; ++f)
		{
			if (frames[f].frameNum == sortedFrames[i].frameNum)
			{
				frames[f].jobs[frames[f].numJobs] = newJob;
				frames[f].numJobs++;
				frames[f].slack = sortedFrames[i].slack;
				break;
			}
		}

		fprintf(outputFile, "%0.1f in frame=%d\n", newJob.wcet, sortedFrames[i].frameNum);
	}

	fprintf(outputFile, "Split into %d jobs\n", i);

	job.splitNum = i;

	free(sortedFrames);

	fflush(outputFile);
	fclose(outputFile);

	return;
}



/*
 * Given a set of jobs and an uninitialised set of frames.
 * It calculates which job will run in which frame.
 * Also finds out if a job needs to be split.
 */
void
findFrame(TaskInstance *jobs, int numJobs, Frame *frames, int frameSize, int numFrames)
{
	// Initialising the jobs.
	for (int i = 0; i < numJobs; ++i)
	{
		jobs[i].splitNum = -1;
	}

	// Initialising the frames.
	for (int i = 0; i < numFrames; ++i)
	{
		frames[i].frameNum = i;
		frames[i].numJobs = 0;
		frames[i].jobs = (TaskInstance *) malloc(sizeof(TaskInstance) * numJobs);
		frames[i].slack = frameSize;
	}

	/* 
	Algorithm for finding frame:

		For every job j:
			Find the valid frame with the most amount of slack to Place this job in such that wcet < slack.
			If a valid frame was found:
				Add this job to that frame.
				Update slack and other metadata of the frame.
			Else:
				Need to split jobs
	
	End
	*/

	FILE *outputFile = fopen(OUTPUT_FILE, "a");
	fprintf(outputFile, "----------------------------\n");
	fprintf(outputFile, "Finding frames for all jobs.\n");

	for (int i = 0; i < numJobs; ++i) // Iterates over jobs.
	{
		int minIndex = -1;
		float minValue = INT_MAX;
		for (int f = jobs[i].startFrame; f < jobs[i].maxFrame; ++f) // Iterates over frames to find the one with the least slack.
		{
			// Updating the minimum.
			if (frames[f].slack >= jobs[i].wcet && frames[f].slack < minValue)
			{
				minValue = frames[f].slack;
				minIndex = f;
			}
		}

		if (minIndex == -1) // Job needs to be split.
		{
			// fprintf(outputFile, "Job being split.\n");
			fflush(outputFile);
			splitJobs(jobs[i], frames, numFrames);
		}
		else // No split required, can be put in directly.
		{
			fprintf(outputFile, "Job J%d instance %d going into frame %d. wcet=%0.1f\n", jobs[i].taskNum, jobs[i].instanceNum, minIndex, jobs[i].wcet);
			// Updating metadata.
			frames[minIndex].jobs[frames[minIndex].numJobs] = jobs[i];
			frames[minIndex].numJobs++;
			frames[minIndex].slack -= jobs[i].wcet;
		}

	}

	// Reducing the memory footprint.
	for (int i = 0; i < numFrames; ++i)
	{
		frames[i].jobs = realloc(frames[i].jobs, sizeof(TaskInstance) * frames[i].numJobs);
	}

	fclose(outputFile);

	return;
}