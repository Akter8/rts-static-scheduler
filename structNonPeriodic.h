typedef struct
{
	float arrivalTime;
	float wcet;
}
AperiodicJob;


typedef struct
{
	float arrivalTime;
	float wcet;
	float deadline;
}
SporadicJob;

typedef struct
{
	float arrivalTime;
	float wcet;
	float deadline;
}
PeriodicJob;

typedef struct
{
	int frameNum;
	int numPeriodicJobs;
	int numSporadicJobs;
	SporadicJob *sporadicJobs;
	PeriodicJob *periodicJobs;
	float slack;
}
ScheduleFrame;