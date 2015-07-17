#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Book
// Introduction to algorithms 3rd Edition By Thomas H Cormen

#define PARENT(in) (floor(in >> 1))	// i.e. root/2
#define LEFT(in)   (in << 1)		// i.e. root*2
#define RIGHT(in)  (LEFT(in) + 1) 	// i.e. root*2+1

int num_slashes(int height, int level)
{
	return (pow(2, height-level)/height);
}

void print_level(int *array, int height, int level,  int *points)
{
	int i, k;
	int nodes = pow(2, level);
	int front = nodes - 1;
	int links = nodes * 2;
	int prev  = 0;
	int slash = num_slashes(height, level);
	//printf("\nNodes:%d front:%d links:%d height:%d slash:%d \n", nodes, front, links, height, slash);

	for(i=0; i<nodes; i++) {		
		printf("%*d", points[front+i] - prev, array[front+i+1]); // root starts at 1
		//printf("%*s", points[front+i] - prev, "Nil");
		prev = points[front+i];
	}
	printf("\n");
	
	for(k=1; k<=slash; k++) {		
		prev = k;
		for(i=0; i<nodes; i++) {			
			printf("%*s%*s", points[front+i]-prev, "/", k*2, "\\");			
			prev = points[front+i]+k*2;
			
		}
		if(k < slash) printf("\n");
	}	
	printf("\n");
	if(level < height-3) {		
		for(i=0, k=0; i<links; i+=2, k++) {
			points[nodes+front+i]   = points[front+k] - slash;
			points[nodes+front+i+1] = points[front+k] + slash + 1;		
		}
	}	
}

int left_slash_count(int height)
{
	int tmp = 0;
	int count = 0;	
	while(count<height) {
		tmp += num_slashes(height, count);		
		count++;
	}
	return tmp;
}

void print_array(char *msg, int *array, int length)
{
	int i;
	printf("\n%s : ", msg);
	for(i=1; i<=length; i++) printf(" array[%d]=%d ", i, array[i]);
	printf("\n");
}

void print_tree(char *msg, int *array, int length)
{
	int i;
	int height    = floor(log2(length)) + 1;	
	int max_nodes = pow(2, height) - 1;
	int *points   = (int *) calloc(sizeof(int), max_nodes);
	int updated_height = height+2;
	points[0]     = 1 + left_slash_count(updated_height);
	print_array(msg, array, length);
	//printf("Height: %d nodes: %d slash_count:%d leftnodes:%d middle: %d\n"
	//	, height, max_nodes, left_slash_count(updated_height), ((max_nodes+1)/4), points[0]);	
	for(i=0; i<height; i++) print_level(array, updated_height, i, points);		
	//for(i=0; i<max_nodes; i++) printf("points[%d]=%d\n", i, points[i]);
	free(points);
}

int swap(int *array, int i, int j)
{
	int tmp   = array[i];
	array[i]  = array[j];
	array[j]  = tmp;
}

// Min-heap property:
// For every node i other than the root,
//       Array[PARENT[i]] <= Array[i]
// i.e. the smallest element in a min-heap is stored at the root
// Logarithmic Time : O(log n)
void min_heapify(int *array, int index, int length)
{
	int left  	= LEFT(index);
	int right 	= RIGHT(index);
	int smallest 	= index;
	if(left  <= length && array[left]  < array[index])   smallest = left;
	if(right <= length && array[right] < array[smallest]) smallest = right;
	if(smallest != index) {
		swap(array, index, smallest);		
		min_heapify(array, smallest, length);
	}
}


// Max-heap property:
// For every node i other than the root,
//       Array[PARENT[i]] >= Array[i]
// i.e. the largest element in a max-heap is stored at the root
// Logarithmic Time : O(log n)
void max_heapify(int *array, int index, int length)
{
	//char tmp[64];
	//sprintf(tmp, "Max heapify index: %d", index);
	//print_tree(tmp, array, length);
	int left  	= LEFT(index);
	int right 	= RIGHT(index);
	int largest 	= index;
	if(left  <= length && array[left]  > array[index])   largest = left;
	if(right <= length && array[right] > array[largest]) largest = right;
	if(largest != index) {
		swap(array, index, largest);		
		max_heapify(array, largest, length);
	}
}

int find_largest(int *array, int index, int length, int *arg_lar)
{
	int left  	= LEFT(index);
	int right 	= RIGHT(index);
	int largest 	= index;
	if(left  <= length && array[left]  > array[index])   largest = left;
	if(right <= length && array[right] > array[largest]) largest = right;
	if(largest != index) {
		swap(array, index, largest);
		*arg_lar = largest;
		return 1;
	}
	return 0;
}

void max_heapify_non_recursive(int *array, int index, int length)
{	
	int largest = index;
	int ret;
	do {
		index = largest;
		ret = find_largest(array, index, length, &largest);		
	} while(ret);	
}


// Linear Time : O(n)
void build_heap(int *array, int length, 
	void (*heapify)(int *array, int index, int length))
{
	double i;
	for(i=floor(length/2);i>=1; i--) heapify(array, i, length);	
	print_tree("Building heap", array, length);
}


// Viewing heap as a binary tree with height h, 
//    minimum number of elements = 2 ^ h - h
//    maximum number of elements = 2 ^ h - 1
// N - element heap tree has height of floor(log N)
// All leaves : n/2 + 1, n/2 + 2, n/2 + 3 .... n
// Time : O(n log n)
void max_heap_sort(int *array, int length)
{
	int hsize;
	build_heap(array, length, max_heapify);	
	for(hsize=length; hsize>=2; hsize--) {
		swap(array, 1, hsize);		
		max_heapify(array, 1, hsize-1);
	}
}

void min_heap_sort(int *array, int length)
{
	int hsize;
	build_heap(array, length, min_heapify);
	for(hsize=length; hsize>=2; hsize--) {
		swap(array, 1, hsize);
		min_heapify(array, 1, hsize-1);
	}
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int length = argc-1;
		// number of nodes = (2 ^ N - 1) height = floor(log(N)) + 1
		int height = floor(log2(length)) + 1;
		int extra  = (pow(2, 4) - 1) - length; // extra memory for NIL nodes of tree
		extra++; // root starts at index 1		
		int *array = (int *) calloc(sizeof(int), length+extra);
		
		for(i=1; i<=length; i++) array[i] = atoi(argv[i]); // root index is 1
		printf("============================================\n");
		max_heap_sort(array, length);
		printf("Max heap sort: \n");
		for(i=1; i<=length; i++) printf(" %d ", array[i]);
		printf("\n");		
		printf("============================================\n");

		for(i=1; i<=length; i++) array[i] = atoi(argv[i]); // root index is 1
		min_heap_sort(array, length);
		printf("Min heap sort: \n");
		for(i=1; i<=length; i++) printf(" %d ", array[i]);
		printf("\n");		
		printf("============================================\n");
		free(array);
	}
	return 0;
}