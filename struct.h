typedef struct
{
	int period;
	int deadline;
	int wcet;
	int numOfSplits;
	int *splits;

}
Task;

typedef struct
{
	int startFrame;
	int maxFrame;
	int wcet;
	int taskNum;
	int splitNum;
	int instanceNum;
	bool alive;
}
TaskInstance;

typedef struct 
{
	int numFrame;
	int numJobs;
	TaskInstance *jobs;
	float slack;
}
Frame;