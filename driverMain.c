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

#include <stdio.h>
#include "driverMain.h"

int
main(int argc, char const *argv[])
{
	periodTaskDriver(argc, argv);

	nonPeriodicJobDriver();

	return 0;
}