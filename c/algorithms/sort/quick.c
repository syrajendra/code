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

// 3 - way partitioning
// for arrays with large numbers of duplicate keys
void quick_sort_3_way(int *array, int low, int high)
{
	if(high <= low) return;
	int lt = low, gt = high;	
	int pe = array[low];
	int curr = low + 1;	
	while(curr <= gt) {
		if(array[curr] < pe) swap(array, lt++, curr++);
		else if(array[curr] > pe) swap(array, curr, gt--);
		else curr++;
	}
	quick_sort_3_way(array, low, lt-1);
	quick_sort_3_way(array, gt+1, high);
}

// Partitioning process rearranges array to hold below three conditions:
// Assume if partition element is at index j then
// 1. No entry in a[low] through a[j-1] is greater than a[j]
// 2. No entry in a[j+1] through a[high] is less than a[j]
// 3. The entry a[j] should be in its final place in the array
int partition(int *array, int low, int high)
{
	int left  = low, right = high + 1;
	int pe = array[low];
	while(1) {
		// Increment left pointer until array value is less than partition item
		while(array[++left] < pe) if(left == high) break;
		// Decrement right pointer until partition item is less than array value
		while(pe < array[--right]) if(right == low)  break;
		// Exit loop when left & right pointer cross 
		if(left >= right) break;
		// Swap 
		// if a[left] greater than partition element
		// if a[right] less than partition element
		swap(array, left, right);
	}
	// Move partition element in its final place
	swap(array, low, right); 
	return right;
}

// Algorithm 2.5 page 289
// Divide-And-Conquer algorithm
// 2 - way partitioning 
// Works by partitioning an array into two subarrays then rearrange the array 
// such that, when the two subarrays are sorted, the whole array is ordered
// Worst Case : O(n^2) Best/Avg Case: O(n log n) => Linearithmic/Super linear time
// Uses stack space of O(log n)
// Not stable & in-place algorithm
void quick_sort(int *array, int low, int high)
{
	if(high <= low) return;
	int pe_index = partition(array, low, high);
	quick_sort(array, low, pe_index-1);
	quick_sort(array, pe_index+1, high);
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *array = (int *) calloc(sizeof(int), argc-1);		
		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);		
		quick_sort(array, 0, argc-2);
		printf("Quick sort 2-way: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");

		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		quick_sort_3_way(array, 0, argc-2);
		printf("Quick sort 3-way: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");		
		free(array);
	}
	return 0;
}