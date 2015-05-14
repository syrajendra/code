Information Retrieval
=====================

1.1 An example information retrieval problem
 
	$ gcc -o terms build_terms.c -lrt
	$ ./terms doc1.txt doc2.txt
	Number of tokens in file [doc1.txt]: 13 
	Number of tokens in file [doc2.txt]: 14 
	
	Unique Terms: 19
	-------------
	be
	brutus
	caesar
	capitol
	did
	enact
	hath
	i
	it
	julius
	killed
	let
	noble
	so
	the
	told
	was
	with
	you
	
	Time taken to complete execution 274 microseconds 

1.2 A ﬁrst take at building an inverted index 
 
	$ gcc -o inv_index inverted_index.c -lrt
	$ ./inv_index doc1.txt doc2.txt
	Number of tokens in file [doc1.txt - 1]: 14
	Number of tokens in file [doc2.txt - 2]: 15
	[ambitious][1]  ---> [2]
	[be][1]  ---> [2]
	[brutus][2]  ---> [1] ---> [2]
	[caesar][3]  ---> [1] ---> [2]
	[capitol][1]  ---> [1]
	[did][1]  ---> [1]
	[enact][1]  ---> [1]
	[hath][1]  ---> [2]
	[i][3]  ---> [1]
	[it][1]  ---> [2]
	[julius][1]  ---> [1]
	[killed][2]  ---> [1]
	[let][1]  ---> [2]
	[me][1]  ---> [1]
	[noble][1]  ---> [2]
	[so][1]  ---> [2]
	[the][2]  ---> [1] ---> [2]
	[told][1]  ---> [2]
	[was][2]  ---> [1] ---> [2]
	[with][1]  ---> [2]
	[you][1]  ---> [2]

	Time taken to complete execution 272 microseconds
   
Exercise 1.1
 
 	$ ./inv_index doc1.txt doc2.txt doc3.txt doc4.txt 
  	Number of tokens in file [doc1.txt - 1]: 5 
	Number of tokens in file [doc2.txt - 2]: 5 
	Number of tokens in file [doc3.txt - 3]: 6 
	Number of tokens in file [doc4.txt - 4]: 5 
	[forecasts][1]  ---> [1]
	[home][4]  ---> [1] ---> [2] ---> [3] ---> [4]
	[in][3]  ---> [2] ---> [3]
	[increase][1]  ---> [3]
	[july][3]  ---> [2] ---> [3] ---> [4]
	[new][2]  ---> [1] ---> [4]
	[rise][2]  ---> [2] ---> [4]
	[sales][4]  ---> [1] ---> [2] ---> [3] ---> [4]
	[top][1]  ---> [1]

	Time taken to complete execution 305 microseconds
	
Exercise 1.2
 
 	./inv_index doc1.txt doc2.txt doc3.txt doc4.txt 
	Number of tokens in file [doc1.txt - 1]: 4 
	Number of tokens in file [doc2.txt - 2]: 3 
	Number of tokens in file [doc3.txt - 3]: 6 
	Number of tokens in file [doc4.txt - 4]: 5 
	[approach][1]  ---> [3]
	[breakthrough][1]  ---> [1]
	[drug][2]  ---> [1] ---> [2]
	[for][3]  ---> [1] ---> [3] ---> [4]
	[hopes][1]  ---> [4]
	[new][3]  ---> [2] ---> [3] ---> [4]
	[of][1]  ---> [3]
	[patients][1]  ---> [4]
	[schizophrenia][4]  ---> [1] ---> [2] ---> [3] ---> [4]
	[treatment][1]  ---> [3]

	Time taken to complete execution 293 microseconds

1.3 Processing Boolean queries

		
Reference:
---------
http://nlp.stanford.edu/IR-book/html/htmledition/a-first-take-at-building-an-inverted-index-1.html

   Or
   
http://nlp.stanford.edu/IR-book/pdf/irbookprint.pdf
