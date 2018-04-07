/***************************************************************************************************
* @ author : Shuting Guo
* @ date   : 04/3/2018
* @ file   : main.c
*
* This is a example multitask program that uses the FreeRTOS on the TIVA Connected LaunchPad. The
* program creates two tasks that blink the LED0 and LED1 at the rate of 2Hz and 4Hz respectively.
* The timing is done by using a FreeRTOS software timer.
*
***************************************************************************************************/

/* Standard include */
#include <stdio.h>
#include <stdbool.h>

/* FreeRTOS kernel includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* TIVAWARE libraries include */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

TimerHandle_t xTimer;
SemaphoreHandle_t xSem_task1, xSem_task2;

static void task1(void*pvParameters);
static void task2(void*pvParameters);
void vTimerCallback (TimerHandle_t xTimer);
void utilToggleLED(int32_t led_no);

// Modified the startup code isrs

int main(void)
{
    /* Set up system clock to be 120MHz */
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                        SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    /* Enable the GPIO peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    /* Configure the GPIO for LED */
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);

    /* Create two semaphores for signaling two tasks */
    xSem_task1 = xSemaphoreCreateBinary();
    xSem_task2 = xSemaphoreCreateBinary();
    if((xSem_task1 == NULL) | (xSem_task2 == NULL))
    {
        /* Failed to create the semaphore(s), do error handling here */
    }

    /* Create two tasks*/
    if(xTaskCreate(task1, "task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS)
    {
        /* Failed to create task1, do error handling here */
    }

    if(xTaskCreate(task2, "task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS)
    {
        /* Failed to create task1, do error handling here */
    }

    /* Create a timer that times 2 seconds*/
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
        /* Wait for the semaphore to toggle the LED */
        if(xSemaphoreTake(xSem_task1, 10) == pdTRUE)
        {
            utilToggleLED(0);
        }
    }

}

static void task2(void *pvParameters)
{
    for( ; ; )
    {
        /* Wait for the semaphore to toggle the LED */
        if(xSemaphoreTake(xSem_task2, 10) == pdTRUE)
        {
            utilToggleLED(1);
        }
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
        /* Signal the semaphore for task 2 every 4 seconds */
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
