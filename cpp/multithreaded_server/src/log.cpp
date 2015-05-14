#include <common.hpp>
#include <stdarg.h>
#include <stdint.h>
#include <cstdio>
#include <ctime>
using namespace std;
FILE *g_stream;
uint64_t log_size = 0;

static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
#define HUNDREAD_MB  	104857600 // 100MB trace file
//#define HUNDREAD_MB 1048576 // 1MB for debug
#define PTRACE_FILE 	"./logs/trace_query-loc.log"

void gettime(char *buf, int size)
{
	std::time_t rawtime;
	std::tm* timeinfo;
	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);
	std::strftime(buf, size,"%Y-%m-%d %I:%M:%S %p",timeinfo);
}

void log_message(char type, FILE *stream, char const *func,
				 const unsigned int line, char const *fmt, ...)
{
	va_list ap;
	pthread_mutex_lock(&print_mutex);
	int size = 0;
	if(stream) {
		 char buf[80];
		 gettime(buf, 80);
		 size += fprintf(stream, "%s : ", buf);
		if(stream != stderr && stream != stdout) {
			if(type == 'e')
				size += fprintf(stream, "%s(%d) : ERROR: ", func, line);
			else if (type == 'p')
				size += fprintf(stream, "%s(%d) : CONSOLE: ", func, line);
			else
				size += fprintf(stream, "%s(%d) : ", func, line);
		} else {
			if(type == 'e') size += fprintf(stream, "ERROR: ");
		}
		va_start(ap, fmt);
		size += vfprintf(stream, fmt, ap);
		va_end(ap);
		if(type != 'p') size += fprintf(stream, "\n");
		fflush(stream);
	}
	if(g_stream == stream) {
		log_size += size;
	}
	pthread_mutex_unlock(&print_mutex);
}

void create_log_file()
{
	if (g_stream) fclose(g_stream);
	if(NULL == (g_stream = fopen(PTRACE_FILE, "w"))) {
		ERROR("Failed to create trace file at %s\n", PTRACE_FILE);
	}
}

void recreate_log_file()
{
	pthread_mutex_lock(&print_mutex);
	if ( log_size > HUNDREAD_MB) {
		 fseek(g_stream, 0, SEEK_SET);
	}
	pthread_mutex_unlock(&print_mutex);
}


