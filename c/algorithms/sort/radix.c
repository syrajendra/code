#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int max_number(int *input, int length)
{
	int i, max = INT_MIN;
	for(i=0; i<length; i++)	if(input[i] > max) max = input[i];
	return max;
}

void lsd_radix_sort(int *input, int *output, int length)
{
	int i, order = 1, max = max_number(input, length);
	do {
		int bucket[10] = {0};
		for(i=0; i<length; i++) 
			bucket[(input[i]/order) % 10]++;
		for(i=0; i<10; i++)
			bucket[i] += bucket[i-1];
		for(i=length-1; i>=0; i--)
			output[--bucket[(input[i]/order) % 10]] = input[i];
		order *= 10;

	} while(max/order);

}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *input  = (int *) calloc(sizeof(int), argc);
		int *output = (int *) calloc(sizeof(int), argc);
		for(i=1; i<argc; i++) input[i-1] = atoi(argv[i]);
		lsd_radix_sort(input, output, argc-1);
		printf("LSD radix sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", output[i]);
		printf("\n");
		free(input);
		free(output);
	}
	return 0;
}