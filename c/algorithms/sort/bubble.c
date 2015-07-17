#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// http://en.wikipedia.org/wiki/Bubble_sort

int swap(int *array, int i, int min)
{
	int tmp   = array[i];
	array[i]  = array[min];
	array[min]= tmp;
}

// It works by repeatedly swapping adjacent elements that are out of order
// Average & Worst case : ~ N^2 compares & exchanges => O(n^2) time => Quadratic time
// Best case : ~ N compares & exchanges => O(n) => Linear time
// Stable & in-place algorithm
void bubble_sort(int *array, int length)
{
	int i, swapped = 0;
	do {	
		swapped = 0;	
		for(i=1; i<=length-1; i++) {
			if(array[i-1] > array[i]) {
				swap(array, i-1, i);
				swapped = 1;
			}
		}
	} while(swapped);
}

void optimized_bubble_sort(int *array, int length)
{
	int i;
	do {
		int new_length = 0;
		for(i=1; i<=length-1; i++) { // compare only till the last swapped position
			if(array[i-1] > array[i]) {
				swap(array, i-1, i);			
				new_length = i; 
			}
		}
		length = new_length;
	} while(length);
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *array = (int *) calloc(sizeof(int), argc-1);
		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		bubble_sort(array, argc-1);
		printf("Bubble sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");

		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		optimized_bubble_sort(array, argc-1);
		printf("Optimized Bubble sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");
		free(array);
	}
	return 0;
}