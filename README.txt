# RTS-Assignment-2
Given a set of periodic, aperiodic and sporadic jobs in an RTS, finding a feasible schedule for the task set with valid frame sizes


# Tasks to be done
- [X] Find CPU utilisation and hyperperiod.
- [X] Create a Makefile.
- [X] Create basic schedule creator.
- [X] Sort the input based on arrival time.
- [ ] Algorithm of selecting the jobs in a frame (INF).
- [ ] While selecting a job that has been split, making sure that the there is an order that is followed.
- [ ] Find a way to slice multiple jobs more efficiently.
- [ ] Make splitPeriodicTask in split.c return the frame size.
- [ ] Combine the task of slicing and selecting jobs for a frame.
- [ ] Convert all data types into float.
- [X] Taking input from aperiodic, sporadic and frames files.
- [ ] Adding the aperiodic and sporadic jobs part.
- [ ] Making the actual execution time as 20%-100% of wcet using pseudo random numbers.
- [X] Modularise entire code base so that the driver function has at max 20 lines of code.
- [X] Store the frame data in outputfile - periodicSchedule.txt
- [X] Convert numLines into numTasks.

# Assumptions
1. None of the jobs- periodic or non-periodic overrun.

# Known bugs
- [X] Malloc and realloc throw errors in calculateSchedule.c

# How to compile
On the CLI run "make" to compile and "make clean" to clean the compilation.

# Inputs required
1. periodicTasks.txt: First line should have the number of tasks in the task set. Subsequent lines should have 3 integers- period, wcet and relative deadline respectively.
2. aperiodicJobs.txt: First line should have the number of tasks in the task set. Subsequent lines should have 2 integers- arrival time and execution time respectively.
3. sporadicJobs.txt: First line should have the number of tasks in the task set. Subsequent lines should have 3 integers- arrival time, execution time and absolute deadline respectively.