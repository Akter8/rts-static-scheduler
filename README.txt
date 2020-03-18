# RTS-Assignment-2
Name: Akhil Tarikere
ID: 2017A7PS1916G
Given a set of periodic, aperiodic and sporadic jobs in an RTS, finding a feasible schedule for the task set with valid frame sizes.


### Question
For a given in-phase input task set consisting of periodic tasks [Read the input task set from a file whose name is a command line argument], find
1. Hyper-period and the best frame size possible [if required, split the task(s)] and provide a periodic schedule per every frame. [Get frame table with the number of entries equal to the number of frames]. Store the table in a file named periodicSchedule.txt.
2. Read aperiodic jobs [from a file], Sporadic jobs [from another file] and periodic schedule [from periodicSchedule.txt] and execute them using:
	1. Aperiodic jobs with minimum average response time [using Slack stealing approach]
	2. Sporadic jobs with cyclic EDF sporadic scheduler
3. Enhance your implementation by relaxing the assumption on WCET. Assume that the actual execution time of a job can be lesser than or equal to the WCET of it (20% to 100% determined using a pseudo random function).


### Tasks to be done
- [ ] Remove the bug in the scheduler module.
- [ ] Algorithm of selecting the jobs in a frame (INF).
- [ ] While selecting a job that has been split, making sure that the there is an order that is followed.
- [ ] Find a way to slice multiple jobs more efficiently.
- [ ] Make splitPeriodicTask in split.c return the frame size.
- [ ] Combine the task of slicing and selecting jobs for a frame.
- [ ] Make the fprintfs in the scheduler neat.
- [ ] Check if there are any warnings while compiling.
- [ ] Add pre-condition and post-condition to every function.
- [ ] Print everything to the outputfile.
- [ ] Finish the description of files part of README.
- [ ] Finish the README.
- [X] Check for negative periods execution time and deadlines.
- [ ] Comments in function.h


### Tasks Completed and description of the structure of the program
* Define the relevant ADTs.
* Input and sort the data into the various ADTs.
* Find CPU utilisation and Hyperperiod.
* Create the various task instances (jobs) for periodic tasks.
* Slice jobs and fit them into appropriate frames.
* Store the schedule onto periodicSchedule.txt
* Input and sort the data based on arrival time for aperiodic and sporadic jobs.
* Schedule the frames till the hyperperiod with the help of psuedo-random numbers for periodic jobs.


### Assumptions
1. None of the jobs-- periodic or non-periodic overrun.
2. Aperiodic and sporadic jobs have a fixed running time unlike periodic jobs.
3. Release time jitters are zero for all jobs.
4. Scheduling time is zero for all frames.


### Limitations
1. Since the float data shown on the screen has a granularity of 0.1, various times printed on the screen may make it seem that the job may have not executed (Example: Job running for 13.1 to 13.1 might be printed on the screen. But in reality the job may have run from 13.12 to 13.19).
2. Since the periodic jobs might execute right after one another, the same percentage (20 - 100%) of execution of all the periodic jobs may occur.
3. This simulation does not consider the scenario when aperiodic and sporadic jobs also have a variable time of execution.
4. This simulation also does not consider aperiodic and sporadic tasks, we only consider them as single task instances.
5. Scheduling only till hyperperiod. This might not be the best way to simulate sporadic and aperiodic jobs as their deadlines might be after the hyperperiod.
6. This simulation does not take into account the scenario when there is a frame over-run by any of the jobs that execute.
7. Once aperiodic, periodic and sporadic jobs are done in a frame, this simulation looks into the aperiodic queue to see if there are any aperiodic jobs arriving this frame. If there is not aperiodic jobs arriving that frame, then it skips to the end of the frame. (Ex: Frame size = 4, current time = 15. If all jobs are done. Then it searches to see if there are any aperiodic jobs coming in before time = 16).
8. The period of a periodic task is accepted only as an integer. This is because finding hyperperiod for float values will result in very large hyperperiods. Another reason is that while loading these float values onto the system, the values may not be stored without losing some data.
9. The simulation does not take into consideration the amount of slack while scheduling aperiodic or extra sporadic jobs. It reality it does not make sense to schedule such jobs if the slack is not at least enough to finish a good amount of the jobs.
10. The release time jitters and the scheduling time for every frame is considered to be zero, which is not the case in practical systems.
11. A sporadic job may be rejected initially because there may not be enough time to run it. But periodic jobs not running for their 100% worst-case time might produce enough slack to be able to run it.



### What could have been added
1. A sporadic job need not be rejected right away. If it is rejected in the arrival frame, the simulation could give the result of the acceptance test in the next frame (Because of the slack generated by periodic jobs).




### Known bugs



### Description of files
* driverMain.c - The main driver that calls other modules.
* driverPeriodicTask.c - The driver module for the periodic tasks part.
* driverNonPeriodicTask.c - The driver module associated with the non-periodic and scheduling part.
* configuration.h - Contains the configuration files of the repo.
* structPeriodic.h - ADTs associated with the periodic tasks.
* structNonPeriodic.h - ADTs associated with scheduling and non-periodic jobs.
* functionPeriodic.h - List of functions associated with the periodic tasks.
* functionNonPeriodic.h - List of functions associated with non-periodic jobs.
* inputs.c - Stores the data from input files into the ADTs.
* checks.c - Functions related to some basic input checks.
* frameConditions.c - Module that finds and returns possible frame sizes based on the three conditions.
* sort.c - Sorts ADTs based on some parameter (usually arrival time).
* createTaskInstances.c - Module that creates instances of periodic tasks.
* findFrame.c - Finds which jobs will execute in which frame. Also does the role of splitting jobs if required.
* utilityFunctions.c - Some utility functions.
* printInfo.c - Prints the data in the ADTs onto the output file.
* scheduler.c - The module associated with the scheduling of periodic + non-periodic jobs


### How to compile
On the CLI run "make" to compile and "make clean" to clean the compilation.
One can change the required flags and the compiler in the Makefile if required.



## Inputs required
1. periodicTasks.txt (CLI argument): First line should have the number of tasks in the task set. Subsequent lines should have 3 integers- period, wcet and relative deadline respectively.
2. APERIODIC_JOB_FILE: First line should have the number of tasks in the task set. Subsequent lines should have 2 integers- arrival time and execution time respectively.
3. SPORADIC_JOB_FILE: First line should have the number of tasks in the task set. Subsequent lines should have 3 integers- arrival time, execution time and absolute deadline respectively.