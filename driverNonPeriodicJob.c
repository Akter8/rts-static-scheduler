/*
 * Author: Akhil Tarikere
 * Date: 10/3/20
 * 
 * Pre-Condition: 
 * 		
 *
 * Post-Condition:
 *	
 */

#include "driverNonPeriodicJob.h"


int
nonPeriodicJobDriver()
{
	char *aperiodicJobFileName = "aperiodicJobs.txt";
	char *sporadicJobFileName = "sporadicJobs.txt";
	char *frameFileName = "periodicSchedule.txt";

	// Checking to see if the files are properly opened.
	FILE *aperiodicJobsFile = inputFileCheck(aperiodicJobFileName);
	FILE *sporadicJobsFile = inputFileCheck(sporadicJobFileName);
	// FILE *frameFile = inputFileCheck(frameFileName);


	int numAperiodicJobs, numSporadicJobs, numFrames;

	// Load the data from input files onto the right data structures.
	AperiodicJob *aperiodicJobs = aperiodicJobsInput(aperiodicJobsFile, &numAperiodicJobs);
	SporadicJob *sporadicJobs = sporadicJobsInput(sporadicJobsFile, &numSporadicJobs);

	// Load data from periodicSchedule.txt
	//
	//
	//


	return 0;
}