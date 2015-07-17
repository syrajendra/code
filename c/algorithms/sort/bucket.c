#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NUM 10

void bucket_sort(int *input, int length)
{
	int i, j;
	int bucket[MAX_NUM] = {0};
	for(i=0; i<length; i++) bucket[input[i]]++; // frequency count
	for(i=0, j=0; i<MAX_NUM; i++) while(bucket[i]--) input[j++] = i;
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *input  = (int *) calloc(sizeof(int), argc);		
		for(i=1; i<argc; i++) {
			input[i-1] = atoi(argv[i]);
			if(input[i-1] > MAX_NUM) {
				printf("Error: Sort element cannot exceed %d\n", MAX_NUM);
				exit(1);
			}
		}
		bucket_sort(input, argc-1);
		printf("Bucket sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", input[i]);
		printf("\n");
		free(input);
	}
	return 0;
}