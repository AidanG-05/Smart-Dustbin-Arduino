
//Testing LCD & Target Board

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
// addr,en,rw,rs,d4,d5,d6,d7,bl,blpol
// addr can be 0x3F or 0x27

int counterA = 0;
int counterB = 0;
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
    //interrupts();
}
void loop() {
    //Serial.print("The counter is now at: ");
    //Serial.println(counter);
    lcd.setCursor(0, 0);
    lcd.print("A score is:");
    lcd.print(counterA);
    if (counterA == 4)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A is winner");
        lcd.setCursor(0, 1);
        lcd.print("Reset to restart");
        noInterrupts();
    }
    lcd.setCursor(0, 1);
    lcd.print("B score is:");
    lcd.print(counterB);
    delay(100);
    if (counterB == 4)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("B is winner");
        lcd.setCursor(0, 1);
        lcd.print("Reset to restart");
        noInterrupts();
    }
    while ((!digitalRead(2)) && (!digitalRead(4)))
        delay(500);
}

ISR(PCINT2_vect) {
    if (!digitalRead(2))
        counterA++;

    else if (!digitalRead(4))
        counterB++;

}


//Testing IR

#define IR1Pin  A0   // A0 input pin for IR sensor signal
int IR1_Val = 0;     // to store IR sensor 1 signal value

void setup() {
    Serial.begin(9600);   // Setup Serial Communication.               
    Serial.print("Analog IR Sensor 1.\n");
}

void loop() {
    IR1_Val = analogRead(IR1Pin); // Reading and storing IR sensor 1 signal value
    Serial.print("Input Value:");
    Serial.print(IR1_Val);       // Printing IR sensor 1 signal value
    Serial.print("\n");            // moving to new line
    delay(500);                      // Waiting for a while.
    
    
    
  //Test the Relay circuit and motor direction control

#define motionPin 9

void setup() 
{
  pinMode(motionPin, OUTPUT); //digital output for motor direction control
}

void loop() {
// test tamiya car
  digitalWrite(motionPin, LOW); // move toward one direction for 2s
  delay(2000);
  digitalWrite(motionPin, HIGH); //move toward opposite direction for 2s
  delay(2000);
}


   
