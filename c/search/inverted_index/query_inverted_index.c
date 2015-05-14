#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
 #include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>

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
	off_t	plist_length;
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

static off_t count_terms(char *addr, off_t length) 
{
	off_t i, count = 0;	
	for(i=0; i<=length; i++) {		
		if(is_token_separator(addr[i])) { 			
			count++;
		}			
	}	
	return count;
}

static char *allocate_token(char *src, unsigned int length)
{
	char *dest = (char *) malloc(length+1);
	strncpy(dest, src, length);
	dest[length] = '\0';
	if(strlen(dest)) return dest;
	free(dest);
	return NULL;
}

static char *linguistic_preprocessing(char *src, unsigned int length)
{
	int i, j;	
	for(i=0, j=0; i<length; i++) {
		if(src[i] >= 'A' && src[i] <= 'Z') {
			src[j++] = src[i] + 32; // change to lower case
		} else if((src[i] >= '0' && src[i] <= '9') || (src[i] >= 'a' && src[i] <= 'z')) {
			j++; // do not change 
		}
	}	
	if(src) src[j] = '\0';
	//printf("src: %.*s dest: %s\n", length, src, dest);
	return src;
	
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
       	 	int start = 0, i;
       	 	dinfo->length = 0;
       	 	off_t tokens_count = count_terms(addr, sb.st_size);
       	 	dinfo->tokens = (char **) calloc(sizeof(char *), tokens_count);
       	 	printf("Number of tokens in file [%s - %lu]: %lu \n", filename, id, tokens_count);

		for(i=0; i<=sb.st_size; i++) {
			if(is_token_separator(addr[i])) {
				 dinfo->tokens[dinfo->length] = linguistic_preprocessing(allocate_token(addr+start, i-start), i-start);
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
/*
static void display_terms(char *msg, char **terms, int ucount)
{
	int i;	
	printf("\n%s Terms: %d\n", msg, ucount);
	printf("-------------\n");
	for(i=0; i<ucount; i++)	
		printf("%s\n", terms[i]);
}
*/
static posting_list_t *get_new_posting(off_t doc_id)
{
	posting_list_t *tmp = (posting_list_t *) calloc(sizeof(posting_list_t), 1);
	tmp->doc_id = doc_id;
	tmp->next = NULL;
	return tmp;
}

static int update_postings_list(off_t doc_id, posting_list_t **head)
{
	posting_list_t *ptr = *head, *prev;	
	if(ptr) {
		do {			
			if(ptr->doc_id == doc_id) return 0;
			prev = ptr;
			ptr = ptr->next;
		} while(ptr);
		prev->next = get_new_posting(doc_id);		
	} else *head = get_new_posting(doc_id);
	return 1;
}

static void insert_term(off_t doc_id, inverted_index_t **array, unsigned int length, unsigned int pos, char *term) 
{
	int i;	
	//printf("Add [%s] @ position [%d]\n", term, pos);
	for(i=length; i>pos; i--) 
		array[i] = array[i-1];
	array[pos] = (inverted_index_t *) calloc(sizeof(inverted_index_t), 1);
	array[pos]->token =  term;
	array[pos]->frequency = 1;
	array[pos]->plist_length = 0;
	array[pos]->plist = NULL;
	if(update_postings_list(doc_id, &array[pos]->plist)) array[pos]->plist_length++;
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
			if(update_postings_list(doc_id, &iindex[j]->plist)) iindex[j]->plist_length++;
			iindex[j]->frequency++;
			continue;
		}
		insert_term(doc_id, iindex, iicount, j, new_term);
		iicount++;
	}
	return iicount;
}

static void display_inverted_index(inverted_index_t **iindex, off_t iicount) 
{
	off_t i;
	printf("\nInverted index: \n");
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

static void free_inv_memory(inverted_index_t **iindex, off_t iicount)
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


posting_list_t *intersect_postings_merge_algorithm(posting_list_t *p1, posting_list_t *p2)
{
	posting_list_t *answer = NULL;
	while(p1 && p2) {
		if(p1->doc_id == p2->doc_id) {
			if(!answer) answer = get_new_posting(p1->doc_id);
			else {
				while(answer->next) answer = answer->next;
				answer->next = get_new_posting(p1->doc_id);
			}
			p1 = p1->next;
			p2 = p2->next;	
		} else {
			if(p1->doc_id < p2->doc_id) 
				p1 = p1->next;
			else
				p2 = p2->next;
		}
	}
	return answer;
}

static void display_postings(char *msg, posting_list_t *ptr)
{
	printf("%s: Posting list: [", msg);
	while(ptr) {
		printf(" %lu ", ptr->doc_id);
		ptr = ptr->next;
	}
	printf("]\n");
	
}

static void free_postings_memory(posting_list_t *ptr) {
	posting_list_t *next = NULL;
	while(ptr) {
		next = ptr->next;
		free(ptr);
		ptr  = next;
	}
}

posting_list_t *find_postings_array(inverted_index_t **iindex, off_t iicount, char *tk, off_t *plength)
{
	int i;
	for(i=0; i<iicount; i++) {
		if(!strcmp(iindex[i]->token, tk)) {
			*plength = iindex[i]->plist_length;
			return iindex[i]->plist;
		}
	}
	return NULL;
}

static void sort_by_post_list_length(int nt, posting_list_t **parray, off_t plength[], char **tks)
{
	int i, j;
	// insertion sort good for small arrays	
	//FIXME: Change this implementation to Quick sort
	//for(i=0;i<nt; i++) printf(" %lu ", plength[i]);
	//printf("\n");
	for(i=1; i<nt; i++) { 
		off_t tmpf = plength[i];
		posting_list_t *tmpp = parray[i];
		char *tmpt = tks[i];
		for(j=i-1; (tmpf<plength[j]) && (j>=0); j--) {
			plength[j+1]  = plength[j];
			parray[j+1] = parray[j];
			tks[j+1]    = tks[j];
		}
		plength[j+1]  = tmpf;
		parray[j+1] = tmpp;
		tks[j+1]    = tmpt;
	}
	//for(i=0;i<nt; i++) printf(" %lu ", plength[i]);
	//printf("\n");
}

static posting_list_t *execute_N_token_query(inverted_index_t **iindex, off_t iicount, char *operation, int num, ...)
{
	int i;
	printf("\nQuery: ");
	posting_list_t *result = NULL;
	if(num<=0) return NULL;	
	char **tks = (char **) calloc(sizeof(char*), num);
	posting_list_t **parray = (posting_list_t **) calloc(sizeof(posting_list_t **), num);
	off_t *plength = (off_t *) calloc(sizeof(off_t), num);
	va_list ap;
	va_start(ap, num);
	for(i=0; i<num; i++) {
		tks[i] = va_arg(ap, char *);
		printf(" %s ", tks[i]);
		if(i+1 != num) printf(" %s ", operation);
		linguistic_preprocessing(tks[i], strlen(tks[i]));
		parray[i] = find_postings_array(iindex, iicount, tks[i], &plength[i]);
		//display_postings(tks[i], parray[i]);
	}	
	va_end(ap);
	if(num == 1) return parray[0];
	sort_by_post_list_length(num, parray, plength, tks);
	printf("\nSorting by frequency: \n");
	for(i=0; i<num; i++) {
		display_postings(tks[i], parray[i]);		
	}
	if(!strcmp(operation, "AND")) {
		i = 1;
		result = parray[0];
		while(result && (i<num)) {
			result = intersect_postings_merge_algorithm(result, parray[i++]);
		}
	} else if(!strcmp(operation, "OR")) {
	} else if(!strcmp(operation, "NOT")) {
	}
	free(tks);
	free(parray);
	free(plength);
	return result;	
}

static void test_n_token_query(inverted_index_t **iindex, off_t iicount)
{
	// Test - 3
	char tk1[100], tk2[100], tk3[100], tk4[100]; 
	posting_list_t *result = NULL;
	strcpy(tk1, "Brutus");
	strcpy(tk2, "Caesar");
	strcpy(tk3, "Calpurnia");
	strcpy(tk4, "Julius");
	result = execute_N_token_query(iindex, iicount, "AND", 4, tk1, tk2, tk3, tk4);
	display_postings("Result", result);	
	free_postings_memory(result);
}

posting_list_t *execute_2_token_query(inverted_index_t **iindex, off_t iicount, char *operation, char *tk1, char *tk2)
{
	off_t i;
	printf("\nQuery : %s %s %s \n", tk1, operation, tk2);	
	linguistic_preprocessing(tk1, strlen(tk1));
	linguistic_preprocessing(tk2, strlen(tk2));
	//printf("Linguistic Preprocessing of tokens (%s - %s) \n", tk1, tk2);
	posting_list_t *p1 = NULL, *p2 = NULL;
	for(i=0; i<iicount; i++) {
		//printf("[%s] : [%s] [%s]\n", iindex[i]->token, tk1, tk2);
		if(!strcmp(iindex[i]->token, tk1)) p1 = iindex[i]->plist;
		if(!strcmp(iindex[i]->token, tk2)) p2 = iindex[i]->plist; 
		if(p1 && p2) break;
	}
	posting_list_t *result = NULL;
	if(!strcmp(operation, "AND")) {
		result = intersect_postings_merge_algorithm(p1, p2);		
	} else if(!strcmp(operation, "OR")) {
	} else if(!strcmp(operation, "NOT")) {
	}	
	return result;	
}

static void test_queries(inverted_index_t **iindex, off_t iicount)
{
	char tk1[100], tk2[100]; 
	posting_list_t *result = NULL;
	// Test - 0	
	strcpy(tk1, "Brutus");
	strcpy(tk2, "Caesar");
	result = execute_2_token_query(iindex, iicount, "AND", tk1, tk2);
	display_postings("Result", result);	
	free_postings_memory(result);

	// Test - 1
	strcpy(tk1, "I");
	strcpy(tk2, "did");
	result = execute_2_token_query(iindex, iicount, "AND", tk1, tk2);
	display_postings("Result", result);	
	free_postings_memory(result);

	// Test - 2
	strcpy(tk1, "LeT");
	strcpy(tk2, "Me");
	result = execute_2_token_query(iindex, iicount, "AND", tk1, tk2);
	display_postings("Result", result);	
	free_postings_memory(result);

	test_n_token_query(iindex, iicount);

	free_inv_memory(iindex, iicount);
}

static void generate_inverted_index(doc_info_t **dinfo, off_t tlength)
{
	off_t i;	
	off_t tmem = 0;
	off_t iicount = 0;
	for(i=0; i<tlength; i++) tmem += dinfo[i]->length;
	inverted_index_t **iindex = (inverted_index_t **) calloc(sizeof(inverted_index_t *), tmem);

	// Unique elements sorting
	for(i=0; i<tlength; i++) 	
		iicount = unique_sort_array(dinfo[i]->doc_id, dinfo[i]->tokens, dinfo[i]->length, iindex, iicount);

	display_inverted_index(iindex, iicount);
	test_queries(iindex, iicount);	
}	

/*
static void free_query_memory(char **qtokens, int num_tks)
{
	int i;
	for(i=0; i<num_tks; i++) {
		free(qtokens[i]);
	}
	free(qtokens);
}

static void query_processing(doc_info_t **dinfo, off_t tlength)
{
	char query[1024];
	printf("\nEnter conjunctive query: ");
	scanf("%[^\n]", query);
	//printf("Query: %s\n", query);
	int length = strlen(query);
	int num_tks= count_terms(query, length);
	
	printf("Number of query tokens : %d\n", num_tks);
	char **qtokens = (char **) calloc(sizeof(char *), num_tks);
	int start = 0, i;
	num_tks = 0;
	for(i=0; i<=length; i++) {		
		if(is_token_separator(query[i])) {
			qtokens[num_tks++] = allocate_token(query+start, i-start);			
			start = i + 1;
		}
	}
	display_terms("Query", qtokens, num_tks);	
	free_query_memory(qtokens, num_tks);	
}
*/

static void free_doc_memory(doc_info_t **dinfo, off_t length) 
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
			//display_terms("Debug", dinfo[i-1]->tokens, dinfo[i-1]->length);
		}		
		generate_inverted_index(dinfo, argc-1);		
		free_doc_memory(dinfo, argc-1);
		display_time("\nTime taken to complete execution", start, micro_second_clock());		
		//query_processing(dinfo, argc-1);		
	} else {
		printf("Error: Supply filenames as arguments\n");
		printf("Usage: %s filename1 filename2 . . .\n", argv[0]);
		ret = 1;
	}	
	return ret;
}
