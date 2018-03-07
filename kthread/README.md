
This kernel module would create two kthreads that communicate with each other via a kfifo. The first thread would enqueue at an interval an info structure that contain information about the currently scheduled process and its previous process and next process; The second thread would dequeue the info structure from the fifo and print out the information.

To compile the program 
make
