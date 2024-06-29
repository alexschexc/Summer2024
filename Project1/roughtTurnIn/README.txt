Instructions for compiling and running code:

- place all files in a directory together.
- move to that directory from the command line.
- run the make utility.
- from the CLI run ./ACS200009PrefixSum:
	+ the code will request an input text file, this file should only contain numbers, and those numbers should entered on their own line. A sample file "hello7" has been provided.
	+ the code will request an output text file. For grader convenience I have provided output.txt, the program can however generate an output file by itself.
	+ the code will request the exact number of array elements. Entering the incorrect number of elements will lead to errors. hello7 has 1,000,000 (1 million) elements. Do not enter the element number with any delimiters, if there are 1 million elements, the accepted entry is 1000000
	+ the code will request a desired number of cores. this number must be an integer.


!!! this version of the code does not cover the case of cores unevenly dividing the number array elements !!!
