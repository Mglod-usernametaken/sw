// PIC24FJ128GA010 Configuration Bit Settings
// For more on Configuration Bits, see Section 1.1
// consult your device data sheet
// CONFIG2
#pragma config POSCMOD = XT // XT Oscillator mode selected
#pragma config OSCIOFNC = ON // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = PRI // Primary Oscillator (XT, HS, EC)
#pragma config IESO = ON // Int Ext Switch Over Mode enabled
// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (1:128)
#pragma config WINDIS = ON // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF // Watchdog Timer disabled
#pragma config ICS = PGx2 // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF // Writes to program memory allowed
#pragma config GCP = OFF // Code protection is disabled
#pragma config JTAGEN = OFF // JTAG port is disabled
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



//----------------------------------------------------------------------

#include <xc.h>
#include "bsp/buttons.h"
#include <libpic30.h>

int TIME = 32;
int shift;
shift = 0;


void sleep(unsigned int time){
    //for(uns:gned int i=0;i<(time*1000);i++){
    //  i == i;
        __delay32(1000000);
    }
//----------------------------------------------------------------------

void zlicz_gora(void) {
        static int zliczg = 0;
        zliczg++;
        if (zliczg>255){
            zliczg = 0;
        }
        LATA = zliczg;
        sleep(TIME);
}

//----------------------------------------------------------------------

void zlicz_dol(void) {
        static int zliczd = 255;
        zliczd--;
        if (zliczd<0){
            zliczd = 255;
        }
        LATA = zliczd;
        sleep(TIME);
}
//----------------------------------------------------------------------

void zlicz_gray_dol(void) {
        static int zliczd = 255;
        zliczd--;
        if (zliczd<0){
            zliczd = 255;
        }
        LATA = zliczd ^ (zliczd >> 1);
        sleep(TIME);
}
//----------------------------------------------------------------------


void zlicz_gray_gora(void) {
        static int zliczg = 0;
        zliczg++;
        if (zliczg>255){
            zliczg = 0;
        }
        LATA = zliczg ^ (zliczg >> 1);
        sleep(TIME);
}
//----------------------------------------------------------------------

struct BCD_Counter {
    unsigned int cyfra1 : 4; // Pierwsza cyfra BCD
    unsigned int cyfra10 : 4; // Druga cyfra BCD
};

// Funkcja inkrementuj?ca licznik BCD
void inkrementujBCD(struct BCD_Counter *licznik) {
    if (licznik->cyfra1 == 9) {
        licznik->cyfra1 = 0;
        if (licznik->cyfra10 == 9) {
            licznik->cyfra10 = 0;
        } else {
            licznik->cyfra10++;
        }
    } else {
        licznik->cyfra1++;
    }
}

//----------------------------------------------------------------------

int main(void) {
unsigned char portValue = 0x53;
// unsigned char start;

struct BCD_Counter licznik = {0, 0};

AD1PCFG = 0xFFFF;
TRISA = 0x0000;

//start = 0b00000001;

while(1){
    if (BUTTON_IsPressed (BUTTON_S3) == true) {
        shift = shift + 1;
        if (shift > 4){
            shift = 0;
        }
    __delay32(1000000);
    } else if ((BUTTON_IsPressed (BUTTON_S4) == true)) {
        shift = shift - 1;
        if (shift < 0){
            shift = 4;
        }
    __delay32(1000000);
    }
    // portValue = start << shift;
    // LATA = portValue;
    if (shift == 0){
        zlicz_gora();
    } else if (shift == 1){
        zlicz_dol();
    } else if (shift == 2){
        zlicz_gray_gora();
    } else if (shift == 3){
        zlicz_gray_dol();
    } else if (shift == 4){
        inkrementujBCD();
    } else{
        __delay32(100);
    }


}
return 0;
}

