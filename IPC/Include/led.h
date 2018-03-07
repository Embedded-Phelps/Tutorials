#ifndef LED_H_
#define LED_H_

static char ledPath[] = {"/sys/class/leds/beaglebone:green:usr0/brightness"};

int ledOn(char *ledAddr);
int ledOff(char *ledAddr);

#endif /* LED_H_ */
