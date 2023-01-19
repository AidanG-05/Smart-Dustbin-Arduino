#include <LiquidCrystal_I2C.h>
#include "../../../../Documents/Arduino/libraries/LiquidCrystal_I2C-1.1.2/LiquidCrystal_I2C.h"
#define Hit_aPin 4;
#define Hit_bPin 2;
LiquidCrystal_I2C lcd(0x27, 16, 2);


int reset = 0;
int startStop = 0;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    pinMode(2, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    PCICR |= B00000100; // Enable interrupts on PD port
    PCMSK2 |= B00010100; // Trigger interrupts on pins D2 and D4
    lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.clear();
}

// the loop function runs over and over again until power down or reset
void loop()
{
    lcd.setCursor(0, 0);
    lcd.print("Free to fill");
    lcd.setCursor(0, 1);
    delay(1000);

}

ISR(PCINT2_vect) {
    if (digitalRead(reset) == LOW)
    {
        Serial.print("Free to fill");
    }
    else if (digitalRead(startStop) == LOW) {

    }
}