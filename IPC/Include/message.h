#ifndef MESSAGE_H__
#define MESSAGE_H__

#define MAX_MSG (0x0F)
#define MAX_MSG_SIZE (50)
#define MSG_ID_STRING (0x00)
#define MSG_ID_LED  (0x01)

#define LED_ON  (0x01)
#define LED_OFF (0x00)

union payload
{
  char string[MAX_MSG_SIZE];
  int led_ctl;
};

typedef struct
{
  int msg_id;
  union payload msg_payload;
  unsigned int payload_len;
}msg_t;

void process_msg(msg_t msg);

#endif
