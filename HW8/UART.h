#ifndef UART__H__  /* Guard against multiple inclusion */
#define UART__H__

#include <stdio.h>
#include <xc.h>                     // processor SFR definitions
#include <sys/attribs.h>            // __ISR macro

#define NU32_SYS_FREQ 48000000ul    // 48 million Hz
#define NU32_DESIRED_BAUD 230400    // Baudrate for RS232

void UART1_INIT(void);
void NU32_ReadUART1(char * message, int maxLength);
void NU32_WriteUART1(const char * string);

#endif 