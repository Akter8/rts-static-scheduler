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

#include "driverMain.h"


int
main(int argc, char const *argv[])
{
	FILE *outputFile = fopen(OUTPUT_FILE, "w"); // Opening the output file in write mode so that everything is overwritten.
	fclose(outputFile);

	periodTaskDriver(argc, argv);

	nonPeriodicJobDriver();

	return 0;
}