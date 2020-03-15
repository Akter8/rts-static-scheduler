/*
 * Author: Akhil Tarikere
 * Date: 9/3/20
 * 
 * Pre-Condition: Takes in the CLI.
 * 		
 *
 * Post-Condition: Runs the entire program.
 *	
 */


// This is the main driver.


#include "driverMain.h"


int
main(int argc, char const *argv[])
{

	char *outputFileName = OUTPUT_FILE;
	// Opening the output file in write mode so that everything is overwritten.
	FILE *outputFile = fopen(outputFileName,"w");
	fclose(outputFile);

	periodTaskDriver(argc, argv);

	nonPeriodicJobDriver();

	

	return 0;
}