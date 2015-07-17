    $ gcc -o bigo bigo.c -lm
    $ ./bigo 16
	Constant O(1) 				= 1
	Logarithmic O(log10 (16)) 	= 1.204120 <==> 10 ^ 1.204120 = 16.000000
	Logarithmic O(log2 (16)) 	= 4.000000 <==>  2 ^ 4.000000 = 16.000000
	Linear O(16) 				= 16 
	Superlinear O(16 log10 (16))= 19.265920 <==> Linearithmic
	Superlinear O(16 log2 (16)) = 64.000000 <==> Linearithmic
	Quadratic O(16 ^ 2) 		= 256.000000 
	Cubic O(16 ^ 3) 			= 4096.000000 
	Polynomial O(16 ^ k) for some constant k > 3	
	Exponential O(2 ^ 16) 		= 65536.000000
	Factorial O(16!) 			= 20922789888000


	Explanation:
	------------
	Change-of-Base formula:
		log_b(x) = log_d(x) / log_d(b)
		
		Ex:
			log2(n) 	= log10(n) / log10(2)
			log2(16)	= log10(16) / log10(2) = 1.204120 / 0.301029 = 4.000000

		Ex: O(logn) where n = 16
		16/2 = 8
		8/2  = 4
		4/2  = 2
		2/2  = 1

		total steps = 4 = log2(16)

		Ex: O(loglogn) where n = 16
		sqrt(16) = 4
		sqrt(4)  = 2

		sqrt(16) = sqrt(2^4) = (2^4)^(1/2) = 2^2 = 4
		sqrt(4)  = sqrt(2^2) = (2^2)^(1/2) = 2^1 = 2

		total steps = 2 = log2(log2(16)) = log2(4) = 2