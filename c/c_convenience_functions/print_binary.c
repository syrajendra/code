#include <stdio.h>
#include <iostream>

template <class T>
void number_to_binary(T num) {
	T mask = 1;
	for(T i=(sizeof(num)*CHAR_BIT)-1; i>=0; i--) {
		printf("%d", ( num & (mask<<i)) ? 1 : 0);
		if((i%8) == 0) printf(" ");
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	number_to_binary(1);
	number_to_binary(10);
	number_to_binary(512);
	number_to_binary(1024);
	number_to_binary(2048);
}
