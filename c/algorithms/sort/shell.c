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

void insertion_sort(int *array, int curr, int length)
{
	int i, j;
	// find smallest element before current position
	for(i=curr; i<length; i++) for(j=i; j>=curr && array[j] < array[j-curr]; j-=curr) swap(array, j, j-curr);
}

// ALGORITHM 2.3 page 259
// Is a simple extension of insertion sort that gains speed by allowing exchanges of array entries that are far apart
// Average case time is O(n^1.25)
// Worst case time is O(n^1.5)
// Not stable & in-place algorithm
void shell_sort(int *array, int length)
{
	int h = 1;
	while(h < length/3) h = 3 * h + 1; // 1, 4, 13, 40, 121 ...
	while(h >= 1) {
		insertion_sort(array, h, length);
		h = h/3;
	}
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *array = (int *) calloc(sizeof(int), argc-1);
		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		shell_sort(array, argc-1);
		printf("Shell sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");
		free(array);
	}
	return 0;
}