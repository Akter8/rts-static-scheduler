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
	AperiodicJob *aperiodicJobs = aperiodicJobsInput(aperiodicJobsFile, &numAperiodicJobs);
	SporadicJob *sporadicJobs = sporadicJobsInput(sporadicJobsFile, &numSporadicJobs);
	ScheduleFrame *framesData = framesInput(frameFile, &numFrames, &frameSize);

	// Sort the aperiodic and sporadic jobs based on arrival time.
	sortAperiodicJobs(aperiodicJobs, 0, numAperiodicJobs - 1);
	sortSporadicJobs(sporadicJobs, 0, numSporadicJobs - 1);


	// Calculate the frames in which the sporadic jobs will arrive.
	// findSporadicFrames(sporadicJobs, numSporadicJobs, frameSize);
	for (int i = 0; i < numSporadicJobs; ++i)
	{
		// [startFrame, maxFrame);
		// [.] => can use that frame as well.
		// (.) => cannot use that frame.
		sporadicJobs[i].startFrame = sporadicJobs[i].arrivalTime / frameSize + 1;
		sporadicJobs[i].maxFrame = sporadicJobs[i].deadline / frameSize;
	}


	// Print the data onto the output file.
	printAperiodicJobInfo(aperiodicJobs, numAperiodicJobs);
	printSporadicJobInfo(sporadicJobs, numSporadicJobs);
	printScheduleFrameInfo(framesData, numFrames);


	// Start the scheduler.
	scheduler(framesData, numFrames, frameSize, aperiodicJobs, numAperiodicJobs, sporadicJobs, numSporadicJobs);


	// Free the data.
	free(aperiodicJobs);
	free(sporadicJobs);
	for (int i = 0; i < numFrames; ++i)
	{
		free(framesData[i].periodicJobs);
	}
	free(framesData);

	return 0;
}