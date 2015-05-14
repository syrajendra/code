#include <stdio.h>
#include <sys/types.h>

typedef union {
	int32_t data;
	unsigned char bytes[4];
} dmap;

int endianity()
{
	int x = 1;
	char *start_mem = (char *)(&x);
	if (*start_mem == 1) {
	    	fprintf(stderr, "Little endian\n");
		return 0;
	} else {
	    	fprintf(stderr, "Big endian\n");
		return 1;
	}
}

int endian_convert(int32_t data)
{
	dmap d1, d2;
	d1.data = d2.data = data;
	d1.bytes[0] = d2.bytes[3];
	d1.bytes[1] = d2.bytes[2];
	d1.bytes[2] = d2.bytes[1];
	d1.bytes[3] = d2.bytes[0];
	return d1.data;
}

int main(int argc, char *argv[])
{
	endianity();
	int32_t data = 256;
	dmap d;
	d.data = data;
	if(d.data == data) { // little endian
		int32_t cdata 	= endian_convert(data);
		printf("Data: %d Big-endian: %d \n", data, cdata);
		printf("Data Little-endian: %d\n", endian_convert(cdata));
	} else { // Big endian
		int32_t cdata 	= endian_convert(cdata);
		printf("Data: %d Little-endian: %d \n", data, cdata);
		printf("Data Big-endian: %d\n", endian_convert(cdata));
	}
	return 0;
}
