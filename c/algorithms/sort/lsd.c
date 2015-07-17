#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Book: 
//    Algorithms 4th Edition By Robert Sedgewick and Kevin Wayne

char *input[] = {
		"4PGC938",
		"2IYE230",
		"3CIO720",
		"1ICK750",
		"1OHV845",
		"4JZY524",
		"1ICK750",
		"3CIO720",
		"1OHV845",
		"1OHV845",
		"2RLA629",
		"2RLA629",
		"3ATW723"
};

// This algorithm is based on counting sort
// ALGORITHM 5.1 - page 707
// Least Significant Digit first (LSD) string sort stably sorts fixed-length strings
// Time: linear time { N*W where N is input size & W is width of string }
void lsd(char **input, int len)
{
	int i, j, k;	
	int width = strlen(input[0]);
	printf("String length: %d\nInput:\n", width);	
	for(i=0; i<len; i++) {
		printf("input[%d]=%s\n", i, input[i]);
	}	
	
	for(i=width-1; i>=0; i--) { // for each char in every string
		int count[256] = {0}; // radix
		char *aux[256] = {NULL};			
		for(j=0; j<len; j++) { // STEP1: compute frequency of char (number of times a char occurred)
			count[input[j][i]+1]++;
		}
		for(j=0; j<256; j++) { // STEP2: find start indexes of char
			count[j+1] += count[j];
		}
		for(j=0; j<len; j++) { // STEP3: copy in correct place		
			int index = count[input[j][i]]++;
			aux[index] = input[j];			
		}
		for(j=0; j<len; j++) {	// STEP4: copy back					
			input[j] = aux[j];
			aux[j]   = NULL;
 		}		
	}		
}

int main(int argc, char *argv[])
{
	int i;
	int total = 13;
	lsd(input, total);
	printf("LSD sorted strings:\n");
	for(i=0; i<total; i++) {
		printf("input[%d]=%s\n", i, input[i]);
	}
	return 0;
}