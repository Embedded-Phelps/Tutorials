

This is an example event-driven multitasking program that uses the FreeRTOS on the TIVA Connected LaunchPad.

The program creates three tasks. Task1 signals task3's TOGGLE_LED event notification at a 2Hz rate; Task2 signals task3's LOG_STRING event notification at a 4Hz with a string that shows the current task count. Task3 controls the UART and LEDs and waits for TaskNotify signals from the two other tasks.

This program runs on the TIVA Connected LaunchPad that requires TIVAWARE (including the driverlib), FreeRTOS v10.0.
