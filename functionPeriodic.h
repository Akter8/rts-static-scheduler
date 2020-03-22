#include <stdio.h>

// Contains the ADTs of periodic part.
#include "structPeriodic.h"


// The periodic driver (driverPeriodicTask.c)
int periodicTaskDriver(int, char **); 

// inputs.c
Task *periodicTaskInput(FILE *, int *); // Input the task data.

// Sorting tasks based on their periods. (sort.c)
void sortTasks(Task *, int, int);
void mergeTasks(Task *, int, int, int);

// The three conditions for finding valid frame sizes. (frameConditions.c)
int frameSizeCondition1(Task *, int); 
int frameSizeCondition2(int, int *);
int frameSizeCondition3(int *, Task *, int, int, int *);

// Creates all task instances of all tasks for the hyperperiod. (createTaskInstances.c)
void createTaskInstances(Task *, TaskInstance *, int, int, int, int);

// For splitting jobs and fitting jobs into frames (findFrame.c)
void findFrame(TaskInstance *, int, Frame *, int, int);
void splitJobs(TaskInstance, Frame *, int);

// Functions required for sorting frames based one their slack. (sort.c)
void sortFramesOnSlack(Frame *, int, int);
void mergeFramesOnSlack(Frame *, int, int, int);

// For printing and storing the data structures. (printInfo.c)
void printTaskInfo(Task *, int);
void printJobInfo(TaskInstance *, int);
void printFrameInfo(Frame *, int, int);
void storeFrameInfo(Frame *, int, int);

// utilityFunctions.c
void firstCheck(int, char *[]); // Does the initial input checks.
FILE *inputFileCheck(char *);
float calculateCpuUtilisation(Task *, int);
void checkCpuUtilisation(Task*, int);
int findHyperPeriod(Task *, int);
int gcd(int, int);
int findNumJobs(Task *, int, int);