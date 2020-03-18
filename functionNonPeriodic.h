#include "structNonPeriodic.h"

int nonPeriodicJobDriver();
AperiodicJob *aperiodicJobsInput(FILE *, int *);
SporadicJob *sporadicJobsInput(FILE *, int *);
ScheduleFrame *framesInput(FILE *, int *, int *);
void sortSporadicJobs(SporadicJob *, int, int);
void mergeSporadicJobs(SporadicJob *, int, int, int);
void sortAperiodicJobs(AperiodicJob *, int, int);
void mergeAperiodicJobs(AperiodicJob *, int, int, int);
void printAperiodicJobInfo(AperiodicJob *, int);
void printSporadicJobInfo(SporadicJob *, int);
void printScheduleFrameInfo(ScheduleFrame *, int);
void scheduler(ScheduleFrame *, int, int, AperiodicJob *, int, SporadicJob *, int);
float runPeriodic(PeriodicJob);
void sortSporadicJobsOnEDF(SporadicJob *, int, int);
void mergeSporadicJobsOnEDF(SporadicJob *, int, int, int);