#ifndef _RMEMCHECK_H_
#define _RMEMCHECK_H_

void *r_mem_check_malloc	(size_t size,		 	const char *file, int line);
void  r_mem_check_free		(void *ptr,		 	const char *file, int line);
void *r_mem_check_calloc	(size_t num, size_t size,	const char *file, int line);
void *r_mem_check_realloc	(void *ptr, size_t size, 	const char *file, int line);

char *r_mem_check_strdup	(const char *str,	 	const char *file, int line);
char *r_mem_check_strndup 	(const char *s, size_t size,	const char *file, int line);

int   remove_info(void *ptr);
void  add_info(void *ptr, size_t size, const char *file, int line);
void  dump_info();


#ifndef CHECK_DISCARD
#define malloc(size      )	r_mem_check_malloc	(size,		__FILE__, __LINE__)
#define free(ptr         )	r_mem_check_free	(ptr,		__FILE__, __LINE__)
#define calloc(num,size  )	r_mem_check_calloc	(num, size, 	__FILE__, __LINE__)
#define realloc(ptr,size )	r_mem_check_realloc	(ptr, size, 	__FILE__, __LINE__)
#define strdup(str	 )	r_mem_check_strdup	(str,		__FILE__, __LINE__)
#endif
void set_log_file(const char *filename);
#endif
