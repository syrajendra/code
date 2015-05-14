
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

int open_file_descriptor_limit(int fd_limit)
{
	struct rlimit rlim;
	if(-1 == getrlimit(RLIMIT_NOFILE, &rlim)) {
		printf("Error: Failed to run getrlimit (%s) \n", strerror(errno));
	    	return -1;
	}
	printf("Default file descriptor limit soft : %d hard : %d\n", (int)rlim.rlim_cur, (int)rlim.rlim_max);
	if(fd_limit > 0) {
		if(rlim.rlim_cur < fd_limit || rlim.rlim_max < fd_limit) {
			rlim.rlim_cur = rlim.rlim_max = fd_limit;
			if (-1 == setrlimit(RLIMIT_NOFILE, &rlim)) {
				printf("Error: Failed to run setrlimit (%s) for %d, Try as sudo\n", strerror(errno), fd_limit);
		      		return -1;
			}
			printf("Increased file descriptor limit to soft & hard : %d \n", fd_limit);
		    	return 0;
		} else {
			printf("Default file descriptor limit is greater than %d\n", fd_limit);
			return 0;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int i;	
	if(argc > 1) {
		if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			printf("Usage: %s <fd_limit> <command>\n", argv[0]);
			return 0;
		} else {
			int fd_limit = atoi(argv[1]);
			if(!open_file_descriptor_limit(fd_limit)) {
				if(argc > 2) {
					char *cmd = NULL;
					int length= 1; // for '\0'
					for(i=2; i<argc; i++) length += strlen(argv[i]) + 1; // 1 space extra
					cmd   	= (char *) calloc(length, sizeof(char));					
					for(i=2; i<argc; i++) {
						strcat(cmd, argv[i]);
						strcat(cmd, " ");
					}
					printf("Running Command: %s\n", cmd);
					int ret = system(cmd);
					free(cmd);
					return ret;
				}
			}
		}
	} else {
		open_file_descriptor_limit(-1);	
	}
	return 0;
}
