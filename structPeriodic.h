#include <stdbool.h>

typedef struct
{
	int taskNum;
	int period;
	float deadline;
	float wcet;
	int numOfSplits;
	int *splits;

}
Task;

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

typedef struct 
{
	int frameNum;
	int numJobs;
	TaskInstance *jobs;
	float slack;
}
Frame;