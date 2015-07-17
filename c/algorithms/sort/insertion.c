#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Book: 
//    Algorithms 4th Edition By Robert Sedgewick and Kevin Wayne

int swap(int *array, int i, int j)
{
	int tmp   = array[i];
	array[i]  = array[j];
	array[j]= tmp;
}

// Cards game: inserting each item into its proper place among those already sorted
// Insertion sort is an in-place & stable sort algorithm
// Average & Worst case : ~ N^2/4 compares & exchanges => O(n^2) time => Quadratic time
// Best case : ~ N^2/2 compares & exchanges => O(n) => Linear time
void insertion_sort(int *array, int length)
{
	int i, j;
	// find smallest element before current position	
#ifdef SIMPLE 
	// Below code also works but j loop simply runs for all previous values which is not needed
	for(i=0; i<length; i++) for(j=i; j>=0 && array[j] < array[j-1]; j--) swap(array, j, j-1);
#else
	// Optimized: j loop runs until current item less than previous item
	for(i=0; i<length; i++) 
		for(j=i; j>=0; j--) {
			if(array[j] > array[j-1]) break; // previous items are already sorted
			swap(array, j, j-1);			
		}
#endif
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *array = (int *) calloc(sizeof(int), argc-1);
		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		insertion_sort(array, argc-1);
		printf("Insertion sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");
		free(array);
	}
	return 0;
}

