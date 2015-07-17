#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

uint64_t factorial(int n)
{
	if(n == 0) return 1;
	return n * factorial(n-1);
}

void constant_time_algorithm_0(int n)
{
	printf("Constant O(1) = %d\n",1);
}

void logarithmic_base10_algorithm_1(int n)
{
	double result = log10(n);
	double input_size  = pow(10, result);
	printf("Logarithmic O(log10 (%d)) = %f ", n, result);
	printf("<==> 10 ^ %f = %f\n", result, input_size);
}

void logarithmic_base2_algorithm_1(int n)
{
	double result = log2(n);
	double input_size  = pow(2, result);
	printf("Logarithmic O(log2 (%d)) = %f ", n, result);
	printf("<==> 2 ^ %f = %f\n", result, input_size);
}

void linear_algorithm_2(int n)
{
	printf("Linear O(%d) = %d \n", n, n);	
}

void superlinear_base10_algorithm_3(int n)
{
	printf("Superlinear O(%d log10 (%d)) =%f <==> Linearithmic\n", n, n, n * log10(n));	
}

void superlinear_base2_algorithm_3(int n)
{
	printf("Superlinear  O(%d log2 (%d)) =%f <==> Linearithmic\n", n, n, n * log2(n));	
}

void quadratic_algorithm_4(int n, int c)
{
	printf("Quadratic O(%d ^ %d) = %f \n", n, c, pow(n, c));	
}

void cubic_algorithm_5(int n, int c)
{
	printf("Cubic O(%d ^ %d) = %f \n", n, c, pow(n, c));	
}

void polynomial_algorithm_6(int n)
{
	int k; // K some constant
	printf("Polynomial O(%d ^ k) for some constant k > 3\n", n);	
}

void exponential_algorithm_7(int n, int c)
{
	printf("Exponential O(%d ^ %d) = %f \n", c, n, pow(c, n));	
}

void factorial_algorithm_8(int n)
{
	printf("Factorial O(%d!) = %lu \n", n, factorial(n));	
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		int input = atoi(argv[1]);
		constant_time_algorithm_0(input);
		logarithmic_base10_algorithm_1(input);
		logarithmic_base2_algorithm_1(input);
		linear_algorithm_2(input);
		superlinear_base10_algorithm_3(input);
		superlinear_base2_algorithm_3(input);
		quadratic_algorithm_4(input, 2);
		cubic_algorithm_5(input, 3);
		polynomial_algorithm_6(input);
		exponential_algorithm_7(input, 2);
		factorial_algorithm_8(input);
	}
	return 0;
}