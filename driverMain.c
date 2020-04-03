/*
 * Author: Akhil Tarikere
 * Date: 9/3/20
 * 
 * Pre-Condition: Takes in the CLI arguments.
 * 		
 *
 * Post-Condition: Runs the entire program.
 *	
 */

#include <stdio.h>
#include "functionPeriodic.h"
#include "functionNonPeriodic.h"
#include "configuration.h"

// ADTs for periodic part.
int numTasks;
Task *tasks;
TaskInstance *jobs;
Frame *frames;

// ADTs for non-periodic part.
AperiodicJob *aperiodicJobs;
SporadicJob *sporadicJobs;
ScheduleFrame *framesData;

FILE *outputFile;

int
main(int argc, char *argv[])
{
	outputFile = fopen(OUTPUT_FILE, "w"); // Opening the output file in write mode so that everything is overwritten.

	// Calling the periodic task part.
	periodicTaskDriver(argc, argv);

	// Calling the non-periodic and scheduling part.
	nonPeriodicJobDriver();

	// Program has finished.
	fprintf(outputFile, "-------------------THE END-------------------\n");
	fclose(outputFile);

	return 0;
}