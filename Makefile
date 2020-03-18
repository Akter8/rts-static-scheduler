CC=gcc

flags=-c

executableName=test

driver = driverMain

outputFile = outputFile.txt


all: $(driver).o driverPeriodicTask.o printInfo.o checks.o findFrame.o frameConditions.o createTaskInstances.o inputs.o driverNonPeriodicJob.o sort.o scheduler.o utilityFunctions.o
	$(CC)  $(driver).o driverPeriodicTask.o printInfo.o checks.o findFrame.o frameConditions.o createTaskInstances.o inputs.o driverNonPeriodicJob.o sort.o scheduler.o utilityFunctions.o -o $(executableName)

$(driver).o: $(driver).c
	$(CC) $(flags) $(driver).c

driverPeriodicTask.o: driverPeriodicTask.c
	$(CC) $(flags) driverPeriodicTask.c

driverNonPeriodicJob.o: driverNonPeriodicJob.c
	$(CC) $(flags) driverNonPeriodicJob.c

scheduler.o: scheduler.c
	$(CC) $(flags) scheduler.c

sort.o: sort.c
	$(CC) $(flags) sort.c

printInfo.o: printInfo.c
	$(CC) $(flags) printInfo.c

findFrame.o: findFrame.c
	$(CC) $(flags) findFrame.c

frameConditions.o: frameConditions.c
	$(CC) $(flags) frameConditions.c

createTaskInstances.o: createTaskInstances.c
	$(CC) $(flags) createTaskInstances.c

checks.o: checks.c
	$(CC) $(flags) checks.c

inputs.o: inputs.c
	$(CC) $(flags) inputs.c

utilityFunctions.o: utilityFunctions.c
	$(CC) $(flags) utilityFunctions.c

clean:
	rm -f *.o $(executableName) $(outputFile)
