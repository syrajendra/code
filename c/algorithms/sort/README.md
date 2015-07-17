Sorting Algorithms
==================

Stable sort: retain the original ordering of keys when identical keys are present in the input data
			 i.e. sort algorithm will leave identical keys in the same relative position in the sorted output

In-place sort: sort the array in-place, no extra memory is required


			 		
    Algorithm 		 	Worst-case		Average-case 		Best-case		Space		Stable		In-place 	Explanation
    --------------------------------------------------------------------------------------------------------------------------
    Insertion sort |		n^2  			n^2					n 			 -			 Yes		  Yes
    Gnome sort 	   |		n^2  			n^2					n 			 -			 Yes		  Yes	
    Selection sort |		n^2  			n^2					n^2          -  		 No 		  Yes
    Shell sort 	   |    	n^1.5 			n^1.25 				n^1.25  	 - 			 No 		  Yes
    Bubble sort    | 		n^2 			n^2 				n 			 -  		 Yes		  Yes
    Merge sort 	   |		n log n  		n log n 			n log n 	 n 			 Yes 		  No
    Heap sort 	   |		n^2 	  		n log n 			n log n 	 - 			 No  		  Yes
    Quick sort 	   |		n^2 			n log n 	 		n log n      log n 		 No 		  Yes
    3Way-Quick sort|		n^2 			n log n 	 		n		     log n 		 No 		  Yes
    Counting sort  |		k + 2n 			k + 2n 				k + 2n 		 n + k 		 Yes   		  No 		k = max number
    LSD Radix sort |		d(2n + k) 		d(2n + k)			d(2n + k)	 n + k 		 Yes 		  No 		d = max order k = 10 (0-9) 
    MSD Radix sort |																	 No
    Bucket sort    |		n^2 			n 					n 			 n 			 Yes          Yes

    
    Sorting strings:
    ----------------

    Algorithm 		 	 Worst-case		 Average-case 		Best-case		Space		Stable		in-place 	Explanation
    --------------------------------------------------------------------------------------------------------------------------------
    LSF sorted	   |		n * W  			n * W			 n * W 			 n			 Yes		  No	    W = Width of string
    			   |																							Works with strings of same width
    MSF sorted	   |		n * W			n * W				n			(n + W)R	  No		  No	    W = Max width of string
    																											R = Radix (ASCII=256 UNICODE=65536)

    Example:

    Speed of machine = 10 billion instructions per second = 10, 000, 000, 000 = 10 ^ 10
    input n          = 10 million                         = 10, 000, 000      = 10 ^ 7

    1. Time to complete insertion sort : n ^ 2  / number of instruction per second 

    	( 10 ^ 7 ) ^ 2 instructions
      ----------------------------------- = 20, 000 seconds = 20, 000 / 60 = 333.33 minutes = 333.33 / 60 = 5.5 hours
        ( 10 ^ 10 ) instructions / second

    2. Time to complete merge sort : n log2(n) / number of instruction per second
	
		( 10 ^ 7 ) * log2 (10 ^ 7) instructions       (10 ^ 7) * 23.25
      ------------------------------------------  =  ----------------    =   0.02325 seconds 
        ( 10 ^ 10 ) instructions / second    			(10 ^ 10)



    