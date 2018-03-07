Two processes communicate with each other via a named pipe. Children process sends out toggled led control messages and receive string messages from parent process; parent process reads the led control message and send out a string to the children process.

To compile the program:
gcc -I../Include pipe.c ../message.c -o out -lrt
