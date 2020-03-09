CC=gcc

flags=-c -Wall

executableName=test

driver=test

all: Main

Main: $(driver).o printInfo.o frameConditions.o createTaskInstances.o calculateSchedule.o
	$(CC)  $(driver).o printInfo.o frameConditions.o createTaskInstances.o calculateSchedule.o -o $(executableName)

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

clean:
	rm -f *.o $(executableName)
