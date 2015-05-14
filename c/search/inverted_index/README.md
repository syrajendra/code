1.3 Processing Boolean queries
 
	$ gcc -Wall -O2 -o query query_inverted_index.c -lrt
	$ ./query ../sample/doc1.txt ../sample/doc2.txt ../sample/doc3.txt ../sample/doc4.txt
	Number of tokens in file [../sample/doc1.txt - 1]: 15 
	Number of tokens in file [../sample/doc2.txt - 2]: 15 
	Number of tokens in file [../sample/doc3.txt - 3]: 5 
	Number of tokens in file [../sample/doc4.txt - 4]: 8 
	
	Inverted index: 
	[ambitious][1]  ---> [2]
	[be][1]  ---> [2]
	[brutus][4]  ---> [1] ---> [2] ---> [3] ---> [4]
	[caesar][4]  ---> [1] ---> [2] ---> [4]
	[calpurnia][2]  ---> [1] ---> [4]
	[capitol][2]  ---> [1] ---> [3]
	[did][1]  ---> [1]
	[enact][1]  ---> [1]
	[hath][2]  ---> [2] ---> [4]
	[i][3]  ---> [1]
	[it][1]  ---> [2]
	[julius][1]  ---> [1]
	[killed][3]  ---> [1] ---> [3]
	[let][1]  ---> [2]
	[me][2]  ---> [1] ---> [3]
	[noble][2]  ---> [2] ---> [4]
	[so][1]  ---> [2]
	[the][4]  ---> [1] ---> [2] ---> [3] ---> [4]
	[told][2]  ---> [2] ---> [4]
	[was][2]  ---> [1] ---> [2]
	[with][1]  ---> [2]
	[you][2]  ---> [2] ---> [4]
	
	Query : Brutus AND Caesar 
	Result: Posting list: [ 2  4 ]
	
	Query : I AND did 
	Result: Posting list: [ 1 ]
	
	Query : LeT AND Me 
	Result: Posting list: []
	
	Query:  Brutus  AND  Caesar  AND  Calpurnia  AND  Julius 
	Sorting by frequency: 
	julius: Posting list: [ 1 ]
	calpurnia: Posting list: [ 1  4 ]
	caesar: Posting list: [ 1  2  4 ]
	brutus: Posting list: [ 1  2  3  4 ]
	Result: Posting list: [ 1 ]
	
	Time taken to complete execution 246 microseconds
