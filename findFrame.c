/*
 * Author: Akhil Tarikere
 * Date: 11/3/20
 * 
 * Pre-Condition: Takes in the inputs of various ADTs.
 * 		
 * Post-Condition: Calculates which frames the jobs will have to execute in.
 *	
 */


/*
 * Finds the frames at which a sporadic job can run in.
 */
void
findSporadicJobFrame(SporadicJob *sporadicJobs, int numJobs, int frameSize)
{
	for (int i = 0; i < numJobs; ++i)
	{
		// [startFrame, maxFrame);
		// [.] => can use that frame as well.
		// (.) => cannot use that frame.
		sporadicJobs[i].startFrame = sporadicJobs[i].arrivalTime / frameSize;
		sporadicJobs[i].maxFrame = sporadicJobs[i].deadline / frameSize;
	}
}