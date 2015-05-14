#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

unsigned int num_cpus() 
{
	unsigned int num_cpus = 0;	
#ifndef linux    
	size_t len            = sizeof(num_cpus);
	int mib[4];
	mib[0]                = CTL_HW;
	mib[1]                = HW_NCPU;
	sysctl(mib, 2, &num_cpus, &len, NULL, 0);
#else
	num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
#endif
	if (num_cpus < 1) {
		printf("Error: Failed determine number of CPUs %s \n", strerror(errno));
	}

	return num_cpus;
}

int main(int argc, char *argv[]) 
{
	printf("Number of CPUs = %d\n", num_cpus()); // Should be same as $ cat /proc/cpuinfo
	return 0;
}
