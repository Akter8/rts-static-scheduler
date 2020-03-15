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


bool
checkAperiodicJob(AperiodicJob *aperiodicJobs, int jobIndex, float currentTime)
{
	if (jobIndex == -1)
		return false;

	if (aperiodicJobs[jobIndex].alive && aperiodicJobs[jobIndex].arrivalTime <= currentTime)
		return true;

	return false;
}



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
	while (timeExecuted < maxExecutionTime)
	{
		float timeLeft = maxExecutionTime - timeExecuted;
		float minTime = (aperiodicJobs[*jobIndex].timeLeft < timeLeft) ? aperiodicJobs[*jobIndex].timeLeft : timeLeft; // We can only execute this job either till it finishes, or till our slack is over. whichever is min.

		timeExecuted += minTime;

		aperiodicJobs[*jobIndex].timeLeft -= minTime;

		if (aperiodicJobs[*jobIndex].timeLeft == 0)
		{
			fprintf(outputFile, "Aperiodic Job: A%d has finished\n", aperiodicJobs[*jobIndex].jobNum);
			aperiodicJobs[*jobIndex].alive = false;
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


float
runSporadic(SporadicJob *sporadicJobs, int numJobs, int *jobIndex, float maxExecutionTime)
{
	// Checking if all sporadic jobs are already done.
	if (*jobIndex == -1)
		return 0;

	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, " Running sporadicJobs for the next %0.1f units if possible.\n", maxExecutionTime);

	float timeExecuted = 0;

	while (timeExecuted < maxExecutionTime) // Execute only when we have some slack.
	{
		fprintf(outputFile, "Sporadic Job: S%d timeLeft=%0.1f\n", sporadicJobs[*jobIndex].jobNum, sporadicJobs[*jobIndex].timeLeft);
		float timeLeft = maxExecutionTime - timeExecuted; // Finding the time left.
		float minTime = (sporadicJobs[*jobIndex].timeLeft < timeLeft) ? sporadicJobs[*jobIndex].timeLeft : timeLeft; // We can only execute this job either till it finishes, or till our slack is over. whichever is min.
		fprintf(outputFile, "minTime = %0.1f\n", minTime);

		timeExecuted += minTime;

		sporadicJobs[*jobIndex].timeLeft -= minTime;

		if (sporadicJobs[*jobIndex].timeLeft == 0)
		{
			fprintf(outputFile, "Sporadic Job: S%d has finished\n", sporadicJobs[*jobIndex].jobNum);

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



float
runPeriodic(PeriodicJob periodicJob)
{
	srand(time(NULL));

	// To get a number between 0.2 and 1.0, we can generate a number between 0 and 0.8, and then add 0.2 to it.
	float fraction = rand() % 80;
	fraction = (float)(fraction + 20) / (float) 100;

	float executionTime = fraction * periodicJob.wcet;

	return executionTime;
}




SporadicJob
*findSporadicJobsInFrame(SporadicJob *sporadicJobs, int numJobs, int *numJobsThisFrame, int frameNum, float slack, float *reduceSlackBy, ScheduleFrame *framesData, int numFrames)
{
	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "Finding sporadicJobs in this frame.\n");
	int jobsThisFrame = 0;

	SporadicJob *sporadicJobsThisFrame = (SporadicJob *) malloc(sizeof(SporadicJob) * numJobs);

	int index = 0;

	// Adding already accepted jobs.
	float acceptedTime = 0;
	for (int i = 0; i < numJobs; ++i)
	{
		if (sporadicJobs[i].alive && sporadicJobs[i].accepted && !sporadicJobs[i].rejected && sporadicJobs[i].startFrame <= frameNum && sporadicJobs[i].maxFrame > frameNum)
		{	
			fprintf(outputFile, "Accepted Sporadic job %d\n", i);
			sporadicJobsThisFrame[index++] = sporadicJobs[i];
			*reduceSlackBy += ((sporadicJobs[i].timeLeft < slack - *reduceSlackBy) ? sporadicJobs[i].timeLeft : slack -*reduceSlackBy);
			jobsThisFrame++;
			acceptedTime += sporadicJobs[i].timeLeft;
		}
	}


	// Accepting new jobs.
	for (int i = 0; i < numJobs; ++i)
	{
		// fprintf(outputFile, "Checking sporadic job %d\n", i);
		// fprintf(outputFile, "alive=%d, accepted=%d, rejected=%d, startFrame=%d, maxFrame=%d\n", sporadicJobs[i].alive, sporadicJobs[i].accepted, sporadicJobs[i].rejected, sporadicJobs[i].startFrame, sporadicJobs[i].maxFrame);
		if (sporadicJobs[i].alive && !sporadicJobs[i].accepted && !sporadicJobs[i].rejected && sporadicJobs[i].startFrame <= frameNum && sporadicJobs[i].maxFrame > frameNum)
		{
			// fprintf(outputFile, "Sporadic job %d doing check-1\n", i);
			// Check - 1
			//Scheduler determines the current total slack in the frames before the deadline of Job S is at least equal to the execution time e of S.
			float countSlack = 0;
			for (int f = frameNum; f < numFrames; ++f)
			{
				countSlack += framesData[f].slack;
			}

			if (countSlack < sporadicJobs[i].wcet)
			{
				// fprintf(outputFile, "Rejected Sporadic job %d\n", i);
				sporadicJobs[i].rejected = true;
				sporadicJobs[i].accepted = false;
				continue;
			}

			// fprintf(outputFile, "Sporadic Job %d passed check-1\n", i);

			// Check - 2
			// Scheduler determines whether any sporadic job in the system will complete late if it accepts S. If acceptance of S will not cause any sporadic job in the system to complete too late.
			acceptedTime = 0;
			for (int j = 0; j < numJobs; ++j)
			{
				if (sporadicJobs[j].alive && sporadicJobs[j].accepted && !sporadicJobs[j].rejected)
				{
					if (i != j && sporadicJobs[j].maxFrame >= sporadicJobs[i].maxFrame)
						acceptedTime += sporadicJobs[i].wcet;
				}
			}

			if (countSlack - acceptedTime < sporadicJobs[i].wcet) // If rejected
			{
				fprintf(outputFile, "Rejected Sporadic job %d\n", i);
				sporadicJobs[i].rejected = true;
				sporadicJobs[i].accepted = false;
				continue;
			}
			else // Else if accepted.
			{
				fprintf(outputFile, "Accepted Sporadic job %d\n", i);
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

void
scheduler(ScheduleFrame *framesData, int numFrames, int frameSize, AperiodicJob *aperiodicJobs, int numAperiodicJobs, SporadicJob *sporadicJobs, int numSporadicJobs)
{
	/* Psuedo Code:
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
	*/

	FILE *outputFile = fopen(OUTPUT_FILE, "a");

	fprintf(outputFile, "-----------------------------------\n");
	fprintf(outputFile, "Scheduler running now.\n");
	fprintf(outputFile, "Frame size: %d, No of frames: %d\n", frameSize, numFrames);

	float currentTime = 0;
	int aperiodicIndex = 0;
	int acceptedSporadicIndex = 0;
	int periodicJobIndex = 0;
	float runTime;

	for (int f = 0; f < numFrames; ++f)
	{
		fprintf(outputFile, "\n\nFrame no: %d\n", f);
		fprintf(outputFile, "Slack without sporadicJobs: %0.1f\n", framesData[f].slack);

		acceptedSporadicIndex = -1;
		periodicJobIndex = 0;
		currentTime = 0;

		// Add to the list of accepted sporadic jobs if required
		int numSporadicJobsAtPresent;
		float reduceSlackBy;
		SporadicJob *acceptedSporadicJobs = findSporadicJobsInFrame(sporadicJobs, numSporadicJobs, &numSporadicJobsAtPresent, f, framesData[f].slack, &reduceSlackBy, framesData, numFrames);

		for (int i = 0; i < numSporadicJobs; ++i)
		{
			if (sporadicJobs[i].alive && sporadicJobs[i].startFrame <= f && sporadicJobs[i].maxFrame > f)
			{
				bool flag = true;
				for (int j = 0; j < numSporadicJobs; ++j)
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
				else
				{
					sporadicJobs[i].accepted = true;
					sporadicJobs[i].rejected = false;
				}
			}
		}

		if (numSporadicJobsAtPresent != 0)
			acceptedSporadicIndex = 0;



		// Reduce the available slack.
		framesData[f].slack -= reduceSlackBy;

		fprintf(outputFile, "Slack with sporadicJobs: %0.1f\n", framesData[f].slack);
		fflush(outputFile);


		// Sort the accepted sporadic jobs based on EDF
		sortSporadicJobsOnEDF(acceptedSporadicJobs, 0, numSporadicJobsAtPresent - 1);

		while (1)
		{
			if (currentTime >= frameSize)
				break;

			if (framesData[f].slack > 0.0f)
			{
				fprintf(outputFile, "Slack = %0.1f. Slack > 0\n", framesData[f].slack);
				// If sporadic job exists, execute it.
				if (acceptedSporadicIndex != -1)
				{
					fprintf(outputFile, "Sporadic jobs running. Job = S%d.", acceptedSporadicJobs[acceptedSporadicIndex].jobNum);

					runTime = runSporadic(acceptedSporadicJobs, numSporadicJobsAtPresent, &acceptedSporadicIndex, framesData[f].slack);

					fprintf(outputFile, " Ran from %0.1f to %0.1fA\n", currentTime + (f * frameSize), currentTime + (f * frameSize) + runTime);

					fflush(outputFile);

					// Updating slack
					if (runTime <= framesData[f].slack)
						framesData[f].slack -= runTime;

					currentTime += runTime;
					continue;
				}

				// Else if aperiodic job exists, execute it.
				fprintf(outputFile, "Not sporadic\n");
				fflush(outputFile);

				// Check if aperiodic job is available.
				bool aperiodicCheck = checkAperiodicJob(aperiodicJobs, aperiodicIndex, currentTime + (f * frameSize));
				if (aperiodicCheck)
				{
					fprintf(outputFile, "aperiodic execution\n");
					runTime = runAperiodic(aperiodicJobs, numAperiodicJobs, &aperiodicIndex, framesData[f].slack, currentTime, f, frameSize);
					framesData[f].slack -= runTime;
					currentTime += runTime;
					fflush(outputFile);
					if (runTime != 0)
						continue;
				}
				else
				{
					fprintf(outputFile, "--Aperiodic jobs not available.\n");
					fflush(outputFile);
				}

				// Else if periodic job exists, execute it.
				// fprintf(outputFile, "not aperiodic\n");
				if (periodicJobIndex < framesData[f].numPeriodicJobs)
				{
					runTime = runPeriodic(framesData[f].periodicJobs[periodicJobIndex]);

					fprintf(outputFile, "Initial Slack: %0.1f\n", framesData[f].slack);
					fprintf(outputFile, "Periodic Job: J%d with instance: %d running from %0.1f to %0.1f\n", framesData[f].periodicJobs[periodicJobIndex].taskNum, framesData[f].periodicJobs[periodicJobIndex].instanceNum, currentTime + (f * frameSize), currentTime + (f * frameSize) + runTime);

					// Update the DS about the execution.
					framesData[f].periodicJobs[periodicJobIndex].alive = false;
					framesData[f].slack += (framesData[f].periodicJobs[periodicJobIndex].wcet - runTime);

					fprintf(outputFile, "Slack now is: %0.1f\n\n", framesData[f].slack);
					fflush(outputFile);

					periodicJobIndex++;
					currentTime += runTime;

					continue;
				}

				fprintf(outputFile, "not periodic\n");
				fflush(outputFile);

				// Check if aperiodic job is coming before this frame ends.
				// float nextAperiodicJobArrivalTime;
				// if (aperiodicIndex != -1)
				// 	nextAperiodicJobArrivalTime = aperiodicJobs[aperiodicIndex].arrivalTime;
				// else
				// {
				// 	  currentTime = frameSize;
				// 	  continue;
				// }
				// // If yes, sleep till then and then run aperiodic job.
				// if (nextAperiodicJobArrivalTime - (f * frameSize + currentTime) < frameSize)
				// {
				// 	fprintf(outputFile, "yes aperiodic\n");
				// 	currentTime = nextAperiodicJobArrivalTime - (f * frameSize + currentTime);
				// 	continue;
				// }
				// // If no, sleep till the end of the frame.
				// else
				// {
				// 	fprintf(outputFile, "no aperiodic\n");
				// 	currentTime = frameSize;
				// 	continue;
				// }

				currentTime = frameSize;

			}
			else 	// If there is no slack available.
			{	
				// If periodic jobs exist, execute them.
				if (periodicJobIndex < framesData[f].numPeriodicJobs)
				{
					runTime = runPeriodic(framesData[f].periodicJobs[periodicJobIndex]);
					framesData[f].periodicJobs[periodicJobIndex].alive = false;
					fprintf(outputFile, "Initial Slack: %0.1f\n", framesData[f].slack);

					framesData[f].slack += (framesData[f].periodicJobs[periodicJobIndex].wcet - runTime);

					fprintf(outputFile, "Periodic Job: J%d with instance: %d running from %0.1f to %0.1f\n", framesData[f].periodicJobs[periodicJobIndex].taskNum, framesData[f].periodicJobs[periodicJobIndex].instanceNum, currentTime + (f * frameSize), currentTime + (f * frameSize) + runTime);

					fprintf(outputFile, "Slack now is: %0.1f\n\n", framesData[f].slack);
					fflush(outputFile);

					currentTime += runTime;
					periodicJobIndex++;

					continue;
				}

				// Else if sporadic jobs exist, execute them.
				else if (acceptedSporadicIndex != -1)
				{
					fprintf(outputFile, "Sporadic jobs running. Job = S%d. acceptedSporadicIndex = %d", acceptedSporadicJobs[acceptedSporadicIndex].jobNum, acceptedSporadicIndex);
					runTime = runSporadic(acceptedSporadicJobs, numSporadicJobsAtPresent, &acceptedSporadicIndex, frameSize - currentTime);
					fprintf(outputFile, " Ran from %0.1f to %0.1fB\n", currentTime + (f * frameSize), currentTime + (f * frameSize) + runTime);

					fflush(outputFile);

					currentTime += runTime;
					
					continue;
				}
				else
					// Else we go to  the end of the frame.
					currentTime = frameSize;
			}
		}

		// Update sporadic jobs that finished.
		for (int i = 0; i < numSporadicJobsAtPresent; ++i)
		{
			if (acceptedSporadicJobs[i].alive == false)
			{
				for (int j = 0; j < numSporadicJobs; ++j)
				{
					if (acceptedSporadicJobs[i].jobNum == sporadicJobs[j].jobNum)
					{
						sporadicJobs[j].alive = false;
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
						fprintf(outputFile, "hit\n");
						sporadicJobs[j].timeLeft = acceptedSporadicJobs[i].timeLeft;
						break;
					}
				}
			}
		}
	}
	fprintf(outputFile, "\n\nFinished Scheduling.\n----------------------------\n");
	fclose(outputFile);

	return;
}