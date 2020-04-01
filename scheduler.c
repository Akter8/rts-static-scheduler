/*
 * Author: Akhil Tarikere
 * Date: 10/3/20
 * 
 * Pre-Condition: Takes inputs from 3 files- periodic (written by the PeriodicJobDriver()), aperiodic and sporadic.
 * 		
 *
 * Post-Condition: Does the actual scheduling of the jobs.
 *	
 */


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "functionNonPeriodic.h"
#include "configuration.h"

extern int numPreemptions;


/*
 * Checks whether if there is an aperiodic job alive to be able to run
 * at time units currentTime.
 */
bool
checkAperiodicJob(AperiodicJob *aperiodicJobs, int jobIndex, float currentTime)
{
	// jobIndex = -1 when all jobs are done.
	if (jobIndex == -1)
		return false;

	// If no jobs are available at current time.
	if (aperiodicJobs[jobIndex].alive && aperiodicJobs[jobIndex].arrivalTime <= currentTime)
		return true;

	return false;
}



/*
 * Runs aperiodic jobs till either the slack gets over, or till the aperiodic
 * job queue is empty.
 * Returns the amount of time aperiodic jobs ran in this call.
 */
float
runAperiodic(AperiodicJob *aperiodicJobs, int numJobs, int *jobIndex, float maxExecutionTime, float currentTime, int currentFrame, int frameSize)
{
	// Check if all aperiodic jobs are done.
	if (*jobIndex == -1)
		return 0;

	// Check if no jobs are currently available.
	if (currentTime + (currentFrame * frameSize) < aperiodicJobs[*jobIndex].arrivalTime)
	{
		return 0;
	}

	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "Running Aperiodic Jobs from %0.1f to %0.1f\n", currentTime + (currentFrame *frameSize), maxExecutionTime + currentTime + (currentFrame *frameSize));

	float timeExecuted = 0;
	while (timeExecuted < maxExecutionTime) // Execute only when we have some slack.
	{
		fprintf(outputFile, "Aperiodic Job: A%d timeLeft=%0.1f before running.\n", aperiodicJobs[*jobIndex].jobNum, aperiodicJobs[*jobIndex].timeLeft);
		float timeLeft = maxExecutionTime - timeExecuted; // Max time left to run these.
		float minTime = (aperiodicJobs[*jobIndex].timeLeft < timeLeft) ? aperiodicJobs[*jobIndex].timeLeft : timeLeft; // We can only execute this job either till it finishes, or till our slack is over. whichever is min.

		timeExecuted += minTime;

		aperiodicJobs[*jobIndex].timeLeft -= minTime;

		// Increment number of preemptions.
		if (aperiodicJobs[*jobIndex].timeLeft > timeLeft)
		{
			numPreemptions++;
			fprintf(outputFile, "Aperiodic Job: A%d being preempted. TimeLeft=%0.1f\n", aperiodicJobs[*jobIndex].jobNum, aperiodicJobs[*jobIndex].timeLeft);
		}

		if (aperiodicJobs[*jobIndex].timeLeft == 0)	// If the current job is done.
		{
			fprintf(outputFile, "Aperiodic Job: A%d has finished\n", aperiodicJobs[*jobIndex].jobNum);

			// Changing the job's parameters.
			aperiodicJobs[*jobIndex].alive = false;

			// Respose time = time at which it finished - arrival time
			// = start of aperiodic execution time + time aperiodic jobs have run for - arrival time.
			aperiodicJobs[*jobIndex].responseTime = currentTime + (currentFrame * frameSize) + timeExecuted - aperiodicJobs[*jobIndex].arrivalTime;
			(*jobIndex)++;
			if (*jobIndex >= numJobs)
				*jobIndex = -1;
		}

		if (*jobIndex == -1)
			break;
	}
	fclose(outputFile);

	return timeExecuted;
}



/*
 * Runs accepted sporadic jobs till either the slack gets over, or till the
 * accepted job queue is empty.
 * Returns the amount of time sporadic jobs ran for during this call.
 */
float
runSporadic(SporadicJob *sporadicJobs, int numJobs, int *jobIndex, float maxExecutionTime, float currentTime, int currentFrame, int frameSize)
{
	// Checking if all sporadic jobs are already done.
	if (*jobIndex == -1)
		return 0;

	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "Running sporadicJobs for the next %0.1f units if possible.\n", maxExecutionTime);

	float timeExecuted = 0;

	while (timeExecuted < maxExecutionTime) // Execute only when we have some slack.
	{
		fprintf(outputFile, "Sporadic Job: S%d timeLeft=%0.1f before running.\n", sporadicJobs[*jobIndex].jobNum, sporadicJobs[*jobIndex].timeLeft);
		float timeLeft = maxExecutionTime - timeExecuted; // Finding the time left.
		float minTime = (sporadicJobs[*jobIndex].timeLeft < timeLeft) ? sporadicJobs[*jobIndex].timeLeft : timeLeft; // We can only execute this job either till it finishes, or till our slack is over. whichever is min.
		// fprintf(outputFile, "minTime = %0.1f\n", minTime);

		timeExecuted += minTime;

		sporadicJobs[*jobIndex].timeLeft -= minTime;


		// Increment number of preemptions.
		if (sporadicJobs[*jobIndex].timeLeft > timeLeft)
		{
			numPreemptions++;
			fprintf(outputFile, "Sporadic Job: S%d being preempted. TimeLeft=%0.1f\n", sporadicJobs[*jobIndex].jobNum, sporadicJobs[*jobIndex].timeLeft);
		}


		if (sporadicJobs[*jobIndex].timeLeft == 0)	// When the current job is finished.
		{
			fprintf(outputFile, "Sporadic Job: S%d has finished\n", sporadicJobs[*jobIndex].jobNum);

			// Respose time = time at which it finished - arrival time
			// = start of aperiodic execution time + time aperiodic jobs have run for - arrival time.
			sporadicJobs[*jobIndex].responseTime = currentTime + (currentFrame * frameSize) + timeExecuted - sporadicJobs[*jobIndex].arrivalTime;

			sporadicJobs[*jobIndex].alive = false;
			(*jobIndex)++;
			if (*jobIndex >= numJobs)
				*jobIndex = -1;
		}

		if (*jobIndex == -1)
			break;
	}
	fclose(outputFile);

	return timeExecuted;
}



/*
 * Runs a single periodic job between 20% to 100% of its worst-case execution time.
 * Returns the amount of time the job ran for.
 */
float
runPeriodic(PeriodicJob periodicJob)
{
	// To get a number between 0.2 and 1.0, we can generate a number between 0 and 0.8, and then add 0.2 to it.
	float fraction = rand() % 80;
	fraction = (float)(fraction + 20) / (float) 100;

	float executionTime = fraction * periodicJob.wcet;

	return executionTime;
}



/*
 * Finds what all sporadic jobs have to and can execute in a particular frame.
 * Returns a list of sporadic jobs that have to run in that particular frame.
 */
SporadicJob
*findSporadicJobsInFrame(SporadicJob *sporadicJobs, int numJobs, int *numJobsThisFrame, int frameNum, float slack, float *reduceSlackBy, ScheduleFrame *framesData, int numFrames)
{
	/* 
	Algorithm:
	Find sporadic jobs that were accepted in previous frames but are yet to finish.
	Do the acceptance test for other sporadic jobs arriving this frame.
	Acceptance test:
		Check-1:
			If there is enough slack for this job in its valid frames.
			Calculate this by adding up the slack for every frame.
		Check-2:
			If there is enough slack for this job along with sporadic jobs that were accepted previously.
			Calculate this by adding the slack of that frame, but subtracting the execution time of a previously accepted sporadic job if the latter's deadline is before the former's (this will work because we are following EDF while scheduling sporadic jobs).
		If a new sporadic job passes both the test, accept it.
		Else reject it.
	End 
	*/

	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "Finding sporadicJobs in this frame.\n");
	int jobsThisFrame = 0;

	SporadicJob *sporadicJobsThisFrame = (SporadicJob *) malloc(sizeof(SporadicJob) * numJobs);

	int index = 0;

	// Adding jobs that were accepted in the previous frames and still have to finished.
	float acceptedTime = 0;
	for (int i = 0; i < numJobs; ++i)
	{
		if (sporadicJobs[i].alive && sporadicJobs[i].accepted && !sporadicJobs[i].rejected && sporadicJobs[i].startFrame <= frameNum && sporadicJobs[i].maxFrame > frameNum)
		{	
			fprintf(outputFile, "Adding sporadic job: S%d\n", sporadicJobs[i].jobNum);
			sporadicJobsThisFrame[index++] = sporadicJobs[i];
			*reduceSlackBy += ((sporadicJobs[i].timeLeft < slack - *reduceSlackBy) ? sporadicJobs[i].timeLeft : slack -*reduceSlackBy);
			jobsThisFrame++;
			acceptedTime += sporadicJobs[i].timeLeft;
		}
	}


	// Acceptance test for new jobs arriving this frame.
	for (int i = 0; i < numJobs; ++i)
	{
		// fprintf(outputFile, "Checking sporadic job %d\n", i);
		// fprintf(outputFile, "alive=%d, accepted=%d, rejected=%d, startFrame=%d, maxFrame=%d\n", sporadicJobs[i].alive, sporadicJobs[i].accepted, sporadicJobs[i].rejected, sporadicJobs[i].startFrame, sporadicJobs[i].maxFrame);
		if (sporadicJobs[i].alive && !sporadicJobs[i].accepted && !sporadicJobs[i].rejected && sporadicJobs[i].startFrame <= frameNum && sporadicJobs[i].maxFrame > frameNum)
		{
			// fprintf(outputFile, "Sporadic job %d doing check-1\n", sporadicJobs[i].jobNum);

			// Condition - 1
			//Scheduler determines the current total slack in the frames before the deadline of Job S is at least equal to the execution time e of S.
			float countSlack = 0;
			for (int f = frameNum; f < numFrames; ++f)
			{
				countSlack += framesData[f].slack;
			}

			if (countSlack < sporadicJobs[i].wcet)	// Rejecting after condition-1.
			{
				fprintf(outputFile, "Rejected Sporadic job %d\n", sporadicJobs[i].jobNum);
				sporadicJobs[i].rejected = true;
				sporadicJobs[i].accepted = false;
				continue;
			}

			// fprintf(outputFile, "Sporadic Job %d passed check-1\n", sporadicJobs[i].jobNum);

			// Condition - 2
			// Scheduler determines whether any sporadic job in the system will complete late if it accepts S. If acceptance of S will not cause any sporadic job in the system to complete too late.
			// Total slack should be reduced by the execution time of already accepted jobs that have a deadline before the job being considered.
			acceptedTime = 0;
			for (int j = 0; j < numJobs; ++j)
			{
				if (sporadicJobs[j].alive && sporadicJobs[j].accepted && !sporadicJobs[j].rejected)
				{
					if (i != j && sporadicJobs[j].maxFrame >= sporadicJobs[i].maxFrame)
						acceptedTime += sporadicJobs[i].wcet;
				}
			}

			if (countSlack - acceptedTime < sporadicJobs[i].wcet) // If rejected after condition-2.
			{
				fprintf(outputFile, "Rejected Sporadic job %d\n", sporadicJobs[i].jobNum);
				sporadicJobs[i].rejected = true;
				sporadicJobs[i].accepted = false;
				continue;
			}
			else // Else if accepted after both condition checks.
			{
				fprintf(outputFile, "Accepted Sporadic job: S%d\n", sporadicJobs[i].jobNum);
				sporadicJobs[i].accepted = true;
				sporadicJobs[i].rejected = false;
				*reduceSlackBy += ((sporadicJobs[i].timeLeft < slack - *reduceSlackBy) ? sporadicJobs[i].timeLeft : slack -*reduceSlackBy);
				sporadicJobsThisFrame[index++] = sporadicJobs[i];
				jobsThisFrame++;

			}
		}
			
	}

	if (jobsThisFrame == 0)
		*reduceSlackBy = 0;

	fprintf(outputFile, "Sporadic jobsThisFrame: %d\n", jobsThisFrame);
	fprintf(outputFile, "Reducing slack by: %0.1f\n", *reduceSlackBy);
	*numJobsThisFrame = jobsThisFrame;
	sporadicJobsThisFrame = realloc(sporadicJobsThisFrame, sizeof(SporadicJob) * jobsThisFrame);
	fclose(outputFile);


	return sporadicJobsThisFrame;
}



/*
 * The actual scheduler that goes through every frame and schedules jobs based on the slack available.
 */
void
scheduler(ScheduleFrame *framesData, int numFrames, int frameSize, AperiodicJob *aperiodicJobs, int numAperiodicJobs, SporadicJob *sporadicJobs, int numSporadicJobs)
{
	/* Algorithm:
	In a frame f:
		accept sporadic jobs if required.
		sort the sporadic jobs based on EDF.
		update the slack of the frame.
		while 1:
			if slack > 0:
				if sporadic jobs exists:
					run sporadic jobs
					update slack
					continue
				else if aperiodic jobs exists:
					run aperiodic jobs
					update slack
					continue
				else if periodic job exists:
					run periodic job
					update slack
					continue
				else:
					if aperiodic job arrives before frame gets over:
						sleep till then and then run aperiodic job.
					else:
						sleep till the end of the frame.
			else:
				if periodic job exists:
					run periodic job
					update slack
					continue
				else if sporadic job exists:
					run sporadic job
					continue
				else:
					set slack = time left in the frame.
	end
	*/

	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "----------------------------------------------\n");
	fprintf(outputFile, "Scheduler running now.\n");
	fprintf(outputFile, "Frame size: %d, No of frames: %d\n", frameSize, numFrames);
	fprintf(outputFile, "Disclaimer: The slack may be shown as 0 (because of the granularity, but it might in fact be greater than 0, and hence do something which would only be done when slack is not 0.\n");

	float currentTime = 0; // Refers to the current time wrt to the current frame. Can range between 0 and frameSize.
	
	int acceptedSporadicIndex;
	int periodicJobIndex;
	float runTime; // To find how for how much time a job ran.

	int aperiodicIndex = -1; // Initially set.
	if (numAperiodicJobs > 0)
		aperiodicIndex = 0;

	// Initialising the seed before the jobs start executing.
	srand(time(NULL));

	for (int f = 0; f < numFrames; ++f)
	{
		fprintf(outputFile, "\n\nFrame no: %d\n", f);
		fprintf(outputFile, "Slack without sporadicJobs: %0.1f\n", framesData[f].slack);

		acceptedSporadicIndex = -1; // Set if no sporadic jobs are to run in this frame. Will be updated.
		periodicJobIndex = 0; // At the start of the frame, the 0th job runs first.
		currentTime = 0; // At the start of every frame, the relative current time = 0. 

		// Add to the list of accepted sporadic jobs if required.
		int numSporadicJobsAtPresent;
		float reduceSlackBy = 0;
		fflush(outputFile);

		// Finding the set of sporadic jobs set to run this frame.
		SporadicJob *acceptedSporadicJobs = findSporadicJobsInFrame(sporadicJobs, numSporadicJobs, &numSporadicJobsAtPresent, f, framesData[f].slack, &reduceSlackBy, framesData, numFrames);

		// Setting the parameters in the main copy of the sporadic jobs.
		for (int i = 0; i < numSporadicJobs; ++i)
		{
			if (sporadicJobs[i].alive && sporadicJobs[i].startFrame <= f && sporadicJobs[i].maxFrame > f)
			{
				bool flag = true;
				for (int j = 0; j < numSporadicJobsAtPresent; ++j)
				{
					if (sporadicJobs[i].jobNum == acceptedSporadicJobs[j].jobNum)
					{
						flag = false;
						break;
					}
				}

				if (flag) // Not been accepted.
				{
					sporadicJobs[i].accepted = false;
					sporadicJobs[i].rejected = true;
				}
				else // Accepted.
				{
					sporadicJobs[i].accepted = true;
					sporadicJobs[i].rejected = false;
				}
			}
		}

		if (numSporadicJobsAtPresent != 0)
			acceptedSporadicIndex = 0;



		// Reduce the available slack after accepting sporadic jobs.
		framesData[f].slack -= reduceSlackBy;


		fprintf(outputFile, "Slack with sporadicJobs: %0.1f\n", framesData[f].slack);
		fflush(outputFile);


		// Sort the accepted sporadic jobs based on EDF.
		sortSporadicJobsOnEDF(acceptedSporadicJobs, 0, numSporadicJobsAtPresent - 1);

		while (1) // Runs all the jobs for this frame.
		{
			if (currentTime >= frameSize)
				break;

			if (framesData[f].slack > 0.0f) // If slack is available.
			{
				fprintf(outputFile, "Slack = %0.1f. Slack > 0\n", framesData[f].slack);
				fflush(outputFile);
				// If sporadic job exists, execute it.
				if (acceptedSporadicIndex != -1)
				{
					fprintf(outputFile, "\nSporadic jobs running. First Job = S%d.\n", acceptedSporadicJobs[acceptedSporadicIndex].jobNum);
					fflush(outputFile);

					runTime = runSporadic(acceptedSporadicJobs, numSporadicJobsAtPresent, &acceptedSporadicIndex, framesData[f].slack, currentTime, f, frameSize);

					fprintf(outputFile, "Sporadic jobs ran from %0.1f to %0.1f\n", currentTime + (f * frameSize), currentTime + (f * frameSize) + runTime);

					fflush(outputFile);

					// Updating slack
					if (runTime <= framesData[f].slack)
						framesData[f].slack -= runTime;

					currentTime += runTime;
					continue;
				}

				// Else if aperiodic job exists, execute it.
				fprintf(outputFile, "Sporadic jobs not available.\n");
				fflush(outputFile);

				// Check if aperiodic job is available.
				bool aperiodicCheck = checkAperiodicJob(aperiodicJobs, aperiodicIndex, currentTime + (f * frameSize));
				if (aperiodicCheck)
				{
					fprintf(outputFile, "Aperiodic jobs executing.\n");
					fflush(outputFile);

					runTime = runAperiodic(aperiodicJobs, numAperiodicJobs, &aperiodicIndex, framesData[f].slack, currentTime, f, frameSize);
					framesData[f].slack -= runTime;
					currentTime += runTime;

					if (runTime != 0)
						continue;
				}
				else
				{
					fprintf(outputFile, "Aperiodic jobs currently not available.\n");
					fflush(outputFile);
				}

				// Else if periodic job exists, execute it.
				// fprintf(outputFile, "not aperiodic\n");
				if (periodicJobIndex < framesData[f].numPeriodicJobs)
				{
					runTime = runPeriodic(framesData[f].periodicJobs[periodicJobIndex]);

					fprintf(outputFile, "\nSlack before periodic job: %0.1f\n", framesData[f].slack);
					fprintf(outputFile, "Periodic Job: J%d with instance: %d, and wcet=%0.1f running from %0.1f to %0.1f\n", framesData[f].periodicJobs[periodicJobIndex].taskNum, framesData[f].periodicJobs[periodicJobIndex].instanceNum, framesData[f].periodicJobs[periodicJobIndex].wcet, currentTime + (f * frameSize), currentTime + (f * frameSize) + runTime);

					// Update the DS about the execution.
					framesData[f].periodicJobs[periodicJobIndex].finishTime = currentTime + (f * frameSize) + runTime;
					framesData[f].periodicJobs[periodicJobIndex].executionTime = runTime;
					framesData[f].periodicJobs[periodicJobIndex].alive = false;
					framesData[f].slack += (framesData[f].periodicJobs[periodicJobIndex].wcet - runTime);

					fprintf(outputFile, "Slack after periodic job: %0.1f\n\n", framesData[f].slack);
					fflush(outputFile);

					periodicJobIndex++;
					currentTime += runTime;

					continue;
				}

				fprintf(outputFile, "Periodic jobs not available.\n");
				fflush(outputFile);

				//Check if aperiodic job is coming before this frame ends.
				float nextAperiodicJobArrivalTime;
				if (aperiodicIndex != -1)
					nextAperiodicJobArrivalTime = aperiodicJobs[aperiodicIndex].arrivalTime;
				else
				{
					fprintf(outputFile, "Idle till the end of the frame.\n");
					fflush(outputFile);

					currentTime = frameSize;
					continue;
				}
				// If yes, sleep till then and then run aperiodic job.
				if (nextAperiodicJobArrivalTime - (f * frameSize) < frameSize)
				{
					fprintf(outputFile, "Idle from %0.1f to %0.1f\n", currentTime, nextAperiodicJobArrivalTime - (f * frameSize));
					currentTime = nextAperiodicJobArrivalTime - (f * frameSize);
					fflush(outputFile);
					continue;
				}
				// If no, sleep till the end of the frame.
				else
				{
					fprintf(outputFile, "Idle till the end of the frame.\n");
					currentTime = frameSize;
					fflush(outputFile);
					continue;
				}				

			}
			else 	// If there is no slack available.
			{	
				// If periodic jobs exist, execute them.
				if (periodicJobIndex < framesData[f].numPeriodicJobs)
				{
					runTime = runPeriodic(framesData[f].periodicJobs[periodicJobIndex]);
					framesData[f].periodicJobs[periodicJobIndex].alive = false;
					fprintf(outputFile, "\nSlack before periodic job: %0.1f\n", framesData[f].slack);

					// Updating slack.
					framesData[f].slack += (framesData[f].periodicJobs[periodicJobIndex].wcet - runTime);

					fprintf(outputFile, "Periodic Job: J%d with instance: %d, and wcet=%0.1f running from %0.1f to %0.1f\n", framesData[f].periodicJobs[periodicJobIndex].taskNum, framesData[f].periodicJobs[periodicJobIndex].instanceNum, framesData[f].periodicJobs[periodicJobIndex].wcet, currentTime + (f * frameSize), currentTime + (f * frameSize) + runTime);

					fprintf(outputFile, "Slack after periodic job: %0.1f\n\n", framesData[f].slack);
					fflush(outputFile);

					framesData[f].periodicJobs[periodicJobIndex].finishTime = currentTime + (f * frameSize) + runTime;
					framesData[f].periodicJobs[periodicJobIndex].executionTime = runTime;

					currentTime += runTime;
					periodicJobIndex++;

					continue;
				}

				// Else if sporadic jobs exist, execute them.
				else if (acceptedSporadicIndex != -1)
				{
					fprintf(outputFile, "\nSporadic jobs running. First Job = S%d.\n", acceptedSporadicJobs[acceptedSporadicIndex].jobNum);
					fflush(outputFile);

					runTime = runSporadic(acceptedSporadicJobs, numSporadicJobsAtPresent, &acceptedSporadicIndex, frameSize - currentTime, currentTime, f, frameSize);
					// Not updating slack as this was a scheduled execution.

					fprintf(outputFile, "Sporadic jobs ran from %0.1f to %0.1f\n", currentTime + (f * frameSize), currentTime + (f * frameSize) + runTime);

					fflush(outputFile);

					currentTime += runTime;
					
					continue;
				}
				else
				{
					// Else we update the slack and continue.
					framesData[f].slack = frameSize - currentTime;
					fprintf(outputFile, "Increasing slack by %0.1f as no scheduled periodic or sporadic jobs are present.\n", frameSize - currentTime);
					fflush(outputFile);
				}
			}
		}

		// Update sporadic jobs that finished in the main copy.
		for (int i = 0; i < numSporadicJobsAtPresent; ++i)
		{
			if (acceptedSporadicJobs[i].alive == false)
			{
				for (int j = 0; j < numSporadicJobs; ++j)
				{
					if (acceptedSporadicJobs[i].jobNum == sporadicJobs[j].jobNum)
					{
						sporadicJobs[j].alive = false;
						sporadicJobs[j].responseTime = acceptedSporadicJobs[i].responseTime;
						break;
					}
				}
			}
			else
			{
				for (int j = 0; j < numSporadicJobs; ++j)
				{
					if (acceptedSporadicJobs[i].jobNum == sporadicJobs[j].jobNum)
					{
						sporadicJobs[j].timeLeft = acceptedSporadicJobs[i].timeLeft;
						break;
					}
				}
			}
		}


		// Freeing memory at the end of every frame.
		free(acceptedSporadicJobs);
	}

	// After all frames are done being scheduled.
	fprintf(outputFile, "\n\nFinished Scheduling.\n----------------------------------------------\n");
	fclose(outputFile);

	return;
}