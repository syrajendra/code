#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NUM 5

void print_array(int *array, int length)
{
	while(length--) printf(" %d ", *array++);
	printf("\n");
}

// Book: 
//    Algorithms 4th Edition By Robert Sedgewick and Kevin Wayne
void counting_sort_1(int *input, int *output, int length)
{
	int *count = (int *) calloc(sizeof(int), MAX_NUM+1);
	int i, j;
	for(i=0; i<length; i++)
		count[input[i]+1]++; //compute frequency counts
	for(i=0; i<=MAX_NUM; i++)
		count[i+1] += count[i]; // transform counts to indices
	for(i=0; i<length; i++) // distribute the data
		output[count[input[i]]++] = input[i]; 
	free(count);
}

// Book: 
// Introduction to algorithms By Thomas H. Cormen
void counting_sort_2(int *input, int *output, int length)
{
	int *count = (int *) calloc(sizeof(int), MAX_NUM+1);
	int i, j;
	for(i=0; i<length; i++)
		count[input[i]]++; //count each char
	for(i=1; i<=MAX_NUM; i++)
		count[i] += count[i-1]; // calculate start positions
	print_array(count, MAX_NUM);
	for(i=0; i<length; i++) {
		output[count[input[i]]-1] = input[i]; 
		--count[input[i]];
	}
	free(count);
}


int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *input  = (int *) calloc(sizeof(int), argc);
		int *output = (int *) calloc(sizeof(int), argc);
		for(i=1; i<argc; i++) {
			input[i-1] = atoi(argv[i]);
			if(input[i-1] > MAX_NUM) {
				printf("Error: Sort element cannot exceed %d\n", MAX_NUM);
				exit(1);
			}
		}
		counting_sort_1(input, output, argc-1);
		printf("Counting sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", output[i]);
		printf("\n");
		free(input);
		free(output);
	}
	return 0;
}