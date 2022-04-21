#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include<math.h>
#include "spi.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use fast frc oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // primary osc disabled
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1 // use largest wdt value ****NOTE: FLAG THIS LINE FOR REVIEW***
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz fast rc internal oscillator
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

unsigned short BitConverter(char channel, unsigned char v);

int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1;
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 0;

    initSPI();

    __builtin_enable_interrupts();
    
    unsigned short s = 0;    
    unsigned char sin_temp = 0; // Sin Function Temp Variable
    unsigned char tri_temp = 0; // Tri Function Temp Variable
    float i = 0; // Sin Function Real Variable
    float j = 0; // Tri Function Real Variable
    int t = 240000; // time
    
    _CP0_SET_COUNT(0);
    
    while (1) {
        
        // Sin and Tri Function Handler
        sin_temp = 1.65*sin(2*3.14*2*_CP0_GET_COUNT()/24000000)+1.65;
        tri_temp = 6.6*abs(((_CP0_GET_COUNT()-6000000) % 24000000)-12000000)/24000000;
        i = sin_temp*255/3.3;
        j = tri_temp*255/3.3;
        
        // write one byte over SPI1
        s = BitConverter(1,i); // get the byte
        LATAbits.LATA0 = 0; // bring CS low
        spi_io(s>>8); // write the byte
        spi_io(s); // write the byte
        LATAbits.LATA0 = 1; // bring CS high
        
        // write one byte over SPI1
        s = BitConverter(0,j); // get the byte
        LATAbits.LATA0 = 0; // bring CS low
        spi_io(s>>8); // write the byte
        spi_io(s); // write the byte
        LATAbits.LATA0 = 1; // bring CS high
        
        t=t+240000; 
        while (_CP0_GET_COUNT() < t) {} // wait 0.01 sec
    }
}

unsigned short BitConverter(char a, unsigned char b) {
    unsigned short c = 0;
    c = c | (a << 15);
    c = c | (0b111 << 12);
    c = c | (b << 4);
    return c;
}