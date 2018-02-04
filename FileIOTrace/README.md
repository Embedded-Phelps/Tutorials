# Track System Calls and Library Calls with FIleIO

track.c is a program that:
- Print to standard out an string
- Create a file
- Modify the permission of the file to be read/write
- Open the file
- Write a charater to the file
- Close the file
- Open the file (in append mode)
- Dynamically allocate an array of memory
- Read and input string from the command line and write to the string
- Write the string to the file
- Flush the output
- Close the file
- Open the file (for reading)
- Read a single character (getc)
- Read a string of characters (fgets)
- Close the file
- Free the memory

### Build the test executable
	gcc -c track.c -o test

###Run the 'ltrace' command line application to collect the output of the system calls and library calls that were used to interact with the file:
	ltrace ./test

###Run the 'strace' command line application to collect the output of the system calls and library calls that were used to interact with th    e file:
	strace ./test

### Run 'perf' to monitor the performance statistics of the program:
	sudo perf stat ./test
