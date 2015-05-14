#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
 #include <sys/mman.h>
#include <errno.h>

#define SPACE 	' '
#define NEWLINE	'\n' // new line
#define CARRIAGE_RETURN	'\r' // carriage return 

struct token_info {
	char **tokens;
	long length;
};
typedef struct token_info token_info_t;

static long micro_second_clock()
{
	struct timespec  ts;
	struct tm       *tm;
	long   ret; // micro seconds
 
	clock_gettime(CLOCK_REALTIME, &ts);
	tm = localtime(&ts.tv_sec);
 
	//printf("Clock: \t  %d:%02d:%02d:[%d nanosecons] (~%d microseconds)\n", tm->tm_hour,
	//       tm->tm_min, tm->tm_sec, ts.tv_nsec,
	//       tm->tm_hour * 3600 * 1000 * 1000 + tm->tm_min * 60 * 1000 * 1000 +
	//       tm->tm_sec * 1000 * 1000 + ts.tv_nsec / 1000);
 
	ret = tm->tm_hour * 3600 * 1000 * 1000 + tm->tm_min * 60 * 1000 * 1000 + 
		tm->tm_sec * 1000 * 1000 + ts.tv_nsec / 1000;
 
	return ret;
}

static void display_time(char *msg, long start, long stop)
{
	char precision[20];
  	long result = stop - start;
  	if (result > 1000000) {
    		result = result/1000000;
    		strcpy(precision, "");
  	} else if (result > 1000) {
    		result = result/1000;
    		strcpy(precision, "milli");
  	} else {
    		strcpy(precision, "micro");
  	}
  	printf("%s %lu %sseconds\n", msg, result, precision);
}

static int open_file(char *filename) 
{
	int fd = open(filename, O_RDONLY);
	if(fd == -1) {
		printf("Error: Failed to open file %s : %s\n", filename, strerror(errno));
		exit(1);
	}
	return fd;
}

static int is_token_separator(char ch) 
{
	return (ch == SPACE || ch == NEWLINE || ch == CARRIAGE_RETURN) ? 1 : 0;
}

static off_t count_spaces(char *addr, off_t length) 
{
	off_t i, count = 0;
	for(i=0; i<=length; i++) {		
		if(is_token_separator(addr[i])) { 
			count++;
		}
	}	
	return count;
}

static char *linguistic_preprocessing(char *src, unsigned int length)
{
	int i, j;
	char *dest = (char *) malloc(length+1);
	for(i=0, j=0; i<length; i++) {
		if(src[i] >= 'A' && src[i] <= 'Z') {
			dest[j++] = src[i] + 32; // change to lower case
		} else if(src[i] >= '0' && src[i] <= '9' || src[i] >= 'a' && src[i] <= 'z') {
			dest[j++] = src[i]; // copy as is
		}
	}
	dest[j] = '\0';
	//printf("src: %.*s dest: %s\n", length, src, dest);
	if(strlen(dest)) return dest;
	free(dest);
	return NULL;
}


static token_info_t *tokenize_document_text(char *filename) 
{
	struct stat sb;
	token_info_t *tinfo = NULL;
	int fd = open_file(filename);
	if (-1 == fstat(fd, &sb)) {
		printf("Error: Failed to stat file %s\n", strerror(errno));
		exit(1);
	}	
	if(sb.st_size) {
		char *addr = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (addr == MAP_FAILED) {
       	 		printf("Error: Failed to mmap file \n");
       	 		exit(1);
       	 	}
       	 	tinfo = (token_info_t *) calloc(sizeof(token_info_t), 1);
       	 	int start = 0, length, i;
       	 	tinfo->length = 0;
       	 	off_t tokens_count = count_spaces(addr, sb.st_size);
       	 	tinfo->tokens = (char **) calloc(sizeof(char *), tokens_count);
       	 	printf("Number of tokens in file [%s]: %lu \n", filename, tokens_count);

		for(i=0; i<=sb.st_size; i++) {
			if(is_token_separator(addr[i])) {
				 tinfo->tokens[tinfo->length] = linguistic_preprocessing(addr+start, i - start);
				 if(tinfo->tokens[tinfo->length]) tinfo->length++;
				 start = i + 1;

			}
		}
		if(-1 == munmap(addr, sb.st_size)) {
			printf("Error: Failed to munmap %s\n", strerror(errno));
			exit(1);
		}
	} else {
		printf("Error: Input file is empty !!!\n");
		exit(1);
	}
	close(fd);
	return tinfo;
}

static int cmpstringp(const void *p1, const void *p2) // code from man page of qsort
{
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}

static void insert_term(char **array, unsigned int length, unsigned int pos, char *term) 
{
	int i;
	char *tmp;
	//printf("Add [%s] @ position [%d]\n", term, pos);
	for(i=length; i>pos; i--) 
		array[i] = array[i-1];
	array[pos] =  term;
}

static display_terms(char *msg, char **terms, int ucount)
{
	int i;	
	printf("\n%s Terms: %d\n", msg, ucount);
	printf("-------------\n");
	for(i=0; i<ucount; i++)	
		printf("%s\n", terms[i]);
}

static unsigned int unique_sort_array(char **input_tokens, unsigned int input_length, char **unique_terms, unsigned int unique_length)
{
	int i, j;
	for(i=0; i<input_length; i++) {			
		char *new_term = input_tokens[i];		
		int flag = 1;
		for(j=0; j<unique_length; j++) {
			char *cur_term = unique_terms[j];
			flag = strcmp(new_term, cur_term);			
			if(!flag) break; // if same skip
			if(flag < 0) { // new term is smaller than current term 
				if((j-1) >= 0) { // if exist previous token
					char *prev_term = unique_terms[j-1];					 
					if(!(strcmp(prev_term, new_term) > 0)) continue; // previous term should be less than new term					
				}				
				break;				
			} else if(flag > 0) { // new term is greater than current term 
				if((j+1) < unique_length) { // if exist next term
					char *next_term = unique_terms[j+1];					
					if(!(strcmp(new_term, next_term) < 0)) continue; // next term should be greater than new term					
				}
				j++;
				break; 				
			}
		}
		if(!flag) continue;
		insert_term(unique_terms, unique_length, j, new_term);
		unique_length++;
	}
	return unique_length;
}

static void generate_unique_terms(token_info_t **tinfo, unsigned int tlength)
{	
	int i;
	int ucount = 0;
	int tmem = 0;
	for(i=0; i<tlength; i++) tmem += tinfo[i]->length;
	char **unique_terms = (char **) calloc(sizeof(char *), tmem);
#if 0  	
	// non unique sorting
	int j;	
	for(i=0; i<tlength; i++) 	
		for(j=0; j<tinfo[i]->length; j++)
			unique_terms[ucount++] =  tinfo[i]->tokens[j];
	qsort(unique_terms, tmem, sizeof(char *), cmpstringp);
	
#else
	// Unique elements sorting
	for(i=0; i<tlength; i++) 	
		ucount = unique_sort_array(tinfo[i]->tokens, tinfo[i]->length, unique_terms, ucount);	
#endif
	display_terms("Unique", unique_terms, ucount);
	free(unique_terms);	
}	

static int free_memory(token_info_t **tinfo, int length) 
{
	int i, j;	
	for(i=0; i<length; i++) {
		for(j=0; j<tinfo[i]->length; j++)			
			free(tinfo[i]->tokens[j]);
		free(tinfo[i]->tokens);
		free(tinfo[i]);	
	}
	free(tinfo);	
}

int main(int argc, char *argv[]) 
{	
	int i;
	int ret = 0;
	long start = micro_second_clock();
	if(argc > 1) {
		token_info_t **tinfo = (token_info_t **) calloc(sizeof(token_info_t *), argc-1);
		for(i=1; i<argc; i++) {
			tinfo[i-1] = tokenize_document_text(argv[i]);
		}		
		generate_unique_terms(tinfo, argc-1);
		free_memory(tinfo, argc-1);
		display_time("\nTime taken to complete execution", start, micro_second_clock());
	} else {
		printf("Error: Supply filenames as arguments\n");
		printf("Usage: %s filename1 filename2 . . .\n", argv[0]);
		ret = 1;
	}	
	return ret;
}
