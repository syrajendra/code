#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define READ                0
#define WRITE               1
#define ERROR               2

FILE *g_stream = NULL;

#ifdef DEBUG
void log_file_lock()
{
    int fd =  fileno(g_stream);           
    if (0 != flock(fd, LOCK_EX)) {
            printf("Error: Failed to lock log file\n");
            exit(1);
    }
}

void log_file_unlock()
{
    int fd =  fileno(g_stream);   
    if (0 != flock(fd, LOCK_UN)) {
            printf("Error: Failed to unlock log file\n");
            exit(1);
    }
}

void my_lock_trace(FILE *stream, const char *func, const unsigned int line, char *fmt, ...)
{
    log_file_lock();    
    fprintf(g_stream, "%s(%d):", func, line); 
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_stream, fmt, ap);
    va_end(ap);
    fprintf(g_stream, "\n");
    fflush(g_stream);
    log_file_unlock();    
}

#define FLOCK_TRACE(format, ...) my_lock_trace(g_stream, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#else
#define FLOCK_TRACE(format, ...) (void)0
#endif

void child_process(int fd_stdin[], int fd_stdout[], char *program, char *argv[], char *envp[]) 
{
    close(fd_stdin[WRITE]);
    close(fd_stdout[READ]);

    if(-1 == dup2(fd_stdin[READ], READ)) {
        FLOCK_TRACE("Failed to redirect stdin \n");
        return;
    }

    if(-1 == dup2(fd_stdout[WRITE], WRITE)) {
        FLOCK_TRACE("Failed to redirect stdout \n");
        return;
    } 

    if(-1 == dup2(fd_stdout[WRITE], ERROR)) {
        FLOCK_TRACE("Failed to redirect stderror \n");
        return;
    }

    FILE *stdout_stream = fdopen(fd_stdout[WRITE], "w");
    setvbuf(stdout_stream, (char *) NULL, _IONBF, 0);
    //char *msg = "Child push message";
    //if(-1 == write(fd_stdout[WRITE], msg, strlen(msg))) 
    //    FLOCK_TRACE("Error: Failed to write %s child process: %s\n", msg, strerror(errno));
    //else
    //    FLOCK_TRACE("Successfully wrote <%s> to stdout\n", msg);                        
    
    FLOCK_TRACE("execve called");
    int ret = execve(program, argv, envp);
    if (-1 == ret) {
        FLOCK_TRACE("Error: Failed to execute program [%s]\n", strerror(errno));
        exit(1);
    }
    FLOCK_TRACE("Error: If exec returns i.e. error has occurred\n");
}

int is_process_alive(pid_t pid)
{
    struct stat sb;
    char pathname[512];
    sprintf(pathname, "/proc/%d", pid);
    //FLOCK_TRACE("Process path: %s\n", pathname);
    if ((stat(pathname, &sb) == 0) && S_ISDIR(sb.st_mode)) return 1;
    return 0;
}

void parent_process(pid_t pid, int fd_stdin[], int fd_stdout[], int input_count, char *input[], char **output)
{
    close(fd_stdin[READ]);
    close(fd_stdout[WRITE]);

    char tmp[1024];
    char *buff  = (char *) malloc(1);
    buff[0]     = '\0';
    fd_set readfd;
    struct timeval tv;
    tv.tv_sec  = 1;
    tv.tv_usec = 0;
    int ret,  status, i, success = 0;    
    FLOCK_TRACE("Input count : %d", input_count);
    while(1) {
        FD_ZERO(&readfd);
        FD_SET(fd_stdout[READ], &readfd);
        ret = select(fd_stdout[READ]+1, &readfd, NULL, NULL, &tv);
        if(-1 == ret) {
            FLOCK_TRACE("Error: Failed select call: %s\n", strerror(errno));
            continue;
        } else  if(!ret) { // timeout            
            FLOCK_TRACE("Timedout Buff %s\n", buff);
            if(input_count > 0 && is_process_alive(pid) && !success) {
                char w_stdin[1024];  
                memset(w_stdin, '\0', 1024);
                for(i=0; i<input_count; i++) {
                    strcat(w_stdin, input[i]);
                    strcat(w_stdin, " ");
                }
                strcat(w_stdin, "\n");
                if(-1 == write(fd_stdin[WRITE], w_stdin, strlen(w_stdin))) 
                    FLOCK_TRACE("Error: Failed to write %s child process: %s\n", w_stdin, strerror(errno));                    
                else {
                    FLOCK_TRACE("Writing to child process = %s", w_stdin);                    
                    success = 1;
                }
            }      
        } else {
            FLOCK_TRACE("Select Buff %s\n", buff);
            if(FD_ISSET(fd_stdout[READ], &readfd)) {
                int len = read(fd_stdout[READ], tmp, 1023);
                if(len > 0) {
                    tmp[len] = '\0';
                    size_t old_len = strlen(buff);
                    buff = (char *) realloc(buff, old_len+len+1); 
                    if(buff) {
                        memcpy(buff+old_len, tmp, len);
                        buff[old_len+len] = '\0';
                    } else {
                        FLOCK_TRACE("Error: Failed to reallocate memory \n");
                        exit(1);
                    }
                } 
            }
        }
        waitpid(pid, &status, WNOHANG);
        if(!is_process_alive(pid)) break;
    } // end of while
    *output = buff;
}

void execute_program(char *program, char *argv[], char *envp[], int input_count, char *input[], char **output)
{
    int fd_stdin[2];
    int fd_stdout[2];

    if(-1 == pipe(fd_stdout)) {
        FLOCK_TRACE("Error: Failed to create pipe for stdout \n");
        exit(1);
    }
    
    if(-1 == pipe(fd_stdin)) {
        FLOCK_TRACE("Error: Failed to create pipe for stdin\n");
        exit(1);
    }
   
    pid_t  pid = fork();
    if(pid== -1) {        
        FLOCK_TRACE("Error: Failed to create a process\n");        
        exit(1);
    } else if(pid == 0) { //Child process gets pid as zero
        FLOCK_TRACE("Child process created");        
        child_process(fd_stdin, fd_stdout, program, argv, envp);        
    } else if(pid > 0) {  // parent process gets child_pid
        FLOCK_TRACE("Parent process");
        parent_process(pid, fd_stdin, fd_stdout, input_count, input, output);
    }
}

int main(int argc, char *argv[], char *envp[])
{
    time_t t;
    time(&t);
#ifdef DEBUG    
    g_stream = fopen("exec_process.log", "w");
    setvbuf(g_stream, (char *) NULL, _IONBF, 0);
#endif
    printf("Welcome  - %s \n", ctime(&t));    
        
    char *args[3] = {"./add", "Rajendra", NULL};      
    char *input[3]= {"20", "30"};    
    char *output = NULL;    
    execute_program(args[0], args, envp, 2, input, &output);
    
    /*
    char *args[4]= {"/bin/ls", "-al", NULL};        
    char *output = NULL;    
    execute_program(args[0], args, envp, 0, NULL, &output);
    */
    
    printf("Executed program %s\nOutput:\n%s", args[0], output);

    free(output);
#ifdef DEBUG     
    fclose(g_stream);
#endif
    return 0;
}

/* Add program
   File: add.c
   Build: gcc -o add add.c
 */
/*
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int a, b;
		printf("Add program by %s\n", argv[1]);
		printf("Enter two numbers: ");
		scanf("%d %d", &a, &b);
		int c = a + b;
		printf("%d + %d = %d\n", a, b, c);
		printf("Bye...\n");
	} else {
		printf("Error: Supply argument\n");
		exit(1);
	}
	return 0;
}
*/
