/*
 * Author: Akhil Tarikere
 * Date: 9/3/20
 * 
 * Pre-Condition: 
 * 		
 *
 * Post-Condition:
 *	
 */

#include <stdio.h>
#include "driverMain.h"

int
main(int argc, char const *argv[])
{
	periodTaskDriver(argc, argv);

	nonPeriodicJobDriver();

	return 0;
}