/**
 * Name: Jake Beslanowitch
 * Email: jakebeslanowitch@u.boisestate.edu
 * Github Username: jakebeslanowitch
 */
#include "lab.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

/*
 * insertion_sort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void insertion_sort(int A[], int p, int r)
{
	int j;

	for (j=p+1; j<=r; j++) {
		int key = A[j];
		int i = j-1;
		while ((i > p-1) && (A[i] > key)) {
			A[i+1] = A[i];
			i--;
		}
		A[i+1] = key;
	}
}

/*
 * serial_mergesort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void serial_mergesort(int A[], int p, int r)
{
	if (r-p+1 <= INSERTION_SORT_THRESHOLD)  {
			insertion_sort(A,p,r);
	} else {
		int q =  r + (p - r)/2;
		serial_mergesort(A, p, q);
		serial_mergesort(A, q+1, r);
		merge(A, p, q, r);
	}
}

/*
 * parallel_sorter(void *sArg):
 *
 * description: 
 */
void *parallel_sorter(void *sArg)
{
	int midPoint;
	int i;
	struct arg_threads *thread;
	thread = (struct arg_threads *) sArg;

	// base case - using serial_mergesort for the base case
	if((*thread).numLevels < 1) {
		serial_mergesort((*thread).A, (*thread).p, (*thread).r);
		return NULL;
	}

	struct arg_threads sArg2[2];

	// finds midpoint
	midPoint = (*thread).r + ((*thread).p - (*thread).r) / 2; 
	
	for(i = 0; i < 2; i++) {
		sArg2[i].A = (*thread).A;									// reference to the array used by the thread
		sArg2[i].numLevels = (*thread).numLevels - 1;				// dec numLevels 
	}

	// splitting into two sub parts
	// Arr[p] to midPoint 
	sArg2[0].p = (*thread).p;					// low 
	sArg2[0].r = midPoint;						// mid
	// midPoint point to end of Arr[r]
	sArg2[1].p = midPoint + 1;					// mid + 1
	sArg2[1].r = (*thread).r;					// high

	pthread_t threadT[2];	

	// creates threads for both sides of the array [Thread 1 & Thread 2]
	for(i = 0; i < 2; i++) {
		pthread_create(&threadT[i], 0, parallel_sorter, &sArg2[i]);
	}

	// Waits for the children threads to stop running 
	for(i = 0; i < 2; i++) {
		pthread_join(threadT[i], 0);
	}

	// merges the results
	merge((*thread).A, (*thread).p, midPoint, (*thread).r);

	return NULL;
}

/*
 * parallel_mergesort(int A[], int p, int q, int r):
 *
 * description: Merge two sorted sequences A[p..q] and A[q+1..r]
 *              and place merged output back in array A. Uses extra
 *              space proportional to A[p..r].
 */

void parallel_mergesort(int A[], int p, int r, int numLvls)
{
	struct arg_threads validArg;
	validArg.A = A;
	validArg.p = p;
	validArg.r = r;
	validArg.numLevels = numLvls;

	// data type to use with pthread_create
	pthread_t oThread;

	// Creating thread for parallel_sorter
	pthread_create(&oThread, 0, parallel_sorter, &validArg);
	pthread_join(oThread, 0); 		
}

/*
 * merge(int A[], int p, int q, int r):
 *
 * description: Merge two sorted sequences A[p..q] and A[q+1..r]
 *              and place merged output back in array A. Uses extra
 *              space proportional to A[p..r].
 */
void merge(int A[], int p, int q, int r)
{
	int *B = (int *) malloc(sizeof(int) * (r-p+1));

	int i = p;
	int j = q+1;
	int k = 0;
	int l;

	// as long as both lists have unexamined elements
	// this loop keeps executing.
	while ((i <= q) && (j <= r)) {
		if (A[i] < A[j]) {
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}
		k++;
	}

	// now only at most one list has unprocessed elements.

	if (i <= q) {
		// copy remaining elements from the first list
		for (l=i; l<=q; l++) {
			B[k] = A[l];
			k++;
		}
	} else {
		// copy remaining elements from the second list
		for (l=j; l<=r; l++) {
			B[k] = A[l];
			k++;
		}
	}

	// copy merged output from array B back to array A
	k=0;
	for (l=p; l<=r; l++) {
		A[l] = B[k];
		k++;
	}

	free(B);
}
