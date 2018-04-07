/***************************************************************************************************
* @ author : Shuting Guo
* @ date   : 04/3/2018
* @ file   : main.c
*
* This is an example event-driven multitasking program that uses the FreeRTOS on the TIVA Connected LaunchPad.
* The program creates three tasks. Task1 signals task3's TOGGLE_LED event notification at a 2Hz rate;
* Task2 signals task3's LOG_STRING event notification at a 4Hz with a string that shows the current
* task count. Task3 controls the UART and LEDs and waits for TaskNotify signals from the two other
* tasks.
*
***************************************************************************************************/
/* Standard include */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* FreeRTOS kernel includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"

/* TIVAWARE libraries include */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

#define TOGGLE_LED  (0x01)
#define LOG_STRING  (0x02)

static TimerHandle_t xTimer;
static SemaphoreHandle_t xSem_task1, xSem_task2;
static TaskHandle_t xTask1 = NULL, xTask2 = NULL, xTask3 = NULL;
static QueueHandle_t xQueue = NULL;

static uint8_t task_event = 0;

static char string[100]={0};

static void task1(void*pvParameters);
static void task2(void*pvParameters);
static void task3(void*pvParameters);
void process_event(void);
void vTimerCallback (TimerHandle_t xTimer);
void utilToggleLED(int32_t led_no);
void serial_send(char * string);

// Modified the startup code isrs

int main(void)
{
    uint32_t ui32SysClkFreq;
    /* Set up system clock to be 120MHz */
    ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                         SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                         120000000);

    /* Enable the UART0 and GPIO peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /* Configure the GPIO for LED */
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);

    /* Configure the UART0 */
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, ui32SysClkFreq, 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    /* Create two semaphores for signaling two tasks */
    xSem_task1 = xSemaphoreCreateBinary();
    xSem_task2 = xSemaphoreCreateBinary();
    if((xSem_task1 == NULL) | (xSem_task2 == NULL))
    {
        /* Failed to create the semaphore(s), do error handling here */
    }

    /* Create a message queue */
    xQueue  = xQueueCreate(1, sizeof(int32_t));
    if(xQueue == NULL)
    {
        /* Failed to create the queue, do error handling here */
    }

    /* Create two tasks*/
    if(xTaskCreate(task1, "task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &xTask1) != pdPASS)
    {
        /* Failed to create task1, do error handling here */
    }
    if(xTaskCreate(task2, "task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &xTask2) != pdPASS)
    {
        /* Failed to create task2, do error handling here */
    }
    if(xTaskCreate(task3, "task3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &xTask3) != pdPASS)
    {
        /* Failed to create task3, do error handling here */
    }

    /* Create a timer */
    xTimer = xTimerCreate("Timer", pdMS_TO_TICKS(2000), pdTRUE, (void*)0, vTimerCallback);
    if(xTimer == NULL)
    {
        /* Failed to create the timer, do error handling here */
    }
    if(xTimerStart(xTimer, 0) != pdPASS)
    {
        /* Failed to start the timer, do error handling here */
    }

    /* Start the scheduler */
    vTaskStartScheduler();

    for( ; ; );
}

static void task1(void*pvParameters)
{
    for( ; ; )
    {

        if(xSemaphoreTake(xSem_task1, 10)==pdTRUE)
        {
            /* Set the TOGGLE LED event notification */
            task_event |= TOGGLE_LED;
            /* Signal task3 */
            xTaskNotifyGive(xTask3);
        }
    }

}

static void task2(void *pvParameters)
{
    TickType_t current_tick;
    int32_t addr = (int32_t)string;
    for( ; ; )
    {
        if(xSemaphoreTake(xSem_task2, 10)==pdTRUE)
        {
            /* Get current task tick count */
            current_tick = xTaskGetTickCount();
            /* Set the LOG_STRING event notification */
            task_event |= LOG_STRING;
            /* Prepare the string and send the address of the string head onto the queue */
            sprintf(string, "Current task tick count: %u", current_tick);
            xQueueSend(xQueue, &addr, 0);
            /* Signal task3 */
            xTaskNotifyGive(xTask3);
        }
    }
}

static void task3(void* pvParameters)
{
    for( ; ; )
    {
        /* Wait for event signal from two other tasks */
        ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        /* Process the event */
        process_event();
    }
}

void process_event(void)
{
    int32_t p_rx;
    if(task_event & TOGGLE_LED)
    {
        /* Clear the event notification */
        task_event &= ~TOGGLE_LED;
        /* Toggle LED */
        utilToggleLED(0);
    }
    else if(task_event & LOG_STRING)
    {
        /* Clear the event notification */
        task_event &= ~LOG_STRING;
        /* Receive the string head address from the queue */
        xQueueReceive(xQueue, &p_rx, portMAX_DELAY);
        /* Send out the stirng via UART */
        serial_send((char*)p_rx);
    }
}

void vTimerCallback (TimerHandle_t xTimer)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t cnt=0;

    cnt ++;
    /* Signal the semaphore for task 1 every 2 seconds*/
    xSemaphoreGiveFromISR(xSem_task1, &xHigherPriorityTaskWoken);
    if(cnt == 2)
    {
        cnt = 0;
        /* Signal the semaphore for task 2 every 4 seconds*/
        xSemaphoreGiveFromISR(xSem_task2, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* Function to toggle the LEDs */
void utilToggleLED(int32_t led_no)
{
    switch(led_no)
    {
    case 0:
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,
        (uint8_t)GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_1) ^ 0x02);
        break;
    case 1:
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0,
        (uint8_t)GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_0) ^ 0x01);
        break;
    }
}

/* Function to send out a string via UART */
void serial_send(char * string)
{
    int i;
    i = strlen(string);
    for(i = 0; i < strlen(string); i++)
    {
        while(!UARTCharPutNonBlocking(UART0_BASE, string[i]));
    }
    while(!UARTCharPutNonBlocking(UART0_BASE, '\n'));
}
