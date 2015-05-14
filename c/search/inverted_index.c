#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>

#define SPACE 	' '
#define NEWLINE	'\n' // new line
#define CARRIAGE_RETURN	'\r' // carriage return 

struct doc_info {
	char 	*doc_name;
	off_t   doc_id;
	char 	**tokens;
	off_t 	length;
};
typedef struct doc_info doc_info_t;

struct posting_list {
	off_t	doc_id;
	struct 	posting_list *next;
};
typedef struct posting_list posting_list_t;

struct inverted_index {
	char 	*token;
	off_t 	frequency;
	posting_list_t *plist;
};
typedef struct inverted_index inverted_index_t;

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
	return (ch == SPACE || ch == NEWLINE || ch == CARRIAGE_RETURN || ch == '\0') ? 1 : 0;
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


static doc_info_t *tokenize_document_text(off_t id, char *filename) 
{
	struct stat sb;
	doc_info_t *dinfo = NULL;
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
       	 	dinfo = (doc_info_t *) calloc(sizeof(doc_info_t), 1);
       	 	dinfo->doc_name = strdup(filename);
		dinfo->doc_id 	= id;
       	 	int start = 0, length, i;
       	 	dinfo->length = 0;
       	 	off_t tokens_count = count_spaces(addr, sb.st_size);
       	 	dinfo->tokens = (char **) calloc(sizeof(char *), tokens_count);
       	 	printf("Number of tokens in file [%s - %lu]: %lu \n", filename, id, tokens_count);

		for(i=0; i<=sb.st_size; i++) {
			if(is_token_separator(addr[i])) {
				 dinfo->tokens[dinfo->length] = linguistic_preprocessing(addr+start, i - start);
				 if(dinfo->tokens[dinfo->length]) dinfo->length++;
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
	return dinfo;
}

static int cmpstringp(const void *p1, const void *p2) // code from man page of qsort
{
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}

static display_terms(char *msg, char **terms, int ucount)
{
	int i;	
	printf("\n%s Terms: %d\n", msg, ucount);
	printf("-------------\n");
	for(i=0; i<ucount; i++)	
		printf("%s\n", terms[i]);
}

static posting_list_t *get_new_posting(off_t doc_id)
{
	posting_list_t *tmp = (posting_list_t *) calloc(sizeof(posting_list_t), 1);
	tmp->doc_id = doc_id;
	tmp->next = NULL;
	return tmp;
}

static void update_postings_list(off_t doc_id, posting_list_t **head)
{
	posting_list_t *ptr = *head, *prev;	
	if(ptr) {
		do {			
			if(ptr->doc_id == doc_id) return;
			prev = ptr;
			ptr = ptr->next;
		} while(ptr);
		prev->next = get_new_posting(doc_id);
	} else *head = get_new_posting(doc_id);
}

static void insert_term(off_t doc_id, inverted_index_t **array, unsigned int length, unsigned int pos, char *term) 
{
	int i;
	char *tmp;
	//printf("Add [%s] @ position [%d]\n", term, pos);
	for(i=length; i>pos; i--) 
		array[i] = array[i-1];
	array[pos] = (inverted_index_t *) calloc(sizeof(inverted_index_t), 1);
	array[pos]->token =  term;
	array[pos]->frequency = 1;
	array[pos]->plist = NULL;
	update_postings_list(doc_id, &array[pos]->plist);
}

static unsigned int unique_sort_array(off_t doc_id, char **input_tokens, unsigned int input_length, inverted_index_t **iindex, unsigned int iicount)
{
	int i, j;
	for(i=0; i<input_length; i++) {			
		char *new_term = input_tokens[i];		
		int flag = 1;
		for(j=0; j<iicount; j++) {
			char *cur_term = iindex[j]->token;
			flag = strcmp(new_term, cur_term);			
			if(!flag) break; // if same skip
			if(flag < 0) { // new term is smaller than current term 
				if((j-1) >= 0) { // if exist previous token
					char *prev_term = iindex[j-1]->token;					 
					if(!(strcmp(prev_term, new_term) > 0)) continue; // previous term should be less than new term					
				}				
				break;				
			} else if(flag > 0) { // new term is greater than current term 
				if((j+1) < iicount) { // if exist next term
					char *next_term = iindex[j+1]->token;					
					if(!(strcmp(new_term, next_term) < 0)) continue; // next term should be greater than new term					
				}
				j++;
				break; 				
			}
		}
		if(!flag) {
			update_postings_list(doc_id, &iindex[j]->plist);
			iindex[j]->frequency++;
			continue;
		}
		insert_term(doc_id, iindex, iicount, j, new_term);
		iicount++;
	}
	return iicount;
}

static display_inverted_index(inverted_index_t **iindex, off_t iicount) 
{
	off_t i;
	for(i=0; i<iicount; i++) {
		printf("[%s][%lu] ", iindex[i]->token, iindex[i]->frequency);
		posting_list_t *ptr = iindex[i]->plist;
		while(ptr) {
			printf(" ---> [%lu]", ptr->doc_id);
			ptr = ptr->next;
		}
		printf("\n");
	}
}

static free_inv_memory(inverted_index_t **iindex, off_t iicount)
{
	off_t i;
	for(i=0; i<iicount; i++) {
		posting_list_t *curr = iindex[i]->plist;
		while(curr) {
			posting_list_t *next = curr->next;
			free(curr);
			curr = next;
		}		
		free(iindex[i]);
	}
	free(iindex);	
}

static void generate_inverted_index(doc_info_t **dinfo, off_t tlength)
{
	off_t i;
	off_t iicount = 0;
	off_t tmem = 0;
	for(i=0; i<tlength; i++) tmem += dinfo[i]->length;
	inverted_index_t **iindex = (inverted_index_t **) calloc(sizeof(inverted_index_t *), tmem);

	// Unique elements sorting
	for(i=0; i<tlength; i++) 	
		iicount = unique_sort_array(dinfo[i]->doc_id, dinfo[i]->tokens, dinfo[i]->length, iindex, iicount);

	display_inverted_index(iindex, iicount);
	free_inv_memory(iindex, iicount);
	
}	

static int free_doc_memory(doc_info_t **dinfo, off_t length) 
{
	off_t i, j;	
	for(i=0; i<length; i++) {
		for(j=0; j<dinfo[i]->length; j++)			
			free(dinfo[i]->tokens[j]);
		free(dinfo[i]->tokens);
		free(dinfo[i]->doc_name);
		free(dinfo[i]);	
	}
	free(dinfo);	
}

int main(int argc, char *argv[]) 
{	
	int i;
	int ret = 0;
	long start = micro_second_clock();
	if(argc > 1) {
		doc_info_t **dinfo = (doc_info_t **) calloc(sizeof(doc_info_t *), argc-1);
		for(i=1; i<argc; i++) {			 
			dinfo[i-1] = tokenize_document_text(i, argv[i]);
		}		
		generate_inverted_index(dinfo, argc-1);
		free_doc_memory(dinfo, argc-1);
		display_time("\nTime taken to complete execution", start, micro_second_clock());
	} else {
		printf("Error: Supply filenames as arguments\n");
		printf("Usage: %s filename1 filename2 . . .\n", argv[0]);
		ret = 1;
	}	
	return ret;
}
