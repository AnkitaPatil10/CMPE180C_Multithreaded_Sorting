
/**
 * CMPE 180C Operating System Project
 * Perform sorting on a list of integers using multiple-threads.
 * The program creates 2 threads, each for sorting half of the array.
 * Then finally the program creates 3 thread which performs merge of the 2 sorted subarrays.
 *
 * Author: Ankita Patil, Charit Upadhyay
 *         Department of Computer Engineering
 *         San Jose State University
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void *runner(void *param); /* threads call this function */
int array_master[2000];
#define MAX 2000


void merge(int arr[], int left, int middle, int right) {

    int i, j, k;
    int half1 = middle - left + 1;
    int half2 = right - middle;

    int first[half1], second[half2]; // temp arrays

    for (i = 0; i < half1; i++) {
        first[i] = arr[left + i];
    }

    for (j = 0; j < half2; j++) {
        second[j] = arr[middle + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < half1 && j < half2) {

        if (first[i] <= second[j]) {
            arr[k] = first[i];
            ++i;
        } else {
            arr[k] = second[j];
            j++;
        }

        k++;
    }

    while (i < half1) {
        arr[k] = first[i];
        i++;
        k++;
    }

    while (j < half2) {
        arr[k] = second[j];
        j++;
        k++;
    }
}

// A utility function to swap two elements
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (int arr[], int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[])
{

    /* Intializes random number generator */
    srand((unsigned) time(NULL));
		// generating random values in array
    for (int i = 0; i < MAX; i++)
    {
        array_master[i] = rand() % 100 ;
    }
    printf ( "Unsorted array: \n");
    for (int i = 0; i < MAX; i++)
        printf ("%d ", array_master[i] );

    printf("\n \n");

		// t1 and t2 for calculating time for
		// merge sort
    clock_t start_time, end_time;
    double time_difference;

    start_time = clock();

   // printf("Initialise first thread");
    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of thread attributes */

    pthread_t tid2; // second thread id
    pthread_attr_t attr2; // second thread attributes

    pthread_t tid3; // third thread id
    pthread_attr_t attr3;


    char *thread_no = "first";

    pthread_attr_init(&attr); /* get the default attributes */
  //  printf("Create first thread");
    pthread_create(&tid,&attr,runner, thread_no); /* create the thread */


    char *thread2 = "second";
    pthread_attr_init(&attr2);
    pthread_create(&tid2,&attr2, runner, thread2);
    /* wait for the thread to exit */

    char *thread3 = "third";
    pthread_attr_init(&attr3);
    pthread_create(&tid3, &attr2, runner, thread3);

    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    end_time = clock();
    printf("Sorted Array:\n");
    int j;
    for (j = 0; j < MAX; j++) {
        if (j == MAX-1) {
            printf("%d", array_master[j]);
        } else {
            printf("%d ", array_master[j]);
        }
    }
    time_difference = (double)(end_time - start_time)/CLOCKS_PER_SEC;
    printf("\n\nTotal time taken by CPU: %f\n", time_difference  );
    pthread_exit(0);

    return 0;
}


void *runner(void *param)
{
    int midpoint = MAX/2;
    if (strcmp(param, "first") == 0) {
        quickSort(array_master, 0, midpoint);
    }

    if (strcmp(param, "second") == 0) {
        quickSort(array_master, midpoint + 1, MAX - 1);
    }

    if (strcmp(param, "third") == 0) {
        merge(array_master, 0, (MAX/2), MAX-1);
    }

    pthread_exit(0);
}

