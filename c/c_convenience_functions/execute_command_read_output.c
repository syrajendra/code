#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

static pthread_mutex_t g_tmp_mutex = PTHREAD_MUTEX_INITIALIZER;

#define TMP_DIR "/tmp"

int run_command(const char *cmd)
{
    int status;
    char *tmp = (char *) calloc(strlen(cmd)+50, sizeof(char));
    sprintf(tmp, "%s > /dev/null 2>&1", cmd);
    //printf("CMD: %s", tmp);
    if(-1 == (status=system(tmp))) {
        printf("Error: Failed to run command %s (%s) \n", tmp, strerror(errno));
        free(tmp);
        return -1;
    }
    if(WIFEXITED(status)) {
        if(WEXITSTATUS(status)) {
            printf("Error: Failed to run cmd [%s]\n", cmd);
            free(tmp);
            return -1;
        }
    }    
    free(tmp);
    return 1;
}

int create_temp_file(char *tmp_name)
{
    pthread_mutex_lock(&g_tmp_mutex);
    sprintf(tmp_name, "%s/%s", TMP_DIR, "delme_tmpfile.XXXXXX");

    int fd;
    if((fd=mkstemp(tmp_name)) && (fd==-1)) {
        printf("Error: Failed to run mkstemp: %s\n", strerror(errno));
        pthread_mutex_unlock(&g_tmp_mutex);
        exit(1);
    }    
    pthread_mutex_unlock(&g_tmp_mutex);
    return fd;
}

void remove_temp_file(int fd, char *tmp_name)
{
    pthread_mutex_lock(&g_tmp_mutex);
    
    if (close (fd) == -1) {
        printf("Error: Failed to close fd of tmp file = %s\n", tmp_name);
        exit(1);
    }

    if(-1 == remove(tmp_name)) {
        printf("Error: Failed to run remove: %s\n", strerror(errno));
        exit(1);
    }
    pthread_mutex_unlock(&g_tmp_mutex);
}

void twod_free_memory(char **result)
{
    unsigned int i, num_lines = atoi(result[0]);
    for(i=0; i<num_lines; i++) {
        free(result[i]);
    }
    free(result);
}

int copy_lines_to_2d_array(char *ptr, size_t size, char ***ret)
{
    off_t i, num_lines = 2; // two extra lines
    for(i=0; i<size; i++) if(ptr[i] == '\n') num_lines++;
    
    char **result = (char **) calloc(num_lines + 1, sizeof(char *));

    result[0]   = (char *) calloc(20, sizeof(char));
    sprintf(result[0], "%d", 0);
    off_t ncount= 1;
    int found   = 0;
    char *lstart=ptr, *lend;

    for(i=0; i<size; i++) {        
        if((ptr[i] == '\n')) {
            found = 1;
            lend = ptr + i + 1; //copy '\n' also
            off_t llength = lend - lstart;            
            if(llength > 0) {
                result[ncount] = (char *) calloc(llength+1, sizeof(char));
                memcpy(result[ncount], lstart, llength); // not null terminated
                result[ncount][llength] = '\0';
                lstart = lend;                
                ncount++;
            } else {
                printf("Error: Some problem no chars nfound\n");
                twod_free_memory(result);            
                return -5;
            }
        }
    }
    if(!found) {
        lend = ptr + i;
        off_t llength = lend - lstart;
        if(llength > 0) {
            result[ncount] = (char *) calloc(llength+1, sizeof(char));
            memcpy(result[ncount], lstart, llength); // not null terminated
            result[ncount][llength] = '\0';
        }
    }
   
    sprintf(result[0], "%lu", (long)ncount);
        
    *ret =  result;
    return 0;
}

int read_file(int fd, char ***result) 
{
    struct stat sb;
    int ret = 0;
    if (-1 == fstat (fd, &sb)) {
        printf("Error: Failed to fstat file fd=%d\n", fd);
        return -1;
    }

    if(sb.st_size == 0) {
        printf("Warning: No data to read from file (Bytes=%ld) ", sb.st_size);
        return -2;
    }

    if (!S_ISREG (sb.st_mode)) {
        printf("Error: fd=%d is not a file\n", fd);
        return -3; 
    }

    char *ptr = (char *)mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Error: Failed to mmap file \n");
        return -4;
    }

    ret = copy_lines_to_2d_array(ptr, sb.st_size, result);

    if (-1 == munmap (ptr, sb.st_size)) {
        printf("Error: Failed to munmap \n");
        return -5;
    }
    return ret;
}

int execute_command(char *cmd, int *status, char ***result)
{  
    int ret = 0;
    if(cmd) {
        char tmp_name[50];
        int fd = create_temp_file(tmp_name);

        int len = strlen(cmd) + strlen(tmp_name) + 20;
        char *tmp_cmd = (char *) calloc(len, sizeof(char));
        sprintf(tmp_cmd, "%s%s%s%s", cmd, " > ", tmp_name, " 2>&1");
        
        printf("CMD: %s\n", tmp_cmd);
        if((*status=system(tmp_cmd)) && (*status==-1)) {
            printf("Error: Failed to run system: %s\n", strerror(errno));
            close(fd);
            free(tmp_cmd);
            return -1;
        }

        free(tmp_cmd);

        ret = read_file(fd, result);

        remove_temp_file(fd, tmp_name);
    } else
        printf("Error: CMD is NULL\n");
    return ret;
}

int main(int argc, char *argv[]) 
{
    char **result;
    int status = 0;
    char *cmd = "ls -al";
    unsigned int i;
    int ret = 0;
    if((ret = execute_command(cmd, &status, &result)) == 0) {
        printf("Number of result Lines: %ld \n", atol(result[0]) - 1);
        for(i=1; i<atol(result[0]); i++) {
            printf("%s", result[i]);
        }
        twod_free_memory(result);
    } else {
        printf ("Error: Failed to execute command %d \n", ret);
    }
}
