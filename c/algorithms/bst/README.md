    $ gcc -o bst ./bst.c -lrt
    $ ./bst 5 2 12 4 3 9 21 19 25
	--------------- InsertRecursive ------------------
	Number of nodes recursive: 9
	Number of nodes iterative: 9
	Number of levels recursive: 4
	Number of levels iterative: 4
	
	Preorder recursive (DLR) :  5  2  4  3  12  9  21  19  25 
	Preorder iterative using stack (DLR) :  5  2  4  3  12  9  21  19  25 
	Preorder Morris Traversal (DLR):  5  2  4  3  12  9  21  19  25 
	Inorder recursive (LDR)	 :  2  3  4  5  9  12  19  21  25 
	Inorder Morris Traversal (LDR):  2  3  4  5  9  12  19  21  25 
	Postorder recursive (LRD)	 :  3  4  2  9  19  25  21  12  5 
	Detailed tree: 
	Root=     5 Left=     2 Right=    12 Count=     9
	Root=     2 Left=-99999 Right=     4 Count=     3
	Root=    12 Left=     9 Right=    21 Count=     5
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=     9 Left=-99999 Right=-99999 Count=     1
	Root=    21 Left=    19 Right=    25 Count=     3
	Root=     3 Left=-99999 Right=-99999 Count=     1
	Root=    19 Left=-99999 Right=-99999 Count=     1
	Root=    25 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- InsertIterative ------------------
	Number of nodes recursive: 9
	Number of nodes iterative: 9
	Number of levels recursive: 4
	Number of levels iterative: 4
	
	Preorder recursive (DLR) :  5  2  4  3  12  9  21  19  25 
	Preorder iterative using stack (DLR) :  5  2  4  3  12  9  21  19  25 
	Preorder Morris Traversal (DLR):  5  2  4  3  12  9  21  19  25 
	Inorder recursive (LDR)	 :  2  3  4  5  9  12  19  21  25 
	Inorder Morris Traversal (LDR):  2  3  4  5  9  12  19  21  25 
	Postorder recursive (LRD)	 :  3  4  2  9  19  25  21  12  5 
	Detailed tree: 
	Root=     5 Left=     2 Right=    12 Count=     9
	Root=     2 Left=-99999 Right=     4 Count=     3
	Root=    12 Left=     9 Right=    21 Count=     5
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=     9 Left=-99999 Right=-99999 Count=     1
	Root=    21 Left=    19 Right=    25 Count=     3
	Root=     3 Left=-99999 Right=-99999 Count=     1
	Root=    19 Left=-99999 Right=-99999 Count=     1
	Root=    25 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- BST ------------------
	Trees are same
	Tree-1 is BST
	Tree-2 is BST
	------------------------------------------
	--------------- Operations ------------------
	Floor recursive of 3 = 3
	Ceil recursive  of 3 = 3
	Rank recursive  of 3 = 1
	Floor recursive of 8 = 5
	Ceil recursive  of 8 = 9
	Rank recursive  of 8 = 4
	------------------------------------------
	--------------- Search ------------------
	DFS recursive traversal:  3  4  2  9  19 
	DFS recursive for 19 found
	DFS iterative traversal:  3  4  2  9  19 
	DFS iterative for 19 found
	BFS iterative traversal:  5  2  12  4  9  21  3  19 
	BFS for 19 found
	------------------------------------------
	--------------- Minimum ------------------
	Minimum iterative 2 
	Minimum recursive 2 
	------------------------------------------
	--------------- DeleteIterative:5 ------------------
	Number of nodes recursive: 8
	Number of nodes iterative: 8
	Number of levels recursive: 4
	Number of levels iterative: 4
	
	Preorder recursive (DLR) :  9  2  4  3  12  21  19  25 
	Preorder iterative using stack (DLR) :  9  2  4  3  12  21  19  25 
	Preorder Morris Traversal (DLR):  9  2  4  3  12  21  19  25 
	Inorder recursive (LDR)	 :  2  3  4  9  12  19  21  25 
	Inorder Morris Traversal (LDR):  2  3  4  9  12  19  21  25 
	Postorder recursive (LRD)	 :  3  4  2  19  25  21  12  9 
	Detailed tree: 
	Root=     9 Left=     2 Right=    12 Count=     8
	Root=     2 Left=-99999 Right=     4 Count=     3
	Root=    12 Left=-99999 Right=    21 Count=     4
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=    21 Left=    19 Right=    25 Count=     3
	Root=     3 Left=-99999 Right=-99999 Count=     1
	Root=    19 Left=-99999 Right=-99999 Count=     1
	Root=    25 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- DeleteIterative:12 ------------------
	Number of nodes recursive: 7
	Number of nodes iterative: 7
	Number of levels recursive: 4
	Number of levels iterative: 4
	
	Preorder recursive (DLR) :  9  2  4  3  21  19  25 
	Preorder iterative using stack (DLR) :  9  2  4  3  21  19  25 
	Preorder Morris Traversal (DLR):  9  2  4  3  21  19  25 
	Inorder recursive (LDR)	 :  2  3  4  9  19  21  25 
	Inorder Morris Traversal (LDR):  2  3  4  9  19  21  25 
	Postorder recursive (LRD)	 :  3  4  2  19  25  21  9 
	Detailed tree: 
	Root=     9 Left=     2 Right=    21 Count=     7
	Root=     2 Left=-99999 Right=     4 Count=     3
	Root=    21 Left=    19 Right=    25 Count=     3
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=    19 Left=-99999 Right=-99999 Count=     1
	Root=    25 Left=-99999 Right=-99999 Count=     1
	Root=     3 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- DeleteIterative:25 ------------------
	Number of nodes recursive: 6
	Number of nodes iterative: 6
	Number of levels recursive: 4
	Number of levels iterative: 4
	
	Preorder recursive (DLR) :  9  2  4  3  21  19 
	Preorder iterative using stack (DLR) :  9  2  4  3  21  19 
	Preorder Morris Traversal (DLR):  9  2  4  3  21  19 
	Inorder recursive (LDR)	 :  2  3  4  9  19  21 
	Inorder Morris Traversal (LDR):  2  3  4  9  19  21 
	Postorder recursive (LRD)	 :  3  4  2  19  21  9 
	Detailed tree: 
	Root=     9 Left=     2 Right=    21 Count=     6
	Root=     2 Left=-99999 Right=     4 Count=     3
	Root=    21 Left=    19 Right=-99999 Count=     2
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=    19 Left=-99999 Right=-99999 Count=     1
	Root=     3 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- DeleteIterative:2 ------------------
	Number of nodes recursive: 5
	Number of nodes iterative: 5
	Number of levels recursive: 3
	Number of levels iterative: 3
	
	Preorder recursive (DLR) :  9  4  3  21  19 
	Preorder iterative using stack (DLR) :  9  4  3  21  19 
	Preorder Morris Traversal (DLR):  9  4  3  21  19 
	Inorder recursive (LDR)	 :  3  4  9  19  21 
	Inorder Morris Traversal (LDR):  3  4  9  19  21 
	Postorder recursive (LRD)	 :  3  4  19  21  9 
	Detailed tree: 
	Root=     9 Left=     4 Right=    21 Count=     5
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=    21 Left=    19 Right=-99999 Count=     2
	Root=     3 Left=-99999 Right=-99999 Count=     1
	Root=    19 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- DeleteRecursive:5 ------------------
	Number of nodes recursive: 8
	Number of nodes iterative: 8
	Number of levels recursive: 4
	Number of levels iterative: 4
	
	Preorder recursive (DLR) :  9  2  4  3  12  21  19  25 
	Preorder iterative using stack (DLR) :  9  2  4  3  12  21  19  25 
	Preorder Morris Traversal (DLR):  9  2  4  3  12  21  19  25 
	Inorder recursive (LDR)	 :  2  3  4  9  12  19  21  25 
	Inorder Morris Traversal (LDR):  2  3  4  9  12  19  21  25 
	Postorder recursive (LRD)	 :  3  4  2  19  25  21  12  9 
	Detailed tree: 
	Root=     9 Left=     2 Right=    12 Count=     8
	Root=     2 Left=-99999 Right=     4 Count=     3
	Root=    12 Left=-99999 Right=    21 Count=     4
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=    21 Left=    19 Right=    25 Count=     3
	Root=     3 Left=-99999 Right=-99999 Count=     1
	Root=    19 Left=-99999 Right=-99999 Count=     1
	Root=    25 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- DeleteRecursive:12 ------------------
	Number of nodes recursive: 7
	Number of nodes iterative: 7
	Number of levels recursive: 4
	Number of levels iterative: 4
	
	Preorder recursive (DLR) :  9  2  4  3  21  19  25 
	Preorder iterative using stack (DLR) :  9  2  4  3  21  19  25 
	Preorder Morris Traversal (DLR):  9  2  4  3  21  19  25 
	Inorder recursive (LDR)	 :  2  3  4  9  19  21  25 
	Inorder Morris Traversal (LDR):  2  3  4  9  19  21  25 
	Postorder recursive (LRD)	 :  3  4  2  19  25  21  9 
	Detailed tree: 
	Root=     9 Left=     2 Right=    21 Count=     7
	Root=     2 Left=-99999 Right=     4 Count=     3
	Root=    21 Left=    19 Right=    25 Count=     3
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=    19 Left=-99999 Right=-99999 Count=     1
	Root=    25 Left=-99999 Right=-99999 Count=     1
	Root=     3 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- DeleteRecursive:25 ------------------
	Number of nodes recursive: 6
	Number of nodes iterative: 6
	Number of levels recursive: 4
	Number of levels iterative: 4
	
	Preorder recursive (DLR) :  9  2  4  3  21  19 
	Preorder iterative using stack (DLR) :  9  2  4  3  21  19 
	Preorder Morris Traversal (DLR):  9  2  4  3  21  19 
	Inorder recursive (LDR)	 :  2  3  4  9  19  21 
	Inorder Morris Traversal (LDR):  2  3  4  9  19  21 
	Postorder recursive (LRD)	 :  3  4  2  19  21  9 
	Detailed tree: 
	Root=     9 Left=     2 Right=    21 Count=     6
	Root=     2 Left=-99999 Right=     4 Count=     3
	Root=    21 Left=    19 Right=-99999 Count=     2
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=    19 Left=-99999 Right=-99999 Count=     1
	Root=     3 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- DeleteRecursive:2 ------------------
	Number of nodes recursive: 5
	Number of nodes iterative: 5
	Number of levels recursive: 3
	Number of levels iterative: 3
	
	Preorder recursive (DLR) :  9  4  3  21  19 
	Preorder iterative using stack (DLR) :  9  4  3  21  19 
	Preorder Morris Traversal (DLR):  9  4  3  21  19 
	Inorder recursive (LDR)	 :  3  4  9  19  21 
	Inorder Morris Traversal (LDR):  3  4  9  19  21 
	Postorder recursive (LRD)	 :  3  4  19  21  9 
	Detailed tree: 
	Root=     9 Left=     4 Right=    21 Count=     5
	Root=     4 Left=     3 Right=-99999 Count=     2
	Root=    21 Left=    19 Right=-99999 Count=     2
	Root=     3 Left=-99999 Right=-99999 Count=     1
	Root=    19 Left=-99999 Right=-99999 Count=     1
	
	------------------------------------------
	--------------- Minimum ------------------
	Minimum iterative 3 
	Minimum recursive 3 
	------------------------------------------
	--------------- BST ------------------
	Trees are same
	Tree-1 is BST
	Tree-2 is BST
	------------------------------------------
	
	Time taken to complete execution 457 microseconds
	