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
#include <libpic30.h>
#include "bsp/buttons.h"
#include "bsp/adc.h"

#define FCY 4000000UL
#define LCD_E LATDbits.LATD4
#define LCD_RW LATDbits.LATD5
#define LCD_RS LATBbits.LATB15
#define LCD_DATA LATE

#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ON 0x0C
#define LCD_OFF 0x08
#define LCD_CONFIG 0x38
#define LCD_CURSOR 0x80
#define LINE1 0x00
#define LINE2 0x40
#define LCD_CUST_CHAR 0x40
#define LCD_SHIFT_R 0x1D
#define LCD_SHIFT_L 0x1B

void __delay_us(unsigned long us) {
    __delay32(us * FCY / 1000000);
}

void __delay_ms(unsigned long ms) {
    __delay32(ms * FCY / 1000);
}

void LCD_sendCommand(unsigned char command) {
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_E = 1;
    LCD_DATA = command;
    __delay_us(50);
    LCD_E = 0;
}

void LCD_sendData(unsigned char data) {
    LCD_RW = 0;
    LCD_RS = 1;
    LCD_E = 1;
    LCD_DATA = data;
    __delay_us(50);
    LCD_E = 0;
}

void LCD_print(unsigned char* string) {
    while (*string) {
        LCD_sendData(*string++);
    }
}

void LCD_setCursor(unsigned char row, unsigned char col) {
    unsigned char address;
    if (row == 1) {
        address = LCD_CURSOR + LINE1 + col;
    } else if (row == 2) {
        address = LCD_CURSOR + LINE2 + col;
    }
    LCD_sendCommand(address);
}

void LCD_saveCustChar(unsigned char slot, unsigned char *array) {
    unsigned char i;
    LCD_sendCommand(LCD_CUST_CHAR + (slot * 8));
    for (i = 0; i < 8; i++) {
        LCD_sendData(array[i]);
    }
}

void LCD_displayCharacter(unsigned char characterCode) {
    LCD_sendData(characterCode);
}

void LCD_init() {
    __delay_ms(20);
    LCD_sendCommand(LCD_CONFIG);
    __delay_us(50);
    LCD_sendCommand(LCD_ON);
    __delay_us(50);
    LCD_sendCommand(LCD_ON);
    __delay_ms(2);
}

void intToStr(unsigned int num, char* str, unsigned int width) {
    str[width] = '\0';  // zakończenie ciągu znaków
    for (int i = width - 1; i >= 0; i--) {
        str[i] = (num % 10) + '0';  // konwertowanie liczby na znak
        num /= 10;
    }
}

void displayTime(unsigned int time, unsigned char row, unsigned char col) {
    char timeStr[6];
    unsigned int minutes = time / 60;
    unsigned int seconds = time % 60;
    intToStr(minutes, timeStr, 2);
    timeStr[2] = ':';
    intToStr(seconds, timeStr + 3, 2);
    LCD_setCursor(row, col);
    LCD_print(timeStr);
}

void chessClock() {
    unsigned int time1 = 300; // 5 minutes for player 1
    unsigned int time2 = 300; // 5 minutes for player 2
    unsigned char currentPlayer = 1;

    LCD_setCursor(1, 0);
    LCD_print("P1:");
    displayTime(time1, 1, 3);

    LCD_setCursor(2, 0);
    LCD_print("P2:");
    displayTime(time2, 2, 3);

    while (1) {
        if (currentPlayer == 1) {
            if (time1 == 0) {
                LCD_sendCommand(LCD_CLEAR);
                LCD_setCursor(1, 0);
                LCD_print("Player 1 loses");
                break;
            }
            time1--;
            displayTime(time1, 1, 3);
        } else {
            if (time2 == 0) {
                LCD_sendCommand(LCD_CLEAR);
                LCD_setCursor(1, 0);
                LCD_print("Player 2 loses");
                break;
            }
            time2--;
            displayTime(time2, 2, 3);
        }

        if (BUTTON_IsPressed(BUTTON_S3)) {
            __delay_ms(300); // debouncing delay
            while (BUTTON_IsPressed(BUTTON_S3)); // wait until button is released
            currentPlayer = 3 - currentPlayer; // toggle between 1 and 2
        }

        if (BUTTON_IsPressed(BUTTON_S4)) {
            LCD_sendCommand(LCD_CLEAR);
            LCD_setCursor(1, 0);
            LCD_print("Game Over");
            break;
        }

        __delay_ms(1000);
    }
}

int main(void) {
    TRISB = 0x7FFF;
    TRISD = 0x0000;
    TRISE = 0x0000;

    LCD_init();

    while (1) {
        chessClock();
    }

    return 0;
}

