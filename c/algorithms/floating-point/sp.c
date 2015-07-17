#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

static char g_float[(sizeof(float)*CHAR_BIT)];
static char g_double[(sizeof(double)*CHAR_BIT)];

typedef union {
    float f;
    struct {
        unsigned int mantissa  : 23;
        unsigned int exponent : 8;
        unsigned int sign     : 1;
    } parts;
} single_precision_format;

typedef union {
    double d;
    struct {
        unsigned long long int mantissa : 52;
        unsigned int exponent : 11;
        unsigned int sign     : 1;
    } parts;
} double_precision_format;

char *dec_to_binary(unsigned int num_bytes, unsigned int n)
{
    int i, j;
    int fsize    = sizeof(float)*CHAR_BIT;
    memset(g_float, '\0', fsize);
    for(i=0, j=num_bytes-1; i<num_bytes; i++, j--)
        g_float[j] = ((n >> i) & 0x1) ? '1' : '0';
    return g_float;
}

int binary_to_decimal(char *str)
{
    int i, j;
    double dec = 0;
    for(i=0, j=strlen(str)-1; i<strlen(str); i++, j--)
        dec += str[i] == '1' ? pow(2, j) : 0;
    return dec;
}

int binary_to_hex(char *str)
{
	return binary_to_decimal(str);
}

char *fraction_to_binary(float n)
{
    int fsize    = sizeof(float) * CHAR_BIT;
    memset(g_float, '\0', fsize);
    float fraction = n - (long)n;
    float result;
    int count = 0;
    do {
        result      = fraction * 2;
        printf("%f x 2 = %f = ", fraction, result);
        fraction    = result - (long)result;
        long decimal= result - fraction;
        printf("%lu + %f =>  %lu\n", decimal, fraction, decimal);
        g_float[count]  =  decimal ? '1' : '0';
        if(count++ > fsize) return g_float;
        result         =  fraction;
    } while(result);
    return g_float;
}

int normalize(char *decimal_part)
{
    int i, pos, fsize = sizeof(float) * CHAR_BIT;
    for(i=fsize-1; i>=0; i--) {
        if(decimal_part[i] == '1') pos = i;
    }
    strcpy(decimal_part, decimal_part + pos + 1);
    pos = fsize - pos - 1;
    printf("Normalization of decimal part : exponent = %d binary left = %s \n", pos, decimal_part);
    return pos;
}

int denormalize(char *fraction_part)
{
    int i, pos, fsize = sizeof(float) * CHAR_BIT;
    for(i=0; i<fsize; i++) {
        if(fraction_part[i] == '1') {
            pos = i + 1;
            break;
        }
    }
    strcpy(fraction_part, fraction_part + pos);
    pos = -pos;
    printf("Denormalization of fractional part : exponent = %d binary left = %s \n", pos, fraction_part);
    return pos;
}

void number_to_binary(unsigned int num)
{
	int i;
	for(i=(sizeof(num)*8)-1; i>=0; i--) {
		printf("%d", (num & (1<<i)) ? 1 : 0);
		if((i%8) == 0) printf(" ");
	}
	printf("\n");
}

void single_pf(float arg)
{
    single_precision_format spf;
    //spf.f = 0.15625;
    //spf.f = 12.375;
    spf.f = arg;
    printf("Single precision floating point number %f using excess-127 format\n\n", spf.f);
    {
        printf("My calculation: \n");
        char sign = spf.f > 0 ? '0' : '1';
        char *decimal_part = strdup(dec_to_binary(sizeof(float) * CHAR_BIT, (unsigned int)spf.f));
        printf("Decimal part is %u to binary = %s\n", (unsigned int)spf.f, decimal_part);
        char *fraction_part= strdup(fraction_to_binary(spf.f));
        printf("Fraction part is %f to binary = %s \n", spf.f - (long)spf.f, fraction_part);
        char mantissa[23];
        int exponent;
        int excess_127;
        if((unsigned int)spf.f > 0) {
            exponent =  normalize(decimal_part);
            excess_127 = 127 + exponent;
            sprintf(mantissa, "%s%s", decimal_part, fraction_part);
        } else {
            exponent    = denormalize(fraction_part);
            excess_127  = 127 + exponent;
            sprintf(mantissa, "%s", fraction_part);
        }
        memset(mantissa+strlen(mantissa), '0', 23 - strlen(mantissa));
        printf("\nExponent : 8 bits [23-30]\n Dec = %d + %d = %d\n Hex = 0x%x\n Bin = %s\n"
                , 127
                , exponent
                , excess_127
                , excess_127
                , dec_to_binary(8, excess_127));

         printf("\nMantisa : 23 bits [0-22]\n Dec = %d\n Hex = 0x%x\n Bin = %s\n"
                , binary_to_decimal(mantissa)
                , binary_to_hex(mantissa)
                , mantissa);

        printf("IEEE 754 binary 32 format representation of %f = %c-%s-%s\n"
                , spf.f
                , sign
                , dec_to_binary(8, excess_127)
                , mantissa);
        free(decimal_part);
        free(fraction_part);
    }
    {
        printf("\nActual: \n");
        printf("Sign : 1 bit\n Bit[31] = %x <%s>\n", spf.parts.sign, spf.parts.sign ? "-ve number" : "+ve number");
        char *exponent = strdup(dec_to_binary(8, spf.parts.exponent));
        printf("\nExponent : 8 bits [23-30]\n Dec = %d\n Hex = 0x%x\n Bin = %s\n"
                , spf.parts.exponent
                , spf.parts.exponent
                , exponent);
        char *mantissa = strdup(dec_to_binary(23, spf.parts.mantissa));
        printf("\nMantisa : 23 bits [0-22]\n Dec = %d\n Hex = 0x%x\n Bin = %s\n"
                , spf.parts.mantissa
                , spf.parts.mantissa
                , mantissa);

        printf("IEEE 754 binary 32 format representation of %f = %c-%s-%s\n"
                , spf.f
                , spf.parts.sign ? '1' : '0'
                , exponent
                , mantissa);
        free(exponent);
        free(mantissa);
    }
    {
	    //FIXME
	    int pos, neg_pos = 0, mi;
	    char f_str[33];
	    double f = 0;
	    f_str[31] = (spf.parts.sign) ? 1 : 0;
	    strcpy(f_str+1, dec_to_binary(8, spf.parts.exponent));
	    strcpy(f_str+9, dec_to_binary(23, spf.parts.mantissa));

	    //spf.f = 12.375; //-0.2109375;// 728.25;
	    //spf.parts.sign = 1;
	    //spf.parts.exponent = 70;
	    //spf.parts.mantissa = 3506176;
	    printf("\nConvert Binary %s to decimal\n", f_str);
	    pos = spf.parts.exponent - 127; // 127 format
	    if (pos < 0) {
	    	if (-pos > 22) { // exponent is far from zero, convert the original (normalized) mantissa
	    		neg_pos = pos;
	    		pos = 0;
	    		f += pow(2, pos--);
	    	} else { // Denormalize
	    		spf.parts.mantissa >>= 1;
	    		spf.parts.mantissa |= (1<<22);
	    		pos = -pos;
	    		pos = pos - 1;
	    		spf.parts.mantissa >>= pos;
	    		pos = -1;
	    	}
	    } else {
	    	f += pow(2, pos--);
	    }
	    mi = 23;
	    while(--mi >= 0) {
	    	if(spf.parts.mantissa & (1 << mi)) f += pow(2, pos);
	    	--pos;
	    }
	    printf("Decimal: %c%f ", (spf.parts.sign)?'-':' ', f);
	    if (pos < 0 && neg_pos != 0) {
	    	printf("X 2^%d", neg_pos);
	    }
	    printf("\n");
	}
}


int main(int argc, char *argv[])
{
    if(argc == 2) single_pf(atof(argv[1]));
    return 0;
}
