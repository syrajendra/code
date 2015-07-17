    Red Black Binary Search Tree
    ----------------------------
    Red-Black binary search tree (RB-BST) properties:
    - Every node is either red or black.
    - The root is black.
    - Every leaf contains NIL and is black.
    - If a node is red, then both its children are black.
    - For each node x in a RBT, all paths in sub-trees rooted at x contain the same number of black nodes.

    Left-leaning Red-Black binary search tree (LLRB-BST) properties:
    - No node has two red links connected to it
    - Every path from root to null has same number of black links
    - Red links always lean left

    RB-BST Algorithm:
    -----------------
    RBT-INSERT(T, z):
        1. Find proper Nil leaf node in tree T
        2. Insert new node 'z' with Color=RED, left & right links pointing to NIL
        3. Call RBT-INSERT-FIXUP(T, z)

    RBT-INSERT-FIXUP(T, z):
        while color parent[z] == RED do 
            if z is left child then /*  z is left child */
                uncle[z] = right sibling of parent[z]
                Case 1: uncle[z] is RED
                        color uncle[z] and parent[z] BLACK
                        color grandparent[z] RED
                        move z up to point to grandparent[z]
                Case 2: uncle[z] is BLACK
                        if z is right child
                            move z to point to parent[z]
                            LEFT-ROTATE(T z)
                Case 3: if z is left child then
                            color parent[z] BLACK
                            color grandparent[z] RED
                            RIGHT-ROTATE(T, grandparent[z])
            else /* z is right child */
                uncle[z] = left sibling of parent[z]
                Case 1: uncle[z] is RED
                        color uncle[z] and parent[z] BLACK
                        color grandparent[z] RED
                        move z up to point to grandparent[z]
                Case 2: uncle[z] is BLACK
                        if z is left child
                            move z to point to parent[z]
                            RIGHT-ROTATE(T z) 
                Case 3: if z is right child
                            color parent[z] BLACK
                            color grandparent[z] RED
                            LEFT-ROTATE(T, grandparent[z])
        color root BLACK    

    
    RBT-DELETE(T, z): TBD
            
    RBT-DELETE-FIXUP(T, z): TBD

    LLRB-BST Algorithm:
    -------------------

    LLRBT-INSERT(T, z): TBD
    
    LLRBT-DELETE(T, z): TBD
            
    LLRBT-DELETE-FIXUP(T, z): TBD

    Compile & Run:
    --------------
    $ gcc -O2 -o rbt rbt.c -lrt
    $ ./rbt 50 70 30 90 20 10 39 40 100 60 80 5 85 15

    Red black tree demo
    INFO:
     NIL - Nil black node
     STN - Sentinel Node
    Time taken to complete RBT insert 10 microseconds
    --------------------------------------------------------------------------------
    Red black tree detailed insert
    Node=   50 Left=   20 Right=   90 Parent=   -- Color=BLACK NodeCount= 14 Level=0
    Node=   20 Left=   10 Right=   39 Parent=   50 Color=BLACK NodeCount=  7 Level=1
    Node=   90 Left=   70 Right=  100 Parent=   50 Color=BLACK NodeCount=  6 Level=1
    Node=   10 Left=    5 Right=   15 Parent=   20 Color=BLACK NodeCount=  3 Level=2
    Node=   39 Left=   30 Right=   40 Parent=   20 Color=BLACK NodeCount=  2 Level=2
    Node=   70 Left=   60 Right=   80 Parent=   90 Color=  RED NodeCount=  3 Level=2
    Node=  100 Left=  NIL Right=  NIL Parent=   90 Color=BLACK NodeCount=  1 Level=2
    Node=    5 Left=  NIL Right=  NIL Parent=   10 Color=  RED NodeCount=  1 Level=3
    Node=   15 Left=  NIL Right=  NIL Parent=   10 Color=  RED NodeCount=  1 Level=3
    Node=   30 Left=  NIL Right=  NIL Parent=   39 Color=  RED NodeCount=  0 Level=3
    Node=   40 Left=  NIL Right=  NIL Parent=   39 Color=  RED NodeCount=  1 Level=3
    Node=   60 Left=  NIL Right=  NIL Parent=   70 Color=BLACK NodeCount=  1 Level=3
    Node=   80 Left=  NIL Right=   85 Parent=   70 Color=BLACK NodeCount=  2 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=  100 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=  100 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=    5 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=    5 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   15 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   15 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   30 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   30 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   40 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   40 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   60 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   60 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   80 Color=BLACK NodeCount=  0 Level=4
    Node=   85 Left=  NIL Right=  NIL Parent=   80 Color=  RED NodeCount=  1 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   85 Color=BLACK NodeCount=  0 Level=5
    Leaf=   -- Left=  STN Right=  STN Parent=   85 Color=BLACK NodeCount=  0 Level=5
    --------------------------------------------------------------------------------
    ---------------------------------------------------------------------------------------
    Red black tree insert
                                     50
                                     / \
                                    /   \
                                   /     \
                                  /       \
                                 /         \
                                /           \
                               /             \
                              /               \
                             /                 \
                            /                   \
                           /                     \
                          /                       \
                         /                         \
                        /                           \
                       /                             \
                      /                               \
                     /                                 \
                    /                                   \
                   20                                   90
                   / \                                  / \
                  /   \                                /   \
                 /     \                              /     \
                /       \                            /       \
               /         \                          /         \
              /           \                        /           \
             /             \                      /             \
            /               \                    /               \
           /                 \                  /                 \
          10                 39                70                100
          / \                / \               / \                / \
         /   \              /   \             /   \              /   \
        /     \            /     \           /     \            /     \
       /       \          /       \         /       \          /       \
       5       15        30       40       60       80       Nil      Nil
      / \      / \       / \      / \      / \      / \       / \      / \
     /   \    /   \     /   \    /   \    /   \    /   \     /   \    /   \
    Nil  Nil Nil  Nil  Nil  Nil Nil  Nil Nil  Nil Nil   85   ST   ST  ST   ST
    / \  / \ / \  / \  / \  / \ / \  / \ / \  / \ / \  / \  / \  / \ / \  / \
    ---------------------------------------------------------------------------------------
    Time taken to complete RBT delete 6 microseconds
    --------------------------------------------------------------------------------
    Red black tree detailed delete
    Node=   60 Left=   15 Right=  100 Parent=   -- Color=BLACK NodeCount=  6 Level=0
    Node=   15 Left=    5 Right=  NIL Parent=   60 Color=BLACK NodeCount=  2 Level=1
    Node=  100 Left=   80 Right=  NIL Parent=   60 Color=BLACK NodeCount=  3 Level=1
    Node=    5 Left=  NIL Right=  NIL Parent=   15 Color=  RED NodeCount=  1 Level=2
    Leaf=   -- Left=  STN Right=  STN Parent=   15 Color=BLACK NodeCount=  0 Level=2
    Node=   80 Left=  NIL Right=   85 Parent=  100 Color=  RED NodeCount=  1 Level=2
    Leaf=   -- Left=  STN Right=  STN Parent=  100 Color=BLACK NodeCount=  0 Level=2
    Leaf=   -- Left=  STN Right=  STN Parent=    5 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=    5 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=   80 Color=BLACK NodeCount=  0 Level=3
    Node=   85 Left=  NIL Right=  NIL Parent=   80 Color=  RED NodeCount=  1 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=   85 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   85 Color=BLACK NodeCount=  0 Level=4
    --------------------------------------------------------------------------------
    ---------------------------------------------------------------------------------------
    Red black tree delete
                     60
                     / \
                    /   \
                   /     \
                  /       \
                 /         \
                /           \
               /             \
              /               \
             /                 \
            /                   \
           15                  100
           / \                  / \
          /   \                /   \
         /     \              /     \
        /       \            /       \
       /         \          /         \
       5        Nil        80        Nil
      / \        / \       / \        / \
     /   \      /   \     /   \      /   \
    Nil  Nil    ST   ST  Nil   85    ST   ST
    / \  / \   / \  / \  / \  / \   / \  / \
    ---------------------------------------------------------------------------------------
    Time taken to complete LLRBT insert 13 microseconds
    --------------------------------------------------------------------------------
    Left-leaning red black tree insert detailed
    Node=   70 Left=   39 Right=   90 Parent=   -- Color=BLACK NodeCount= 14 Level=0
    Node=   39 Left=   20 Right=   50 Parent=   70 Color=  RED NodeCount=  9 Level=1
    Node=   90 Left=   85 Right=  100 Parent=   70 Color=BLACK NodeCount=  4 Level=1
    Node=   20 Left=   10 Right=   30 Parent=   39 Color=BLACK NodeCount=  5 Level=2
    Node=   50 Left=   40 Right=   60 Parent=   39 Color=BLACK NodeCount=  3 Level=2
    Node=   85 Left=   80 Right=  NIL Parent=   90 Color=BLACK NodeCount=  2 Level=2
    Node=  100 Left=  NIL Right=  NIL Parent=   90 Color=BLACK NodeCount=  1 Level=2
    Node=   10 Left=    5 Right=   15 Parent=   20 Color=  RED NodeCount=  3 Level=3
    Node=   30 Left=  NIL Right=  NIL Parent=   20 Color=BLACK NodeCount=  1 Level=3
    Node=   40 Left=  NIL Right=  NIL Parent=   50 Color=BLACK NodeCount=  1 Level=3
    Node=   60 Left=  NIL Right=  NIL Parent=   50 Color=BLACK NodeCount=  1 Level=3
    Node=   80 Left=  NIL Right=  NIL Parent=   85 Color=  RED NodeCount=  1 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=   85 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=  100 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=  100 Color=BLACK NodeCount=  0 Level=3
    Node=    5 Left=  NIL Right=  NIL Parent=   10 Color=BLACK NodeCount=  1 Level=4
    Node=   15 Left=  NIL Right=  NIL Parent=   10 Color=BLACK NodeCount=  1 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   30 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   30 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   40 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   40 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   60 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   60 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   80 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=   80 Color=BLACK NodeCount=  0 Level=4
    Leaf=   -- Left=  STN Right=  STN Parent=    5 Color=BLACK NodeCount=  0 Level=5
    Leaf=   -- Left=  STN Right=  STN Parent=    5 Color=BLACK NodeCount=  0 Level=5
    Leaf=   -- Left=  STN Right=  STN Parent=   15 Color=BLACK NodeCount=  0 Level=5
    Leaf=   -- Left=  STN Right=  STN Parent=   15 Color=BLACK NodeCount=  0 Level=5
    --------------------------------------------------------------------------------
    ---------------------------------------------------------------------------------------
    Left-leaning red black tree insert 
                                     70
                                     / \
                                    /   \
                                   /     \
                                  /       \
                                 /         \
                                /           \
                               /             \
                              /               \
                             /                 \
                            /                   \
                           /                     \
                          /                       \
                         /                         \
                        /                           \
                       /                             \
                      /                               \
                     /                                 \
                    /                                   \
                   39                                   90
                   / \                                  / \
                  /   \                                /   \
                 /     \                              /     \
                /       \                            /       \
               /         \                          /         \
              /           \                        /           \
             /             \                      /             \
            /               \                    /               \
           /                 \                  /                 \
          20                 50                85                100
          / \                / \               / \                / \
         /   \              /   \             /   \              /   \
        /     \            /     \           /     \            /     \
       /       \          /       \         /       \          /       \
      10       30        40       60       80      Nil       Nil      Nil
      / \      / \       / \      / \      / \      / \       / \      / \
     /   \    /   \     /   \    /   \    /   \    /   \     /   \    /   \
     5   15 Nil  Nil  Nil  Nil Nil  Nil Nil  Nil  ST   ST   ST   ST  ST   ST
    / \  / \ / \  / \  / \  / \ / \  / \ / \  / \ / \  / \  / \  / \ / \  / \
    ---------------------------------------------------------------------------------------
    Time taken to complete LLRBT delete 10 microseconds
    --------------------------------------------------------------------------------
    Left-leaning red black tree delete detailed
    Node=   80 Left=   15 Right=  100 Parent=   -- Color=BLACK NodeCount= 14 Level=0
    Node=   15 Left=    5 Right=   60 Parent=   80 Color=  RED NodeCount=  8 Level=1
    Node=  100 Left=   85 Right=  NIL Parent=   80 Color=BLACK NodeCount=  4 Level=1
    Node=    5 Left=  NIL Right=  NIL Parent=   15 Color=BLACK NodeCount=  1 Level=2
    Node=   60 Left=  NIL Right=  NIL Parent=   15 Color=BLACK NodeCount=  5 Level=2
    Node=   85 Left=  NIL Right=  NIL Parent=  100 Color=  RED NodeCount=  2 Level=2
    Leaf=   -- Left=  STN Right=  STN Parent=  100 Color=BLACK NodeCount=  0 Level=2
    Leaf=   -- Left=  STN Right=  STN Parent=    5 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=    5 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=   60 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=   60 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=   80 Color=BLACK NodeCount=  0 Level=3
    Leaf=   -- Left=  STN Right=  STN Parent=   85 Color=BLACK NodeCount=  0 Level=3
    --------------------------------------------------------------------------------
    ---------------------------------------------------------------------------------------
    Left-leaning red black tree delete
             80
             / \
            /   \
           /     \
          /       \
         /         \
        /           \
       15          100
       / \          / \
      /   \        /   \
     /     \      /     \
     5     60    85    Nil
    / \    / \   / \    / \
    ---------------------------------------------------------------------------------------
    