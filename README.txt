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
- [ ] Make the fprintfs in the scheduler neat.
- [ ] Check if there are any warnings while compiling.
- [ ] Finish the README.
- [ ] Comments and testing of periodicTaskDriver.c in the 3 checks part.
- [ ] Test the program with lots of test cases for bugs.
- [X] Add waiting time output for all jobs - added waiting time and execution time.
- [X] Static output in another file - added periodicSchedule_humanReadable.txt
- [X] Remove unnecessary fopens.
- [ ] Remove unnecessary fflushs.


### Tasks Completed
* Define the relevant ADTs.
* Input and sort the data into the various ADTs.
* Find CPU utilisation and Hyperperiod.
* Create the various task instances (jobs) for periodic tasks.
* Slice jobs and fit them into appropriate frames (Using an INF-like algorithm).
* Store the schedule onto periodicSchedule.txt
* Input and sort the data based on arrival time for aperiodic and sporadic jobs.
* Schedule the frames till the hyperperiod with the help of psuedo-random numbers for periodic jobs.



### Description of the structure of the program
* The entire program is broken down into two parts-- periodic and non-periodic parts.
* The periodic part after taking in the task input, calculates the hyperperiod, the three frame-size conditions. It also creates all the task instances and then finds a frame for every job to run. This is also where the job slicing occurs if required (INF-like algorithm).
* The periodic part finally stores the frames data onto periodicSchedule.txt
* The non-periodic part takes in the aperiodic and sporadic inputs. It also takes in the input from periodicSchedule.txt.
* After checking the inputs, the non-periodic module calls the scheduler module.
* The scheduler module runs periodic, aperiodic and sporadic jobs. It also does the acceptance test for the sporadic jobs.


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
* printInfo.c - Prints the data in the ADTs and the scheduling information onto the output file.
* scheduler.c - The module associated with the scheduling of periodic + non-periodic jobs
#### Once the program is compiled
* executableName (variable in the Makefile) - The final executable.
* .o files for all .c files 
#### Once the program is run
* outputFile (variable in the Makefile) - The output file.
* periodicSchedule.txt - The file containing the information about every frame and the periodic tasks in them.
* periodicSchedule_humanReadable.txt - The file containing information about the frame data (static schedule) in a human readable format.



### Assumptions
1. None of the jobs-- periodic or non-periodic overrun.
2. Aperiodic and sporadic jobs have a fixed running time unlike periodic jobs.
3. Release time jitters are zero for all jobs.
4. Scheduling time is zero for all frames.


### Limitations
1. Since the float data shown on the screen has a granularity of 0.1, various times printed on the screen may make it seem that the job may have not executed (Example: Job running for 13.1 to 13.1 might be printed on the screen. But in reality the job may have run from 13.12 to 13.19). A lot of times the slack value is printed as 0, while in fact it may not truly be equal to 0.
2. This simulation does not consider the scenario where aperiodic and sporadic jobs also have a variable time of execution.
3. This simulation also does not consider aperiodic and sporadic tasks, we only consider them as single task instances.
4. Scheduling only till hyperperiod. This might not be the best way to simulate sporadic and aperiodic jobs as their deadlines might be after the hyperperiod.
5. This simulation does not take into account the scenario when there is a frame over-run by any of the jobs that execute.
6. Once aperiodic, periodic and sporadic jobs are done in a frame, this simulation looks into the aperiodic queue to see if there are any aperiodic jobs arriving this frame. If there is not aperiodic jobs arriving that frame, then it skips to the end of the frame. (Ex: Frame size = 4, current time = 15. If all jobs are done. Then it searches to see if there are any aperiodic jobs coming in before time = 16).
7. The period of a periodic task is accepted only as an integer. This is because finding hyperperiod for float values will result in very large hyperperiods. Another reason is that while loading these float values onto the system, the values may not be stored without losing some data.
8. The simulation does not take into consideration the amount of slack while scheduling aperiodic or extra sporadic jobs. It reality it does not make sense to schedule such jobs if the slack is not at least enough to finish a good amount of the jobs.
9. The release time jitters and the scheduling time for every frame is considered to be zero, which is not the case in practical systems.
10. A sporadic job may be rejected initially because there may not be enough time to run it. But periodic jobs not running for their 100% worst-case time might produce enough slack to be able to run it.
11. INF being a job-level splitting algorithm is at a disadvantage against a task-level splitting algorithm as for every instance of the task there might be splits that need to happen. But the advantage is that sometimes these splits need not be the same for every task instance in job-level splitting, whereas in task-level splitting, we are forced to split for the critical instance of the task (which may not be required for all other instances).
12. The INF algorithm may choose to split jobs of different tasks instead of sticking to splitting jobs of just of one task. That is an inherent disadvantage of the algorithm itself.
	Ex: If we have the following input in the periodicTasks.txt
		4
		4	1	4
		5	1.5	5
		10	3.5	10
		20	0.9	20
	We could get away with splitting jobs of T2 into 4. But the INF algorithm might choose to split T2 into 3 and T3 into 2.



### What could have been added
1. A sporadic job need not be rejected right away. If it is rejected in the arrival frame, the simulation could give the result of the acceptance test in the next frame (Because of the slack generated by periodic jobs).


### How to compile
On the CLI run "make" to compile and "make clean" to clean the compilation.
One can change the required flags and the compiler in the Makefile if required.
Please change the input files according to convenience and in the right format.


## Inputs required
1. periodicTasks.txt (CLI argument): First line should have the number of tasks in the task set. Subsequent lines should have 3 integers- period, wcet and relative deadline respectively.
2. APERIODIC_JOB_FILE: First line should have the number of tasks in the task set. Subsequent lines should have 2 integers- arrival time and execution time respectively.
3. SPORADIC_JOB_FILE: First line should have the number of tasks in the task set. Subsequent lines should have 3 integers- arrival time, execution time and absolute deadline respectively.


### Known bugs