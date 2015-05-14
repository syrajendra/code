#include <stdio.h>
#include <string.h>

/* This function manipulates string  content */
int split(char *str, char ch, char **str1, char **str2)
{
	unsigned int i;
	int ret = 0;
	if(str) {
		*str1 = str;
		for(i = 0; str[i] != '\0'; i++) {
			if(str[i] == ch) {
				ret     = 1;
				*str2   = str + i + 1;
				str[i]  = '\0';
				break;
			}
		}
	}
	return ret;
}

int main(int argc, char *argv[]) 
{
	//char *str = "Hello ! World"	<== This will crash because this goes in to read only memory
	char *str = strdup("Hello ! World"); // Always provide heap memory pointer
	char *str1 = NULL, *str2 = NULL;
	split(str, '!', &str1, &str2);
	printf("Str1 = %s Str2 = %s\n", str1, str2);
	free(str);
}
