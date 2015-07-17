#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Book: 
//    Algorithms 4th Edition By Robert Sedgewick and Kevin Wayne

void merge(int *array, int *aux,int low, int mid, int high)
{
	int i, j = low, k = mid + 1;
	for(i=low; i<=high; i++) aux[i] = array[i]; // copy to auxiliary array
	for(i=low; i<=high; i++) {
		if(j > mid)			array[i] = aux[k++];
		else if (k > high)		array[i] = aux[j++];
		else if (aux[k] < aux[j])	array[i] = aux[k++];
		else				array[i] = aux[j++];	 
	}
}

// Divide-And-Conquer algorithm
// Combining two ordered arrays to make one larger ordered array
// Time  = O(nlogn) => Linearithmic/Super linear time
// Space = Extra space equal to input items N = O(n)
// Stable & in-place algorithm
void top_down_merge_sort(int *array, int *aux, int low, int high)
{
	if(high <= low) return;
	int mid = low + (high-low)/2;
	top_down_merge_sort(array, aux, low, mid);
	top_down_merge_sort(array, aux, mid+1, high);
	merge(array, aux, low, mid, high);
}

void bottom_up_merge_sort(int *array, int *aux, int length)
{
	int i, j;
	for(i=1; i<length; i+=i) // 1, 2, 4, 8, 16, 32		
		for(j=0; j<length-i; j+=i+i) 
			merge(array, aux, j, j+i-1, ((j+i+i-1) < length-1) 
							? (j+i+i-1) 
							: length-1);
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *array = (int *) calloc(sizeof(int), argc-1);
		int *aux   = (int *) calloc(sizeof(int), argc-1);
		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		top_down_merge_sort(array, aux, 0, argc-2);
		printf("Top down merge sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");

		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		bottom_up_merge_sort(array, aux, argc-1);
		printf("Bottom up merge sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");
		free(array);
		free(aux);
	}
	return 0;
}

