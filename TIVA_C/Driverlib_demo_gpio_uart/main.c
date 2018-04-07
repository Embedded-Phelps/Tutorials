/***************************************************************************************************
* @ author : Shuting Guo
* @ date   : 04/3/2018
* @ file   : main.c
*
* This is a example program that makes use of the TivaWare software (which includes the driverlib)
* to configure for the GPIO and UART modules of the TIVA Connected LaunchPad. The program blinks the
* LED0 at the rate of 2Hz and increments a blinking count, which is sent out through the UART.
*
***************************************************************************************************/

/* Standard includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* TIVAWARE libraries includes */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

/* Note
 * The stack size is increased to 1024 bytes to accommodate
 * the use of the sprintf function
 */

void serial_send(char * string);
void utilToggleLED(int32_t led_no);

int main(void)
{
    uint32_t ui32SysClkFreq;
    int32_t cnt = 0;
    char blinkcnt[100]={0};

    /* Set up system clock to be 120MHz */
    ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                         SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    /* Enable the UART0 and GPIO peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /* Configure the UART0 */
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, ui32SysClkFreq, 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    /* Configure the GPIO for LED */
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x00);

    serial_send("Project for: <SHUTING GUO> <April 4, 2018>");

    for( ; ; )
    {
        /* Toggle the LED0 every 1s, so the blinking rate is 2Hz*/
        utilToggleLED(0);
        SysCtlDelay(40000000);
        cnt++;
        if(cnt%2==0)
        {
            /* Report the blinking count */
            sprintf(blinkcnt, "LED blink count: %d", cnt/2);
            serial_send(blinkcnt);
        }
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

/* Function to toggle a led */
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

