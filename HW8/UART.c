#include <proc/p32mx170f256b.h>
#include "UART.h"

void UART1_INIT(void) {
    __builtin_disable_interrupts();
    U1MODEbits.BRGH = 0; // set baud to NU32_DESIRED_BAUD
    U1BRG = ((NU32_SYS_FREQ / NU32_DESIRED_BAUD) / 16) - 1;

    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;

    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.UEN = 2;

    U1RXRbits.U1RXR = 0b0000; // Set A2 to U1RX
    RPB3Rbits.RPB3R = 0b0001; // Set B3 to U1TX

    U1MODEbits.ON = 1;

    __builtin_enable_interrupts();
}

void NU32_ReadUART1(char * message, int maxLength) {
    char data = 0;
    int complete = 0, num_bytes = 0;
    while (!complete) {
        if (U1STAbits.URXDA) { 
            data = U1RXREG;
            if ((data == '\n') || (data == '\r')) {
                complete = 1;
            } else {
                message[num_bytes] = data;
                ++num_bytes;
                if (num_bytes >= maxLength) {
                    num_bytes = 0;
                }
            }
        }
    }
    message[num_bytes] = '\0';
}

void NU32_WriteUART1(const char * string) {
    while (*string != '\0') {
        while (U1STAbits.UTXBF) {
            ;
        }
        U1TXREG = *string;
        ++string;
    }
}

