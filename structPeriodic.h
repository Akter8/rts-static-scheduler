/*
 * Author: Akhil Tarikere
 * Date: 6/3/20
 *
 * Contains the ADT related to periodic tasks and schedule.
 */

#include <stdbool.h>


/*
 * ADT of a Task.
 */
typedef struct
{
	int taskNum;
	int period;
	float deadline;
	float wcet;
	int numInstances;
	float *responseTimes;
	float *executionTimes;
}
Task;


/*
 * ADT of a Task instance or a job.
 */
typedef struct
{
	int startFrame;
	int maxFrame;
	float wcet;
	int taskNum;
	int splitNum;
	int instanceNum;
	bool alive;
}
TaskInstance;


/*
 * ADT of a frame.
 */
typedef struct 
{
	int frameNum;
	int numJobs;
	TaskInstance *jobs;
	float slack;
}
Frame;