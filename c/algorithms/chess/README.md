    g++ -O2 -o check check_the_check.c && g++ -DDEBUG -g2 -o check_debug check_the_check.c

    Program to check if king is under attack
    Game 0: 
    Piece [k] is black
    black king [attacker = B : diagonal attack @ 0:2] is in check
    black king [attacker = B : diagonal attack @ 0:2] [under NO attack @ 0:3] is in NO checkmate situation
    Piece [K] is white
    white king [under NO attack @ 7:0] is in NO check
    white king [under NO attack @ 7:0] is in NO checkmate situation
    
     . . k . . . . .
     p p p . p p p p
     . . . . . . . .
     . R . . . B . .
     . . . . . . . .
     . . . . . . . .
     P P P P P P P P
     K . . . . . . .
    
    
    Game 1: 
    Piece [k] is black
    black king [under NO attack @ 0:4] is in NO check
    black king [under NO attack @ 0:4] is in NO checkmate situation
    Piece [K] is white
    white king [attacker = b : diagonal attack @ 7:4] is in check
    white king [attacker = b : diagonal attack @ 7:4] [attacker = b : diagonal attack @ 6:3] is in checkmate
    
     r n b q k . n r
     p p p . . p p p
     . . . . p . . .
     . . . p . . . .
     . b P P . . . .
     . . . . . N . .
     P P . . P P P P
     R N B Q K B . R
    
    Game 2: 
    Piece [k] is black
    black king [attacker = B : diagonal attack @ 0:2] is in check
    black king [attacker = B : diagonal attack @ 0:2] [attacker = R : stright attack @ 0:3] [attacker = Q : stright attack @ 0:1] [attacker = N : circle attack @ 1:2] [attacker = Q : stright attack @ 1:1] [attacker = R : stright attack @ 1:3] is in checkmate
    Piece [K] is white
    white king [under NO attack @ 7:0] is in NO check
    white king [under NO attack @ 7:0] is in NO checkmate situation
    
     . . k . . . . .
     p . . . p p p p
     N . . . . . . .
     . . . . . B . .
     . Q . . . . . .
     . . . . . . . .
     P P P R P P P P
     K . . . . . . .
    
    
    Game 3: 
    Piece [k] is black
    black king [under NO attack @ 0:4] is in NO check
    black king [under NO attack @ 0:4] is in NO checkmate situation
    Piece [K] is white
    white king [attacker = q : diagonal attack @ 7:4] is in check
    white king [attacker = q : diagonal attack @ 7:4] [attacker = q : diagonal attack @ 6:5] is in checkmate
    
     r n b . k b n r
     p p p p . p p p
     . . . . . . . .
     . . . . p . . .
     . . . . . . P q
     . . . . . P . .
     P P P P P . . P
     R N B Q K B N R
    
    Game 4: 
    Piece [k] is black
    black king [attacker = Q : diagonal attack @ 0:4] is in check
    black king [attacker = Q : diagonal attack @ 0:4] [attacker = Q : stright attack @ 0:5] [attacker = Q : stright attack @ 1:4] [attacker = B : diagonal attack @ 1:5] is in checkmate
    Piece [K] is white
    white king [under NO attack @ 7:4] is in NO check
    white king [under NO attack @ 7:4] is in NO checkmate situation
    
     r n b q k . . r
     p p p p . Q p p
     . . . . . n . .
     . . b . p . . .
     . . B . P . . .
     . . . . . . . .
     P P P P . P P P
     R N B . K . N R
    
    
    Game 5: 
    Piece [k] is black
    black king [attacker = Q : diagonal attack @ 0:4] is in check
    black king [attacker = Q : diagonal attack @ 0:4] [attacker = Q : stright attack @ 0:5] [attacker = Q : stright attack @ 1:4] [under NO attack @ 1:5] is in NO checkmate situation
    Piece [K] is white
    white king [under NO attack @ 7:4] is in NO check
    white king [under NO attack @ 7:4] is in NO checkmate situation
    
     r n b q k . . r
     p p p p . Q p p
     . . . . . n . .
     . . b . p . . .
     . . . . P . . .
     . . . . . . . .
     P P P P . P P P
     R N B . K B N R
     
    Game 6: 
    Piece [k] is black
    black king [under NO attack @ 0:4] is in NO check
    black king [under NO attack @ 0:4] is in NO checkmate situation
    Piece [K] is white
    white king [attacker = n : circle attack @ 7:4] is in check
    white king [attacker = n : circle attack @ 7:4] is in checkmate
    
     r . b q k b n r
     p p p p . p p p
     . . . . . . . .
     . . . . p . . .
     . . . . P . . .
     . . N . . n P .
     P P P P N P . P
     R . B Q K B . R
    
    
    Game 7: 
    Piece [k] is black
    black king [under NO attack @ 0:4] is in NO check
    black king [under NO attack @ 0:4] is in NO checkmate situation
    Piece [K] is white
    white king [attacker = n : circle attack @ 7:4] is in check
    white king [attacker = n : circle attack @ 7:4] [attacker = n : circle attack @ 6:3] is in checkmate
    
     r . b . k b n r
     p p p p . p p p
     . . . . . . . .
     . . . . p . B .
     . . . . P . . .
     . . N . . n P .
     P P P . N P . P
     R . . Q K B . R
     
    Game 8: 
    Piece [k] is black
    black king [under NO attack @ 0:4] is in NO check
    black king [under NO attack @ 0:4] is in NO checkmate situation
    Piece [K] is white
    white king [attacker = n : circle attack @ 7:4] is in check
    white king [attacker = n : circle attack @ 7:4] is in checkmate
    
     r . b . k b n r
     p p p p . N p p
     . . . . . . . .
     . . . . . . . .
     . . . . q . . .
     . . . . . n . .
     P P P P B P . P
     R N B Q K R . .
    
    
    Game 9: 
    Piece [k] is black
    black king [under NO attack @ 0:4] is in NO check
    black king [under NO attack @ 0:4] is in NO checkmate situation
    Piece [K] is white
    white king [under NO attack @ 7:4] is in NO check
    white king [under NO attack @ 7:4] is in NO checkmate situation
    
     r n b q k b n r
     p p p p p p p p
     . . . . . . . .
     . . . . . . . .
     . . . . . . . .
     . . . . . . . .
     P P P P P P P P
     R N B Q K B N R

