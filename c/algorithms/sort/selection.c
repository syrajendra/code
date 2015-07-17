#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Book: 
//    Algorithms 4th Edition By Robert Sedgewick and Kevin Wayne

int swap(int *array, int i, int min)
{
	int tmp   = array[i];
	array[i]  = array[min];
	array[min]= tmp;
}

// Find smallest element after current position
int find_minimum(int *array, int start, int end)
{
	int i, min =  start - 1;
	for(i=start; i<end; i++) if(array[i] < array[min]) min = i;
	return min;
}

// ALGORITHM 2.1 - page 249
// It works by repeatedly selecting the smallest remaining item
// ~ N^2/2 compares & N exchanges => O(n^2) time for best, average & worst cases => Quadratic time
// Not stable & in-place algorithm
void selection_sort(int *array, int length)
{
	int i = 0;
	for(i=0; i<length; i++) swap(array, i, find_minimum(array, i+1, length));	
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *array = (int *) calloc(sizeof(int), argc-1);
		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		selection_sort(array, argc-1);
		printf("Selection sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");
		free(array);
	}
	return 0;
}