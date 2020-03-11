#include "structNonPeriodic.h"

AperiodicJob *aperiodicJobsInput(FILE *, int *);
SporadicJob *sporadicJobsInput(FILE *, int *);
ScheduleFrame *framesInput(FILE *, int *, int *);
void sortSporadicJobs(SporadicJob *, int, int);
void mergeSporadicJobs(SporadicJob *, int, int, int);
void sortAperiodicJobs(AperiodicJob *, int, int);
void mergeAperiodicJobs(AperiodicJob *, int, int, int);
void printAperiodicJobInfo(AperiodicJob *, int);
void printSporadicJobInfo(SporadicJob *, int);