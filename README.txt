# RTS-Assignment-2
Given a set of periodic, aperiodic and sporadic jobs in an RTS, finding a feasible schedule for the task set with valid frame sizes


# Tasks to be done
- [X] Find CPU utilisation and hyperperiod.
- [X] Create a Makefile.
- [X] Create basic schedule creator.
- [ ] Algorithm of selecting the jobs in a frame.
- [ ] While selecting a job that has been split, making sure that the there is an order that is followed.
- [ ] Find a way to slice multiple jobs more efficiently.
- [ ] Convert all data types into float.
- [ ] Adding the aperiodic and sporadic jobs part.
- [ ] Making the actual execution time as 20%-100% of wcet using pseudo random numbers.
- [ ] Modularise entire code base so that the driver function has at max 20 lines of code.

# Known bugs
- [X] Malloc and realloc throw errors in calculateSchedule.c