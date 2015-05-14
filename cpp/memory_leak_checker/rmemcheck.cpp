#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <list>

namespace std {} using namespace std;

#define CHECK_DISCARD 
#include "rmemcheck.h"
#undef CHECK_DISCARD

static unsigned int mem_count = 0;
static FILE *g_output = stderr; // default
static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

struct  malloc_info {
	void		*ptr;
	char		*file;
	int		line;
	size_t		size;
};
typedef struct malloc_info malloc_info_t;

typedef list<malloc_info_t *> malloc_list;      

malloc_list *mlist = 0x0;

inline void my_printf(const char *format, ...)
{
	pthread_mutex_lock(&print_mutex);
	va_list ap;
	va_start(ap, format);
	vfprintf(g_output, format, ap);
	va_end(ap);
	pthread_mutex_unlock(&print_mutex);
}

string int_to_string(unsigned int number)
{
	char buf[16];
	sprintf(buf, "%d", number);
	return  (string) buf;    
}

void invalid_free(void *ptr, const char *file, int line)
{
	mem_count++;
	string str_count = int_to_string(mem_count);
	string str_file  = (string)   file;
	string str_line  = int_to_string(line); 
	string temp =  "Problem - " + str_count +  ": Invalid memory free  @ " + str_file + ":"  + str_line;	
	my_printf("%s", temp.c_str());	
}

void *r_mem_check_malloc(size_t size, const char *file, int line)
{		
	void *ptr = malloc(size);
	add_info(ptr, size, file, line);
	return ptr;
}

void  r_mem_check_free(void *ptr, const char *file, int line)
{		
	if(!remove_info(ptr)) 
		invalid_free(ptr, file, line);
	else
		free(ptr);
}

void *r_mem_check_calloc(size_t num, size_t size, const char *file, int line)
{	
	void *ptr = calloc(num, size);
	add_info(ptr, num*size, file, line);
	return ptr;
}

void *r_mem_check_realloc(void *ptr, size_t size, const char *file, int line)
{	
	if(!remove_info(ptr)) printf("Failed to find realloc ptr\n");
	ptr = realloc(ptr, size);	
	add_info(ptr, size, file, line);
	return ptr;
}

char *r_mem_check_strdup(const char *str, const char *file, int line)
{	
	char *ptr = strdup(str);
	add_info(ptr, strlen(str), file, line);
	return ptr;
}

char *r_mem_check_strndup(const char *str, size_t size, const char *file, int line) 
{
	char *ptr = strndup(str, size);
	add_info(ptr, strlen(str), file, line);
	return ptr;
}

void dump_info()
{
	unsigned int tot_mem_leak = 0;
	string temp;	
	malloc_list::iterator i;	
	if(mlist && !mlist->empty()) {
		for(i = mlist->begin(); i != mlist->end(); i++) {
			mem_count++;
			string str_count = int_to_string(mem_count);
			string str_file  = (string)   (*i)->file;
			tot_mem_leak    += (unsigned int)(*i)->size;
			string str_size  = int_to_string((unsigned int)(*i)->size);
			string str_line  = int_to_string((*i)->line); 
			temp = "Problem - " + str_count +  ": Memory leak ["  + str_size + " bytes] @ " + str_file + ":"  + str_line;	
			my_printf("%s\n", temp.c_str());
		}		
		my_printf("\nSummary : \n \tAt %d place(s) Memory Leak(s) Detected\n",mem_count);
		my_printf("\tTotally [%d bytes] of Memory Leak\n",tot_mem_leak);
	} else {
		my_printf("No Memory Leak Detected\n"); 
	}
	my_printf("\n");
}

void int_signal_catch(int signum)
{	
	if (signum == SIGUSR1)	dump_info();	
}

void exiting(void)
{		
	dump_info();	
}

inline void init()
{
	if(!mlist) {
		mlist = new (malloc_list);
		signal(SIGUSR1, &int_signal_catch);
		atexit(exiting);	
	}
}

void display_list()
{
	malloc_list::iterator i;
	for(i = mlist->begin(); i != mlist->end(); i++) {		
		my_printf("<%p> %s:%d\n", (*i)->ptr, (*i)->file, (*i)->line);
	}
}

void add_info(void *ptr, size_t size, const char *file, int line)
{
	init();	
	if(mlist) {
		malloc_info_t *info = (malloc_info_t *) malloc (sizeof(malloc_info_t));
		info->file = strdup(file);
		info->line = line;
		info->ptr  = ptr;
		info->size = size;
		mlist->insert(mlist->begin(), info);
	}
	//display_list();
}

int remove_info(void *ptr)
{	
	if(mlist) {	
		malloc_list::iterator i;
		for(i = mlist->begin(); i != mlist->end(); i++) {
			//my_printf("<%p> = <%p>\n", (*i)->ptr, ptr);			
			if((*i)->ptr == ptr) {
				free((*i)->file);				
				mlist->remove((*i));
				free(*i);
				return 1;
			}
		}
	}
	return 0;
}

void set_log_file(const char *filename)
{
	g_output = fopen(filename, "w+");
	if (!g_output) {
		fprintf(stderr, "Error: Failed to create log file (%s)\n", strerror(errno));
		exit(1);
	}	
}
