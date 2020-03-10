CC=gcc

flags=-c -Wall

executableName=test

driver=driverMain

all: PeriodicTask

PeriodicTask: $(driver).o driverPeriodicTask.o printInfo.o checks.o frameConditions.o createTaskInstances.o calculateSchedule.o inputs.o split.o
	$(CC)  $(driver).o driverPeriodicTask.o printInfo.o checks.o frameConditions.o createTaskInstances.o calculateSchedule.o inputs.o split.o -o $(executableName)

$(driver).o: $(driver).c
	$(CC) $(flags) $(driver).c

driverPeriodicTask.o: driverPeriodicTask.c
	$(CC) $(flags) driverPeriodicTask.c

printInfo.o: printInfo.c
	$(CC) $(flags) printInfo.c

frameConditions.o: frameConditions.c
	$(CC) $(flags) frameConditions.c

createTaskInstances.o: createTaskInstances.c
	$(CC) $(flags) createTaskInstances.c

calculateSchedule.o: calculateSchedule.c
	$(CC) $(flags) calculateSchedule.c

checks.o: checks.c
	$(CC) $(flags) checks.c

inputs.o: inputs.c
	$(CC) $(flags) inputs.c

split.o: split.c
	$(CC) $(flags) split.c

clean:
	rm -f *.o $(executableName)
