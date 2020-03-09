CC=gcc

flags=-c -Wall

executableName=test

driver=test

all: Main

Main: $(driver).o printInfo.o checks.o frameConditions.o createTaskInstances.o calculateSchedule.o inputs.o
	$(CC)  $(driver).o printInfo.o checks.o frameConditions.o createTaskInstances.o calculateSchedule.o inputs.o -o $(executableName)

$(driver).o: $(driver).c
	$(CC) $(flags) $(driver).c

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

clean:
	rm -f *.o $(executableName)
