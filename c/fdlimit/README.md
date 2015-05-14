Increase file descriptor limit
===============================

This is a small utility which will change increse file descritptor limit and run the application.

    $ gcc -o fdlimit fdlimit.c
    $  ./fdlimit -h
      Usage: ./fdlimit <fd_limit> <command>
    $ ./fdlimit
      Default file descriptor limit soft : 1024 hard : 1024
    $ sudo  ./fdlimit 2048 ./program
 
 



