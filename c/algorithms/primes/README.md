    $ gcc -O2 -Wall -o prime ./prime.c -lrt -lm
    $ ./prime 10
    Prime numbers till 10
    0 : -----
    1 : -----
    2 : Prime
    3 : Prime
    4 : -----
    5 : Prime
    6 : -----
    7 : Prime
    8 : -----
    9 : -----
    Time taken to complete execution of is_prime_basic() -  329 microseconds

    0 : -----
    1 : -----
    2 : Prime
    3 : Prime
    4 : -----
    5 : Prime
    6 : -----
    7 : Prime
    8 : -----
    9 : -----
    Time taken to complete execution of is_prime_better() -  123 microseconds

    List of primes till 10 :  2  3  5  7 
    Total primes : 4
    Time taken to complete execution of is_prime_better() -  41 microseconds

    List of primes till 10 using sieve :  2  3  5  7 
    Total primes : 4
    Time taken to complete execution of is_prime_sieve() -  45 microseconds
