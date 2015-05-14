#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int is_process_alive(pid_t pid)
{
    struct stat sb;
    char pathname[100];
    sprintf(pathname, "/proc/%d", pid);
    //PRINTF("pathname: %s\n", pathname);
    if ((stat(pathname, &sb) == 0) && S_ISDIR(sb.st_mode)) return 1;
    return 0;
}

int main(int argc, char *argv[]) 
{	
	printf("%s\n", (is_process_alive((pid_t) atoi(argv[1]))) ? "Alive" : "Dead");
	return 0;
}
