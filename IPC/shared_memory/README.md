Two processes communicate with each other via shared memory. Children process sends out toggled led control messages and receive string messages from parent process; parent process reads the led control message and send out a string to the children process.

To compile the program:
gcc -I../include sharedmem.c ../message.c -o out -pthread -lrt
