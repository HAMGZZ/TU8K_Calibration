#include <Arduino.h>
#include <EEPROM.h>
#include "Adafruit_SSD1306.h"

//***** CAL FIELD
unsigned long int CPU_FREQUENCY = 16000000;



#define TONE_OUT 9

#define PWM_PRESCALER 8
// CPU Frequency will be callibrated and stored in EEPROM.
//#define CPU_FREQUENCY 16000000

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long EepromReadLong(int address);
void EepromWriteLong(int address, unsigned long value);
void EepromWriteInt(int address, int value);
unsigned int EepromReadInt(int address);

void OutputTone(float frequency);

void setup() 
{
    pinMode(TONE_OUT, OUTPUT);

    TCCR1A=_BV(COM1A1)|_BV(COM1B1); // set Fast PWM Mode
    TCCR1B=_BV(WGM13)|_BV(CS11);    // Activate PWM Phase, frequency correction Mode

    display.display();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.cp437(true);
    display.print(F("TU8K CALIBRATION"));
    delay(1000);
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("CF: ");
    display.print(CPU_FREQUENCY, 1);
    display.print("OUTPUT: 100Hz");
    EepromWriteLong(0x10, CPU_FREQUENCY);
    OutputTone(100);
}

void loop() 
{
    // do nothing
}


void OutputTone(float frequency)
{ 
    digitalWrite(13, HIGH);
    float timerVal = (CPU_FREQUENCY) / (2 * PWM_PRESCALER * frequency);
    long int timerValInt = timerVal + 0.5;
    ICR1 = timerValInt;
    OCR1A = timerValInt / 2;
    digitalWrite(13, LOW);
}


unsigned long EepromReadLong(int address)
{
    unsigned long word = EepromReadInt(address);
    word = word << 16;
    word = word | EepromReadInt(address+2);
    return word;
}

void EepromWriteLong(int address, unsigned long value) 
{
    EepromWriteInt(address+2, word(value));
    value = value >> 16;
    EepromWriteInt(address, word(value));
}

void EepromWriteInt(int address, int value) 
{
    EEPROM.write(address,highByte(value));
    EEPROM.write(address+1 ,lowByte(value));
}

unsigned int EepromReadInt(int address) 
{
    unsigned int word = word(EEPROM.read(address), EEPROM.read(address+1));
    return word;
}