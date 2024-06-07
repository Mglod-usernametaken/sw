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
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include "bsp/adc.h"
#include "bsp/buttons.h"
#include <libpic30.h>
#include <xc.h>

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
  }
  if (row == 2) {
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
//--------------------------------------------------------
unsigned char symbol1[8] = {0b00011, 0b00100, 0b01010, 0b01001,
                            0b01010, 0b00100, 0b00011, 0b00000};

unsigned char symbol2[8] = {0b10000, 0b01000, 0b10100, 0b00100,
                            0b10100, 0b01000, 0b10000, 0b00000};
//--------------------------------------------------------
void intToStr(unsigned int num, char *str, unsigned int width) {
  str[width] = '\0';
  for (int i = width - 1; i >= 0; i--) {
    str[i] = (num % 10) + '0';
    num /= 10;
  }
}

//--------------------------------------------------------
void mikrofaluj() {
  unsigned int moc;
  unsigned int czas;
  char timeStr[4];

  ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
  ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);

  LCD_setCursor(1, 2);
  LCD_print("Hell o");
  __delay_ms(2000);
  LCD_sendCommand(LCD_CLEAR);

  LCD_setCursor(1, 2);
  LCD_print("ustaw moc:");
  LCD_sendData(moc + '0');

  while (1) {
    moc = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER) >> 2;
    if (moc == 0xFF)
      continue;
    LCD_setCursor(2, 2);
    LCD_print("ustaw moc:");
    LCD_sendData(moc + '0');

    while(1){
        if (BUTTON_IsPressed(BUTTON_S4) == true) {break;}
    }

    if (BUTTON_IsPressed(BUTTON_S4) == true) {
      __delay_ms(500);
      if (BUTTON_IsPressed(BUTTON_S4) == true)
        break;
    }
  }
  LCD_sendCommand(LCD_CLEAR);

  LCD_setCursor(1, 2);
  LCD_print("czas: ");
  czas = 0;
  while (1) {
    czas = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER) >> 2;
    if (czas == 0xFF)
      continue;
    LCD_setCursor(2, 2);
    LCD_sendData(czas);
    if (BUTTON_IsPressed(BUTTON_S4) == true) {break;}
  }

  LCD_sendCommand(LCD_CLEAR);

  LCD_setCursor(1, 2);
  LCD_print("nacisnij start");
  while (BUTTON_IsPressed(BUTTON_S4) == false);
  __delay_ms(300);

  while (BUTTON_IsPressed(BUTTON_S4) == false);
  __delay_ms(300);


  while (czas > 0) {
    LCD_setCursor(1, 0);
    LCD_print("falowanie:");
    intToStr(czas, timeStr, 3);
    // sprintf(timeStr,"%03u", czas);
    LCD_setCursor(2, 0);
    LCD_print(timeStr);
    __delay_ms(1000);
    czas--;
  }

  LCD_sendCommand(LCD_CLEAR);
  LCD_setCursor(1, 0);
  LCD_print("done.");
  __delay_ms(20000);
  LCD_sendCommand(LCD_CLEAR);
}

int main(void) {
  TRISB = 0x7FFF;
  TRISD = 0x0000;
  TRISE = 0x0000;

  LCD_init();

  while (1) {
    mikrofaluj();
  }

  return 0;
}
