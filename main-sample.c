// PIC24FJ128GA010 Configuration Bit Settings
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

#include <xc.h>
#include "bsp/adc.h"
#include "bsp/buttons.h"
#include <libpic30.h>

int shift = 0;

void sleep(unsigned int time) {
    __delay32(1500000);
}

int main(void) {
    unsigned char portValue = 0x53;
    int alarm = 0;
    int flaga = 1;

    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);

    TRISA = 0x0000;

    unsigned int value;

    while (1) {
        // Odczyt warto?ci potencjometru
        value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);

        // Sprawdzenie poprawno?ci odczytu
        if (value == 0xFFFF) {
            continue;
        }

        // Normalizacja warto?ci do 8 bitów
        unsigned char normValue = value >> 2;

        LATA = normValue;
        // Sprawdzenie progu alarmowego
        if (normValue >= 128) {
            alarm = 1;
            
                  }

        // Obs?uga alarmu
        while (alarm == 1) {
            // Sprawdzenie przycisku do wy??czenia alarmu
            if (BUTTON_IsPressed(BUTTON_S3) == true) {
                alarm = 0;
                flaga = 0;
                break;
            }
            if (normValue< 128) {
                alarm=0;
            }

            // Miganie diod? LED
            for (int i = 0; i < 50; i++) {
                LATA = 249;
                __delay32(10000);
                LATA = 0;
            }

            // Wy?wietlenie warto?ci na porcie LATA
            LATA = normValue;
        }
    }
    return 0;
}
