#include <stdlib.h>
#include "rmemcheck.h" // should be last header

int main(int argc, char *argv[]) {
	//set_log_file("mem_leak.log");
	char *s1 = (char *) malloc(10);
	char *s2 = (char *) calloc(10, 2);	
	char *s3 = (char *) realloc(s2, 50);
	char *s4 = strdup("Rajendra");
	free(s1);
	return 0;
}
