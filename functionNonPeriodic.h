// Contains the ADTs of the non-periodic and scheduling part.
#include "structNonPeriodic.h"

// The non-periodic and scheduling driver.
int nonPeriodicJobDriver();

// Takes in the inputs. (inputs.c)
AperiodicJob *aperiodicJobsInput(FILE *, int *);
SporadicJob *sporadicJobsInput(FILE *, int *);
ScheduleFrame *framesInput(FILE *, int *, int *);

// Sorts sporadic and aperiodic jobs based on arrival time. (sort.c) 
void sortSporadicJobs(SporadicJob *, int, int);
void mergeSporadicJobs(SporadicJob *, int, int, int);
void sortAperiodicJobs(AperiodicJob *, int, int);
void mergeAperiodicJobs(AperiodicJob *, int, int, int);

// Sorts sporadic jobs based on EDF (used while scheduling sporadic jobs)
// (sort.c)
void sortSporadicJobsOnEDF(SporadicJob *, int, int);
void mergeSporadicJobsOnEDF(SporadicJob *, int, int, int);

// Prints the data structures onto the output file. (printInfo.c)
void printAperiodicJobInfo(AperiodicJob *, int);
void printSporadicJobInfo(SporadicJob *, int);
void printScheduleFrameInfo(ScheduleFrame *, int);
void printRunTimeSchedulingInfo(ScheduleFrame *, int, int, AperiodicJob *, int, SporadicJob *, int);

// The scheduler module (scheduler.c)
bool checkAperiodicJob(AperiodicJob *, int, float);
float runPeriodic(PeriodicJob);
SporadicJob *findSporadicJobsInFrame(SporadicJob *, int, int *, int, float, float *, ScheduleFrame *, int);
float runSporadic(SporadicJob *, int , int *, float);
float runPeriodic(PeriodicJob);
void scheduler(ScheduleFrame *, int, int, AperiodicJob *, int, SporadicJob *, int);