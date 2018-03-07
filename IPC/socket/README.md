The client would send out LED_ON/LED_OFF command to the server and the server would react to the command accordingly and send back the LED status in a string format.

To compile the server program:
gcc -I../Include socket_server.c ../message.c ../led.c -o out -lrt
