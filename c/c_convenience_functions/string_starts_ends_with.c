#include <stdio.h>
#include <string.h>

int ends_with(const char *str, const char *s)
{
	int i, j;
	if(str && s && strlen(str) && strlen(s)) {
		for(i=strlen(str), j=strlen(s); i>=0 && j>=0; i--, j--) {
			if(s[j] != str[i]) return 0;
		}
		return 1;
	}
	return 0;
}

int starts_with(const char *str, const char *s)
{
	unsigned int i;
	if(str && s && strlen(str) && strlen(s)) {
		for(i=0; str[i]!='\0' && s[i]!='\0'; i++) {
			if(s[i] != str[i]) return 0;
		}
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	printf("%s\n", (starts_with("Hello World!", "Hello")) ? "Yes" : "No");
	printf("%s\n", (starts_with("Hello World!", "hello")) ? "Yes" : "No");
	printf("%s\n", (starts_with("Hello World!", "H")) ? "Yes" : "No");
	printf("%s\n", (ends_with("Hello World!", "World")) ? "Yes" : "No");
	printf("%s\n", (ends_with("Hello World!", "Hello")) ? "Yes" : "No");
	printf("%s\n", (ends_with("Hello World!", "!")) ? "Yes" : "No");
	return 0;
}
