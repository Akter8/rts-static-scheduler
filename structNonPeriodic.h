/*
 * Author: Akhil Tarikere
 * Date: 10/3/20
 * 
 * To store the input data in appropriate data structures.
 * Contains the ADTs of non-periodic and scheduling parts.
 *	
 */

#include <stdbool.h>


/*
 * The ADT of an aperiodic job.
 */
typedef struct
{
	int jobNum;
	float arrivalTime;
	float wcet;
	bool alive;
	float timeLeft;
	float responseTime;
}
AperiodicJob;


/*
 * The ADT of a Sporadic job.
 */
typedef struct
{
	int jobNum;
	float arrivalTime;
	float wcet;
	float deadline;
	// [startFrame, maxFrame);
	// [.] => can use that frame as well.
	// (.) => cannot use that frame.
	int startFrame;
	int maxFrame;
	bool alive;
	bool accepted;
	bool rejected;
	float timeLeft;
	float responseTime;
}
SporadicJob;



/*
 * The ADT of periodic job (job and not a task).
 */
typedef struct
{
	int taskNum;
	int instanceNum;
	int splitNum;
	float wcet;
	bool alive;
	float finishTime;
	float responseTime;
	float executionTime;
}
PeriodicJob;


/*
 * The ADT of a frame that will be used by the scheduler.
 */
typedef struct
{
	int frameNum;
	int numPeriodicJobs;
	PeriodicJob *periodicJobs;
	float slack;
}
ScheduleFrame;