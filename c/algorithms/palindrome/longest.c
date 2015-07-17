#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void substrcopy (const char *start, const char *end, char *dest)
{
	while ((start != (end+1)) && (*dest++ = *start++))
	*dest = '\0';
}

bool is_palindrome (const char *str, unsigned k, unsigned l)
{
	while (k <= l) {
		while (k<=l && isspace(str[k])) k++; // skip space
		while (l>=k && isspace(str[l])) l--; // skip space
		if (k == l) return true;
		if ((str[k] != str[l])  && (tolower(str[k]) != tolower(str[l]))) return false;
		if (k+1 == l) return true;
		k++;
		l--;
	}
	return false;
}

void find_longest_palindrome (const char *str)
{
	unsigned len = strlen(str);
	char temp[100];
	unsigned i, j;
	unsigned li = 0, lj = 0, longest = 0;
	unsigned int last_pal = 1;
	printf ("Input string [%s]\n", str);
	if(len == 1) {
		printf("Palindrome found = 1 = [%s]\n", str);
		return;
	}
	for (i=0; i < len; i++) {
		if (isspace(str[i])) continue;
		for (j=i+last_pal; j < len; j++) {
			if (isspace(str[j])) continue;
			if(is_palindrome(str, i, j)) {
				last_pal 	= j + 1;
				substrcopy(i+str, j+str, temp);
				printf("Palindrome found = %d = [%s]\n", strlen(temp), temp);
				if (j - i + 1 > longest) {
					longest = j - i + 1;
					li 		= i;
					lj 		= j;
				}
			}
		}
	}
	if (longest) {
		substrcopy(str+li, str+lj, temp);
		printf("Longest palindrome = %d = [%s]\n", longest, temp);
	} else printf ("No palindrome\n");
}

int main (int argc, char *argv[])
{

	const char *str0 = "N i  N";
	find_longest_palindrome(str0);
	const char *str1 = "Famous examples include Amor, Roma";
	find_longest_palindrome(str1);
	const char *str2 = "Amor, Roma";
	find_longest_palindrome(str2);
	const char *str3 = "Famous Nixin";
	find_longest_palindrome(str3);
	const char *str4 = "Famous Nixin examples include Amor, Roma. But A man a plan a canal Panama";
	find_longest_palindrome(str4);
	const char *str5 = "BBABCcbab";
	find_longest_palindrome(str5);
	const char *str6 = "forgeeksskeegfor";
	find_longest_palindrome(str6);
	const char *str7 = "AA";
	find_longest_palindrome(str7);
	const char *str8 = "A";
	find_longest_palindrome(str8);
	const char *str9 = "ABC";
	find_longest_palindrome(str9);
	return 0;
}
