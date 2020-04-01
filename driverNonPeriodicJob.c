/*
 * Author: Akhil Tarikere
 * Date: 10/3/20
 * 
 * Pre-Condition: Takes inputs from 3 files- periodic (written by the PeriodicJobDriver()), aperiodic and sporadic.
 * 		
 *
 * Post-Condition: Checks to see if aperiodic and sporadic jobs can be accomodated.
 *	
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functionPeriodic.h"
#include "functionNonPeriodic.h"
#include "configuration.h"

extern int numTasks;
extern Task *tasks;
extern AperiodicJob *aperiodicJobs;
extern SporadicJob *sporadicJobs;
extern ScheduleFrame *framesData;

int numPreemptions = 0;
int numCacheImpactPoints = 0;
int numContinuousPeriodicJobsOfSameTask = 0;


int
nonPeriodicJobDriver()
{
	// File name initialisation.
	char *aperiodicJobFileName = APERIODIC_JOB_FILE;
	char *sporadicJobFileName = SPORADIC_JOB_FILE;
	char *frameFileName = FRAME_INFO_FILE;

	// Checking to see if the files are properly opened.
	FILE *aperiodicJobsFile = inputFileCheck(aperiodicJobFileName);
	FILE *sporadicJobsFile = inputFileCheck(sporadicJobFileName);
	FILE *frameFile = inputFileCheck(frameFileName);


	int numAperiodicJobs, numSporadicJobs, numFrames, frameSize;

	// Load the data from input files onto the right data structures.
	aperiodicJobs = aperiodicJobsInput(aperiodicJobsFile, &numAperiodicJobs);
	sporadicJobs = sporadicJobsInput(sporadicJobsFile, &numSporadicJobs);
	framesData = framesInput(frameFile, &numFrames, &frameSize);

	// Sort the aperiodic and sporadic jobs based on arrival time.
	sortAperiodicJobs(aperiodicJobs, 0, numAperiodicJobs - 1);
	sortSporadicJobs(sporadicJobs, 0, numSporadicJobs - 1);


	// Calculate the frames in which the sporadic jobs will arrive.
	for (int i = 0; i < numSporadicJobs; ++i)
	{
		// [startFrame, maxFrame);
		// [.] => can use that frame as well.
		// (.) => cannot use that frame.
		sporadicJobs[i].startFrame = sporadicJobs[i].arrivalTime / frameSize;
		sporadicJobs[i].maxFrame = sporadicJobs[i].deadline / frameSize;
	}


	// Print the data onto the output file.
	printAperiodicJobInfo(aperiodicJobs, numAperiodicJobs);
	printSporadicJobInfo(sporadicJobs, numSporadicJobs);
	printScheduleFrameInfo(framesData, numFrames, OUTPUT_FILE);

	// Prints data onto the static schedule data file.
	printScheduleFrameInfo(framesData, numFrames, FRAME_INFO_HUMAN_READABLE_FILE);


	// Start the scheduler.
	scheduler(framesData, numFrames, frameSize, aperiodicJobs, numAperiodicJobs, sporadicJobs, numSporadicJobs);
	

	// Prints the run time scheduler information about all types of jobs.
	printRunTimeSchedulingInfo(framesData, numFrames, frameSize, aperiodicJobs, numAperiodicJobs, sporadicJobs, numSporadicJobs);

	printPreemptionInfo();

	// Free the data.
	free(aperiodicJobs);
	free(sporadicJobs);
	for (int i = 0; i < numFrames; ++i)
	{
		free(framesData[i].periodicJobs);
	}
	free(framesData);
	for (int i = 0; i < numTasks; ++i)
	{
		free(tasks[i].responseTimes);
		free(tasks[i].executionTimes);
	}
	free(tasks);

	return 0;
}