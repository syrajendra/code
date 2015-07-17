#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <errno.h>

static uint64_t micro_second_clock()
{
        struct timespec  ts;
        struct tm       *tm;
        uint64_t   ret; // micro seconds
 
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

static void display_time(char *msg, uint64_t start, uint64_t stop)
{
        char precision[50];
        uint64_t result = stop - start;
        if (result > 1000000) {
                result = result/1000000;
                strcpy(precision, "seconds");                
                if(result > 60) {
                	//printf("%s %llu %s\n", msg, result, precision);                	
                	printf("%s %llumins %llusecs\n", msg, result/60, result%60);
                	return;
                }
        } else if (result > 1000) {
                result = result/1000;
                strcpy(precision, "milliseconds");
        } else {
               strcpy(precision, "microseconds");
        }
        printf("%s %llu %s\n", msg, result, precision);
}

// Prime - number divisible by 1 & itself
uint64_t is_prime_basic(uint64_t n)
{
	uint64_t i;
	if(n<=1) return 0;
	if(n==2) return 1;
	for(i=2; i<n; i++) if(n%i == 0) return 0;
	return 1;	
}

uint64_t is_prime_better(uint64_t n)
{
	uint64_t i;
	if(n<=1) return 0;
	if(n==2) return 1;
	if(n%2 == 0) return 0;
	double m = sqrt(n); // reduce loop to sqrt of number
	for(i=3; i<=m; i+=2) if(n%i == 0) return 0; // check only odd numbers
	return 1;	
}

// http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
void list_primes_sieve_of_eratosthenes(uint64_t n)
{
	uint64_t i, j;
	uint64_t count = 0;
	uint64_t start = micro_second_clock();
	uint64_t *not_primes= (uint64_t *) calloc(n+1, sizeof(uint64_t));
	if(!not_primes) {
		printf("Error: Failed to allocate memory of %llu bytes - %s\n", sizeof(uint64_t *)*(n+1),strerror(errno));
		return;
	}
	not_primes[0] = 1;
	not_primes[1] = 1;
	double m = sqrt(n); // reduce loop to sqrt of number
	for(i=2; i<=m; i++)
		if(!not_primes[i]) // for each prime
			for(j=i*i; j<=n; j+=i) // mark its multiples as not primes
				not_primes[j] = 1;
	printf("\nList of primes till %llu using sieve : ", n);
	for(i=0; i<=n; i++) {
		if(!not_primes[i])  {
			count++;
			printf(" %llu ", i);
		}
	}
	free(not_primes);
	printf("\nTotal primes : %llu\n", count);
	display_time("Time taken to complete execution of is_prime_sieve() - ", start, micro_second_clock());
}

void list_primes(uint64_t n)
{
	uint64_t i;
	uint64_t count = 0;
	uint64_t start = micro_second_clock();
	printf("\nList of primes till %llu : ", n);
	for(i=0; i<n; i++) {
		if(is_prime_better(i)) {
			count++;
			printf(" %llu ", i);
		}
	}
	printf("\nTotal primes : %llu\n", count);
	display_time("Time taken to complete execution of is_prime_better() - ", start, micro_second_clock());
}

int main(int argc, char *argv[])
{		
	if(argc > 1) {
		char *endptr, *str = argv[1];
		errno = 0;
		uint64_t num = strtoll(argv[1], &endptr, 10);
		if ((errno == ERANGE && (num == LLONG_MAX || num == LLONG_MIN)) || (errno != 0 && num == 0)) {
               		perror("strtol");
               		exit(EXIT_FAILURE);
           	}
           	if (endptr == str) {
               		fprintf(stderr, "No digits were found\n");
               		exit(EXIT_FAILURE);
           	}

           	printf("Prime numbers till %llu\n", num);

           	if (*endptr != '\0') printf("Inputted characters after number: %s\n", endptr);

		
		uint64_t i;
		uint64_t start = micro_second_clock();
		for(i=0; i<num; i++)
			printf("%llu : %s\n", i, is_prime_basic(i) ? "Prime" : "-----"); 
		display_time("Time taken to complete execution of is_prime_basic() - ", start, micro_second_clock());
		
		printf("\n");

		start = micro_second_clock();
		for(i=0; i<num; i++)
			printf("%llu : %s\n", i, is_prime_better(i) ? "Prime" : "-----"); 
		display_time("Time taken to complete execution of is_prime_better() - ", start, micro_second_clock());
		
		list_primes(num);
		list_primes_sieve_of_eratosthenes(num);		
	}
	return 0;
}