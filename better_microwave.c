// PIC24FJ128GA010 Configuration Bit Settings
// CONFIG2
#pragma config POSCMOD = XT   // XT Oscillator mode selected
#pragma config OSCIOFNC = ON  // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = PRI    // Primary Oscillator (XT, HS, EC)
#pragma config IESO = ON      // Int Ext Switch Over Mode enabled
// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128   // WDT Prescaler (1:128)
#pragma config WINDIS = ON     // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF    // Watchdog Timer disabled
#pragma config ICS = PGx2      // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF      // Writes to program memory allowed
#pragma config GCP = OFF       // Code protection is disabled
#pragma config JTAGEN = OFF    // JTAG port is disabled


#include "bsp/adc.h"
#include "bsp/buttons.h"
#include <libpic30.h>
#include <xc.h>
#include <stdio.h>
#include <string.h>


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

void __delay_us(unsigned long us) { __delay32(us * FCY / 1000000); }
void __delay_ms(unsigned long us) { __delay32(us * FCY / 1000); }

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

void LCD_print(unsigned char *string) {
  while (*string) {
    LCD_sendData(*string++);
  }
}

void LCD_setCursor(unsigned char row, unsigned char col) {
  unsigned char address;
  if (row == 1) {
    address = LCD_CURSOR + LINE1 + col;
  } else {
    address = LCD_CURSOR + LINE2 + col;
  }
  LCD_sendCommand(address);
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

void LCD_clear() {
  LCD_sendCommand(LCD_CLEAR);
}

void wyswietlaj(unsigned int time_left, unsigned char current_power) {
  char bufor_wyswietlacza[16]; 
  unsigned char sec = time_left % 60;
  unsigned char min = time_left / 60; 
  sprintf(bufor_wyswietlacza, "%02u:%02u MOC: %u", min, sec, current_power);
  LCD_clear();
  LCD_print((unsigned char *)bufor_wyswietlacza);
}

void buttonierka(unsigned int *time_left, unsigned char *current_power, unsigned char *aktywny) {
  if (BUTTON_IsPressed(BUTTON_S3) && *current_power < 5) {
    (*current_power)++;
    while (BUTTON_IsPressed(BUTTON_S3)); 
  }
  if (BUTTON_IsPressed(BUTTON_S4) && !*aktywny && *time_left < 5999) {
    *time_left += 30;
    while (BUTTON_IsPressed(BUTTON_S4)); 
  }
  if (BUTTON_IsPressed(BUTTON_S6)) {
    *aktywny = !*aktywny;
    while (BUTTON_IsPressed(BUTTON_S6)); 
  }
  if (BUTTON_IsPressed(BUTTON_S5)) {
    *time_left = 0;
    *aktywny = 0;
    while (BUTTON_IsPressed(BUTTON_S5)); 
  }
}

int main(void) {
  unsigned int time_left = 0;
  unsigned char current_power = 1;
  unsigned char aktywny = 0;

  AD1PCFG = 0xFFFF;
  TRISB = 0x7FFF;
  TRISD = 0x0000;
  TRISE = 0x0000;

  LCD_init();

  while (1) {
    wyswietlaj(time_left, current_power);
    buttonierka(&time_left, &current_power, &aktywny);
    if (aktywny && time_left > 0) {
      __delay_ms(1000); 
      time_left--; 
    } else if (time_left == 0 && aktywny) {
      LCD_print((unsigned char *)"Done!");
      aktywny = 0; 
    }
  }

  return 0;
}

