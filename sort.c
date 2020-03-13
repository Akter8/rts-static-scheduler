/*
 * Author: Akhil Tarikere
 * Date: 11/3/20
 * 
 * Pre-Condition: Input of ADTs in an unsorted manner.
 * 		
 *
 * Post-Condition: Return ADTs in a sorted manner.
 *	
 */


#include "structNonPeriodic.h"
#include "struct.h"


/*
 * Sorting sporadic jobs based on arrival time.
 * Using merge sort algorithm.
 */
void 
mergeSporadicJobs(SporadicJob *arr, int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 

	// create temp arrays
	SporadicJob L[n1], R[n2]; 

	// Copy data to temp arrays L[] and R[] 
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1+ j]; 

	// Merge the temp arrays back into arr[l..r]
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2) 
	{ 
		if ((L[i].arrivalTime < R[j].arrivalTime) || (L[i].arrivalTime == R[j].arrivalTime && L[i].wcet <= R[j].wcet))
		{ 
			arr[k] = L[i]; 
			i++; 
		} 
		else
		{ 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	// Copy the remaining elements of L[], if there are any
	while (i < n1) 
	{ 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 

	// Copy the remaining elements of R[], if there are any 
	while (j < n2) 
	{ 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
} 


void
sortSporadicJobs(SporadicJob *arr, int l, int r)
{
	if (l < r) 
	{ 
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		int m = l+(r-l)/2; 

		// Sort first and second halves 
		sortSporadicJobs(arr, l, m); 
		sortSporadicJobs(arr, m+1, r); 

		mergeSporadicJobs(arr, l, m, r); 
	} 
}



/*
 * Sorting aperiodic jobs based on arrival time.
 * Using merge sort algorithm.
 */
void 
mergeAperiodicJobs(AperiodicJob *arr, int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 

	// create temp arrays
	AperiodicJob L[n1], R[n2]; 

	// Copy data to temp arrays L[] and R[] 
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1+ j]; 

	// Merge the temp arrays back into arr[l..r]
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2) 
	{ 
		if ((L[i].arrivalTime < R[j].arrivalTime) || (L[i].arrivalTime == R[j].arrivalTime && L[i].wcet <= R[j].wcet)) 
		{ 
			arr[k] = L[i]; 
			i++; 
		} 
		else
		{ 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	// Copy the remaining elements of L[], if there are any
	while (i < n1) 
	{ 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 

	// Copy the remaining elements of R[], if there are any 
	while (j < n2) 
	{ 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
}

void
sortAperiodicJobs(AperiodicJob *arr, int l, int r)
{
	if (l < r) 
	{ 
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		int m = l+(r-l)/2; 

		// Sort first and second halves 
		sortAperiodicJobs(arr, l, m); 
		sortAperiodicJobs(arr, m+1, r); 

		mergeAperiodicJobs(arr, l, m, r); 
	} 
}



/*
 * Sorting periodic tasks based on their periods.
 * Using merge sort algorithm.
 */
void 
mergeTasks(Task *arr, int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 

	// create temp arrays
	Task L[n1], R[n2]; 

	// Copy data to temp arrays L[] and R[] 
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1+ j]; 

	// Merge the temp arrays back into arr[l..r]
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2) 
	{ 
		if ((L[i].period < R[j].period) || (L[i].period == R[j].period && L[i].wcet <= R[j].wcet)) 
		{ 
			arr[k] = L[i]; 
			i++; 
		} 
		else
		{ 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	// Copy the remaining elements of L[], if there are any
	while (i < n1) 
	{ 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 

	// Copy the remaining elements of R[], if there are any 
	while (j < n2) 
	{ 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
}

void
sortTasks(Task *arr, int l, int r)
{
	if (l < r) 
	{ 
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h.
		int m = l+(r-l)/2; 

		// Sort first and second halves.
		sortTasks(arr, l, m); 
		sortTasks(arr, m+1, r); 

		mergeTasks(arr, l, m, r); 
	} 
}



/*
 * Sorting accepted sporadic jobs based on their deadline for EDF.
 * Using merge sort algorithm.
 */
void 
mergeSporadicJobsOnEDF(SporadicJob *arr, int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 

	// create temp arrays
	SporadicJob L[n1], R[n2]; 

	// Copy data to temp arrays L[] and R[] 
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1+ j]; 

	// Merge the temp arrays back into arr[l..r]
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2) 
	{ 
		if ((L[i].maxFrame < R[j].maxFrame) || (L[i].maxFrame == R[j].maxFrame && L[i].wcet <= R[j].wcet)) 
		{ 
			arr[k] = L[i]; 
			i++; 
		} 
		else
		{ 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	// Copy the remaining elements of L[], if there are any
	while (i < n1) 
	{ 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 

	// Copy the remaining elements of R[], if there are any 
	while (j < n2) 
	{ 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
}

void
sortSporadicJobsOnEDF(SporadicJob *arr, int l, int r)
{
	if (l < r) 
	{ 
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h.
		int m = l+(r-l)/2; 

		// Sort first and second halves.
		sortSporadicJobsOnEDF(arr, l, m); 
		sortSporadicJobsOnEDF(arr, m+1, r); 

		mergeSporadicJobsOnEDF(arr, l, m, r); 
	} 
}