#include "struct.h"
#include <stdio.h>

float calculateCpuUtilisation(Task *, int);
int findHyperPeriod(Task *, int);
int gcd(int, int);
int frameSizeCondition1(Task *, int);
int frameSizeCondition2(int, int *);
int frameSizeCondition3(int *, Task *, int, int, int *);
void createTaskInstances(Task *, TaskInstance *, int, int, int, int);
void calculateSchedule(TaskInstance *, int, int, int, Frame *);
void printTaskInfo(Task *, int);
void printJobInfo(TaskInstance *, int);
void printFrameInfo(Frame *, int, int);
void firstCheck(int, char *[]);
FILE *inputFileCheck(char *);
Task *periodicTaskInput(FILE *, int *);
void checkCpuUtilisation(Task*, int);
void conditionChecker(Task *, int, int, int *, int *, int *, int *, int *);
int splitTasks(int, int, int *, Task *, int);
int findNumJobs(Task *, int, int);
void storeFrameInfo(Frame *, int, int);
void sortTasks(Task *, int, int);
void mergeTasks(Task *, int, int, int);