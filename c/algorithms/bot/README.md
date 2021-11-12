Bot Program:
------------
A particular ‘RPA’ firm is in the process of developing a restaurant which has bot waiters.
The bot needs to deliver the food from the specified exit door, of the kitchen with multiple exit doors to the specified table. 

The restaurant has blocked areas where Bot cannot move and the passages where it can move. The layout of the restaurant can be rectangular or square. 
The layout map can be considered as a matrix of cells where cell with ‘0’ represents the passage and cell with ‘1’, the blocked area.
The bot has memorized this map in a particular format for faster processing. Given a particular table, the location bot needs to find the path
from the kitchen from the specified exit door to the table to serve the customer.

The bot has a constraint it can move only in either right or down direction one step at a time (i.e. only to the adjacent 
cell in right or down direction provided it is not blocked) with first preference to the right direction.

Layout matrix is stored in the file input.txt 

You need to provide the code that helps the bot with the path from the specific exit door of the kitchen to the
mentioned table if there exists one. If multiple paths exist your code should find anyone. 

Implementation:
---------------
Your code should provide the following functionalities

1. Read input from a file 
Layout matrix consists of ‘0’ (allowed for moving) and ‘1’ (Blocked area) 
Your code should read the layout matrix from the file input.txt (file reading) and store it as a multi-list data structure
 
You need to scan the layout matrix and create a node whenever ‘0’ is encountered in the input file.

The node should maintain x coordinate, y coordinate (coordinate position with respect to the input file, indexing starts from 0) for each location
with ‘0’ value, link to the next node in the same row, link to the next node in the same column. 

2. Find path
Help bot to find a path from the start point to the endpoint if one exists. 
You should access data only from the created multi-linked list structure. You should provide an iterative implementation for this functionality.
There could be multiple paths, your program should find out any one of the available paths which satisfies the constraint. 

3. Store path
Your program should create an output file out.txt 

Input file description:
First-line indicates the start point for the Bot, the second-line termination point, 3rd line onwards represents the layout matrix. 

1.
Sample Input file: 
00
33
0111 
0010
0011
1000

Sample Output file: 
00
10
11
21
31
32
33

2.
Sample Input file:
0 0 0 0 0 1 1 0 0 0
1 1 0 1 0 0 1 0 1 0
1 1 0 1 0 0 0 1 1 0
0 1 0 0 0 1 0 0 1 0
1 1 1 0 1 1 1 0 1 0
0 1 0 0 0 1 1 0 0 1
1 1 1 1 0 1 1 0 1 0
1 0 0 0 0 0 0 0 1 1
0 0 0 0 0 1 1 0 0 0
1 1 0 1 1 0 0 1 1 0

Sample Output file:
0 0
0 1
0 2
0 3
0 4
1 4
1 5
2 5
2 6
3 6
3 7
4 7
5 7
6 7
7 7
8 7
8 8
8 9
9 9

3.
Sample Input file: 
0 0 0 1
0 1 0 1
0 0 1 1
1 0 1 0

Sample Output file:
Error: Failed to find a path to endpoint

4.
Sample Input file:
0 0 0 0 0 0 0 0 0 1
1 1 0 1 1 0 1 0 1 1
1 1 0 1 0 1 0 1 1 0
0 1 0 0 0 1 0 0 1 0
1 1 1 0 1 1 1 0 1 0
0 1 0 0 0 1 1 0 0 1
1 1 1 1 0 1 1 0 1 0
1 0 0 0 0 0 0 0 1 1
0 0 0 0 0 1 1 1 0 0
1 1 0 1 1 0 0 1 1 0

Sample Output file:
Error: Failed to find a path to endpoint
