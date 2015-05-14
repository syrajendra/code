#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

int str_to_int(char *str) 
{
	int ret  = 0;
	int sign = 1;
	errno	 = 0;
	char flow[6];
	memset(flow, '\0', 6);
	if(str) {
		if(*str == '-') {
			sign = -1;
			str++;
		}
		while(*str) {
			if (*str >= '0' && *str <= '9') {
				//printf("ret = %d\n", ret);
				ret = (ret * 10) + (*str++ - '0');
				if(sign == 1 && (INT_MAX-ret) < 0)  memcpy(flow, "Over", 4);
				if(sign == -1 && (INT_MIN+ret) > 0) memcpy(flow, "Under", 5);
				if(strlen(flow)) {				
					printf("Error: %sflow\n", flow);
					ret = 0;
					errno = EILSEQ; // Illegal byte sequence	
					break;
				}
			} else {
				printf("Error: Input string is not a numeric\n");
				ret = 0;
				errno = EILSEQ; // Illegal byte sequence
				break;
			}
		}
	} else {
		printf("Error: Input string is null\n");
		errno = EILSEQ; // Illegal byte sequence
	}
	return ret * sign;
}

int main(int argc, char *argv[]) {		
	printf("[NULL] = %d\n\n", str_to_int(NULL));	
	printf("[1234] = %d\n\n", str_to_int("1234"));
	printf("[-1234] = %d\n\n", str_to_int("-1234"));
	printf("[-1a234] = %d\n\n", str_to_int("-1a234"));
	printf("[0] = %d\n\n", str_to_int("0"));
	printf("[2147483647] = %d\n\n", str_to_int("2147483647")); // Good
	printf("[2147483648] = %d\n\n", str_to_int("2147483648")); // Overflow	 
	printf("[4294967295] = %d\n\n", str_to_int("4294967295")); // Overflow
	printf("[-2147483648] = %d\n\n", str_to_int("-2147483648")); // Good
	printf("[-2147483649] = %d\n\n", str_to_int("-2147483649")); // Underflow
	printf("[-9223372036854775807] = %d\n\n", str_to_int("-9223372036854775807")); // Underflow
	return 0;	
}
