/*
 Name:		IED_PROJECT.ino
 Created:	1/12/2023 10:17:34 PM
 Author:	Aidan
*/

/*
 Name:		IED_PROJECT.ino
 Created:	1/12/2023 10:17:34 PM
 Author:	Aidan
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);\

#define Hit_aPin 4
#define Hit_bPin 2
int counterA = 0;
int counterB = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(counterA, INPUT_PULLUP);
    pinMode(counterB, INPUT_PULLUP);
    PCICR = B00000100;
    PCMSK2 = B00010100;
    lcd.init();
    lcd.setCursor(0, 0);
    lcd.begin(16, 2); // 16 characters, 2 lines
    lcd.backlight();  // turn backlight on
    lcd.setCursor(0, 0); // position cursor at character 0, line 0
    lcd.print("Game is ready");
    delay(500);
    lcd.setCursor(0, 1); // position cursor at character 0, line 1
    lcd.print("for Hit A and B");
    delay(100);
}

void loop()
{
    Serial.print("hit A score is: ");
    Serial.println(counterA);
    Serial.print("hit B score is: ");
    Serial.println(counterB);
    Serial.println("ready for next hit");
    delay(1000);
}

ISR(PCINT2_vect) {
    if (digitalRead(counterA) == LOW) {
        counterA += 1;
    }
    else if (digitalRead(counterB) == LOW) {
        counterB += 1;
    }
}