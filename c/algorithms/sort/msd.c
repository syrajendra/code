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

// This algorithm is based on quick sort + counting sort
void msd(char **input, int low, int high, int digit)
{
	int i, r;
	if(high <= low) return;
	int count[256+2] = {0};
	char *aux[256]   = {NULL};
	for (i=low; i<=high; i++) // Compute frequency
		count[input[i][digit] + 2]++;

	for (i=0; i<256+1; i++)   // Transform counts to indices
		count[i+1] += count[i];

	for (i=low; i<=high; i++) // Distribute
		aux[count[input[i][digit] + 1]++] = input[i];

	for (i=low; i<=high; i++) // Copy back
		input[i] = aux[i - low];

	// Recursively sort for each character value
	for (r=0; r<256; r++)
		msd(input, low + count[r], low + count[r+1] - 1, digit+1);
}

int main(int argc, char *argv[])
{
	int i;
	int total = 13;
	msd(input, 0, total-1, 0);
	printf("MSD sorted strings:\n");
	for(i=0; i<total; i++) {
		printf("input[%d]=%s\n", i, input[i]);
	}
	return 0;
}