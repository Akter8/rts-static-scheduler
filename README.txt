# RTS-Assignment-2
Name: 	Akhil Tarikere
ID:		2017A7PS1916G
Given a set of periodic, aperiodic and sporadic jobs in an RTS, finding a feasible schedule for the task set with valid frame sizes.


### Question
For a given in-phase input task set consisting of periodic tasks [Read the input task set from a file whose name is a command line argument], find
1. Hyper-period and the best frame size possible [if required, split the task(s)] and provide a periodic schedule per every frame. [Get frame table with the number of entries equal to the number of frames]. Store the table in a file named periodicSchedule.txt.
2. Read aperiodic jobs [from a file], Sporadic jobs [from another file] and periodic schedule [from periodicSchedule.txt] and execute them using:
	1. Aperiodic jobs with minimum average response time [using Slack stealing approach]
	2. Sporadic jobs with cyclic EDF sporadic scheduler
3. Enhance your implementation by relaxing the assumption on WCET. Assume that the actual execution time of a job can be lesser than or equal to the WCET of it (20% to 100% determined using a pseudo random function).


### Tasks to be done
- [ ] Algorithm of selecting the jobs in a frame (INF).
- [ ] While selecting a job that has been split, making sure that the there is an order that is followed.
- [ ] Find a way to slice multiple jobs more efficiently.
- [ ] Make splitPeriodicTask in split.c return the frame size.
- [ ] Combine the task of slicing and selecting jobs for a frame.
- [ ] Convert all data types into float.
- [ ] Remove the printfs in the scheduler.
- [X] Check if every time I write into stderr, if I am exiting or not.
- [ ] Modify what is to be done when slack is present, but aperiodic, sporadic and periodic jobs are not present.
- [ ] Finish the check-2 while accepting a new job in findSporadicJobsInFrame() in scheduler.c
- [ ] Finish the sorting of sporadic jobs based on EDF in scheduler.c
- [ ] Optimise the code in scheduler.c
- [ ] Check if there are any warnings while compiling.


### Tasks Completed
* Define the relevant ADTs.
* Input and sort the data into the various ADTs.
* Find CPU utilisation and Hyperperiod.
* Generate the various jobs for periodic tasks.
* Slice jobs if required.
* Fit the jobs into appropriate frames.
* Store the schedule onto periodicSchedule.txt
* Input and sort the data based on arrival time for aperiodic and sporadic jobs.
* Schedule the frames till the hyperperiod with the help of psuedo-random numbers for periodic jobs.

### Assumptions
1. None of the jobs-- periodic or non-periodic overrun.
2. Aperiodic and sporadic jobs have a fixed running time unlike periodic jobs.


### Limitations
1. Since the float data shown on the screen has a granularity of 0.1, various times printed on the screen may make it seem that the job may have not executed (Example: Job running for 13.1 to 13.1 might be printed on the screen. But in reality the job may have run from 13.12 to 13.19).
2. Since the periodic jobs might execute right after one another, the same percentage (20 - 100%) of execution of all the periodic jobs may occur.
3. This simulation does not consider the scenario when aperiodic and sporadic jobs also have a variable time of execution.
4. This simulation also does not consider aperiodic and sporadic tasks, we only consider them as single task instances.
5. Scheduling only till hyperperiod. This might not be the best way to simulate sporadic and aperiodic jobs as their deadlines might be after the hyperperiod.
6. This simulation does not take into account the scenario when there is a frame over-run by any of the jobs that execute.




### Known bugs



### Description of files
* 
*
*
*


### How to compile
On the CLI run "make" to compile and "make clean" to clean the compilation.



## Inputs required
1. periodicTasks.txt: First line should have the number of tasks in the task set. Subsequent lines should have 3 integers- period, wcet and relative deadline respectively.
2. aperiodicJobs.txt: First line should have the number of tasks in the task set. Subsequent lines should have 2 integers- arrival time and execution time respectively.
3. sporadicJobs.txt: First line should have the number of tasks in the task set. Subsequent lines should have 3 integers- arrival time, execution time and absolute deadline respectively.