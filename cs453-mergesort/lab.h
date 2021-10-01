/**
 * Name: Jake Beslanowitch
 * Email: jakebeslanowitch@u.boisestate.edu
 * Github Username: jakebeslanowitch
 */

#ifndef __LAB_H
#define __LAB_H

#ifdef __cplusplus
extern "C" {
#endif

#define TRUE 1
#define FALSE 0

// function prototypes
void serial_mergesort(int A[], int p, int r);
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
void *parallel_sort(void *arg);
void parallel_mergesort(int A[], int p, int r, int numLevels);


const int INSERTION_SORT_THRESHOLD = 100; //based on trial and error

struct arg_threads
{
	int p;
	int r;
	int numLevels;
    int *A;
};


#ifdef __cplusplus
}
#endif
#endif //__LAB_H
