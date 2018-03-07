#include <stdio.h>
#include "message.h"
//#include "led.h"

void process_msg(msg_t msg)
{
  switch (msg.msg_id)
  {
    case MSG_ID_LED:
      if (msg.msg_payload.led_ctl == LED_ON)
        printf("ledon\n");
        //ledON(ledPath);
      else if (msg.msg_payload.led_ctl == LED_OFF)
        //ledOff(ledPath);
        printf("ledoff\n");
      break;
    case MSG_ID_STRING:
      for (unsigned int i=0; i<msg.payload_len; i++)
        printf("%c", msg.msg_payload.string[i]);
      printf("\n");
      break;
    default:;
  }
}
