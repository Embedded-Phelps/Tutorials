#include "led.h"
#include <stdio.h>

int ledOn(char *ledAddr)
{
	FILE *pFile = NULL;
	if((pFile = fopen(ledAddr,"w")) == NULL)
		return -1;
	if(fwrite("1",1,1,pFile) < 0)
		return -1;
	if(fclose(pFile) != 0)
		return -1;
	return 0;
}


int ledOff(char *ledAddr)
{
	FILE *pFile = NULL;
	if((pFile = fopen(ledAddr,"w")) == NULL)
		return -1;
	if(fwrite("0",1,1,pFile) < 0)
		return -1;
	if(fclose(pFile) != 0)
		return -1;
	return 0;
}
