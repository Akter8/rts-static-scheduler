#include <stdio.h>
#include "structPeriodic.h"

int periodicTaskDriver(int, char **);
float calculateCpuUtilisation(Task *, int);
int findHyperPeriod(Task *, int);
int gcd(int, int);
int frameSizeCondition1(Task *, int);
int frameSizeCondition2(int, int *);
int frameSizeCondition3(int *, Task *, int, int, int *);
void createTaskInstances(Task *, TaskInstance *, int, int, int, int);
void printTaskInfo(Task *, int);
void printJobInfo(TaskInstance *, int);
void printFrameInfo(Frame *, int, int);
void firstCheck(int, char *[]);
FILE *inputFileCheck(char *);
Task *periodicTaskInput(FILE *, int *);
void checkCpuUtilisation(Task*, int);
int findNumJobs(Task *, int, int);
void storeFrameInfo(Frame *, int, int);
void sortTasks(Task *, int, int);
void mergeTasks(Task *, int, int, int);
void findFrame(TaskInstance *, int, Frame *, int, int);
void splitJobs(TaskInstance, Frame *, int);
void sortFramesOnSlack(Frame *, int, int);
void mergeFramesOnSlack(Frame *, int, int, int);