#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int swap(int *array, int i, int j)
{
	int tmp   = array[i];
	array[i]  = array[j];
	array[j]  = tmp;
}

void gnome_sort(int *array, int length)
{
	int i;
	// find smallest element before current position
	for(i=1; i<length;) {
		if(i == 0 || array[i-1] <= array[i]) ++i;
		else {
			swap(array, i, i-1);
			--i;
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int i;
		int *array = (int *) calloc(sizeof(int), argc-1);
		for(i=1; i<argc; i++) array[i-1] = atoi(argv[i]);
		gnome_sort(array, argc-1);
		printf("Gnome sort: \n");
		for(i=0; i<argc-1; i++) printf(" %d ", array[i]);
		printf("\n");
		free(array);
	}
	return 0;
}