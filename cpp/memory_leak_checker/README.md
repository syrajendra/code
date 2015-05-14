Static Memory Leak Checker
--------------------------
This will list memory leaks in test applications.

Steps
-----
1. Add rmemcheck.h file in source file as a last header file
2. Build test applications with 'rmemcheck.cpp' file
3. By default all memory leaks are printed on 'stderr' 
4. To redirect prints call set_log_file(filename)
5. For applications which run in loop can trigger SIGUSR1 signal to get memory leak info

Compile
-------
$ g++ -o test test.cpp rmemcheck.cpp

Run
---
$ ./test

Output
------
Problem - 1: Memory leak [8 bytes] @ test.cpp:9
Problem - 2: Memory leak [50 bytes] @ test.cpp:8

Summary : 
        At 2 place(s) Memory Leak(s) Detected
        Totally [58 bytes] of Memory Leak


Trigger SIGUSR1 signal
----------------------
$ kill -s 10 <pid>
